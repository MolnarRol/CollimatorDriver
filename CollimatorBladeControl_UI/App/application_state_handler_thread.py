import App.global_vars
from App.global_vars import *
from Communication.Protocol import *
import threading
import struct
from time import sleep
import tkinter as tk
from tkinter import ttk
from PIL import ImageTk, Image

homing_toplevel = None
motor_homing_toplevel = False

torque_error_toplevel = None
torque_error_toplevel_active = False
info_img = ImageTk.PhotoImage(Image.open('img/info.png').resize((60, 60)))
err_img = ImageTk.PhotoImage(Image.open('img/error.png').resize((60, 60)))
err_img_copy = err_img

total_error_toplevel = None
total_error_prev_state = False

def start_application_handler_thread():
    thrd = threading.Thread(target=application_handler_thread, daemon=True)
    thrd.start()


def application_handler_thread():
    while True:
        if serial_handler.is_open:
            bytes = construct_message(HeaderId.HELLO_MSG_e, [])
            serial_handler.new_transaction(bytes, priority=0, callback=application_data_recieved)
            pass
        sleep(0.35)
    pass


def application_data_recieved(data):
    global motor_homing_toplevel, homing_toplevel, torque_error_toplevel, torque_error_toplevel_active
    global total_error_prev_state, total_error_toplevel
    global err_img, info_img, err_img_copy

    try:
        reconstructed = deconstruct_message(data)
        payload_data = struct.unpack('>B', reconstructed.payload)
        movement_enabld = bool(payload_data[0] & 1)
        torque_error = bool(payload_data[0] & (1 << 2))
        homed = bool(payload_data[0] & (1 << 3))
        total_error = bool(payload_data[0] & (1 << 4))
        set_movement_enable_state(movement_enabld)

        if total_error and not total_error_prev_state:
            total_error_toplevel = tk.Toplevel(root)
            total_error_toplevel.title('Error: Homing procedure interrupted.')
            total_error_toplevel.resizable(False, False)
            err_icon = tk.Label(total_error_toplevel, image=err_img_copy)
            err_icon.grid(row=0, column=00, sticky='NSEW', padx=10, pady=10)
            error_text = tk.Label(total_error_toplevel, text="Homing procedure interrupted. Service check needed",
                                  font='Verdana 14')
            error_text.grid(row=0, column=1, sticky='NSEW')

        total_error_prev_state = total_error

        if not homed and not motor_homing_toplevel:
            homing_toplevel = tk.Toplevel(root)
            homing_toplevel.title('Collimator homing')
            homing_toplevel.resizable(False, False)
            homing_toplevel.protocol("WM_DELETE_WINDOW", disable_close)
            info_icon = tk.Label(homing_toplevel, image=info_img)
            info_icon.grid(row=0, column=00, sticky='NSEW', padx=10, pady=10)
            homing_text = tk.Label(homing_toplevel, text="Motor is homing, wait for finish.", font='Verdana 14')
            homing_text.grid(row=0, column=1, sticky='NSEW')
            motor_homing_toplevel = True
            homing_toplevel.grab_set()

        if homed and motor_homing_toplevel:
            homing_toplevel.destroy()
            motor_homing_toplevel = False
            homing_toplevel.grab_release()

        if torque_error and not torque_error_toplevel_active:
            torque_error_toplevel_active = True
            torque_error_toplevel = tk.Toplevel(root)
            torque_error_toplevel.title('Collimator blocked')
            torque_error_toplevel.resizable(False, False)
            torque_error_toplevel.protocol("WM_DELETE_WINDOW", disable_close)
            torque_error_toplevel.grab_set()

            global err_img
            error_icon = tk.Label(torque_error_toplevel, image=err_img)
            error_icon.grid(row=0, column=0, rowspan=2, sticky='NSEW', padx=10, pady=10)

            error_label = tk.Label(torque_error_toplevel,
                                   text='Collimator is blocked.\nAcknowledge this error to resume operation.',
                                   font='Verdana 12')
            error_label.grid(row=0, column=1, sticky='NSEW')
            ack_btn = tk.Button(torque_error_toplevel, text='Acknowledge error', command=command_clear_error,
                                justify='center', font='Verdana 14 bold')
            ack_btn.grid(row=1, column=1, sticky='NSE')

        if not torque_error and torque_error_toplevel_active:
            torque_error_toplevel_active = False
            torque_error_toplevel.grab_release()
            torque_error_toplevel.destroy()

        set_transaction_lock(False)
    except:
        set_transaction_lock(True)
        pass


def clear_error_transaction_callback(data):
    try:
        resp_dec = deconstruct_message(data)
        print(resp_dec.payload)
        if resp_dec.payload[0] == 0x0:
            global torque_error_toplevel_active
            torque_error_toplevel_active = False
            torque_error_toplevel.grab_release()
            torque_error_toplevel.destroy()
    except:
        pass


def command_clear_error():
    data = struct.pack('>B', RESET_ERROR_FLAGS_e)
    bytes = construct_message(HeaderId.COMMAND_e, data)
    serial_handler.new_transaction(bytes, priority=0, callback=clear_error_transaction_callback)
    print('Clearing')
    pass


def disable_close():
    pass
