import time
from tkinter import *
from tkinter import ttk

from App.custom_elements import InputSlider
from App.custom_elements import TwoStateBtn

import struct
from Communication.Protocol import *
from App.global_vars import serial_handler, get_remote_position, get_remote_max_position_, set_remote_max_position

app_tab_el = None

position = 0
pos_indicator = None
pos_input = None
act_pos_entry_var = StringVar()

def start():
    print('start')


def stop():
    print('stop')


def set_position_callback(data):
    try:
        resp_dec = deconstruct_message(data)
    except:
        pass


def set_position():
    global position
    data = struct.pack('>BI', SET_REFERENCE_POSITION_e, int(position*1000))
    bytes = construct_message(HeaderId.COMMAND_e, data)
    state = serial_handler.new_transaction(bytes, priority=0, callback=set_position_callback)
    if state is False:
        serial_handler.flush_transactions()
        serial_handler.new_transaction(bytes, priority=0, callback=set_position_callback)


def write_position(val):
    global position
    position = val


def write_max_position_callback(data):
    try:
        resp_dec = deconstruct_message(data)
        max_pos = struct.unpack('>I', resp_dec.payload[1:])
        set_remote_max_position(max_pos[0])
        pass
    except:
        pass


def update_positions():
    global app_tab_el, act_pos_entry_var
    max = get_remote_max_position_()
    if max == 0.0:
        data = struct.pack('>B', GET_MAXIMUM_POSITION_e)
        bytes = construct_message(HeaderId.COMMAND_e, data)
        serial_handler.new_transaction(bytes, priority=0, callback=write_max_position_callback)

    curr_pos = get_remote_position()
    act_pos_entry_var.set(str(curr_pos))
    if curr_pos > 0.1:
        curr_pos -= 0.1
    pos_input.slider.config(to=max)
    pos_input.update_limits((0, max))
    pos_indicator.config(maximum=max)
    pos_indicator.step(curr_pos)
    app_tab_el.after(100, update_positions)


def application_ctrl_tab(root):
    global app_tab_el, pos_indicator, pos_input, act_pos_entry_var
    app_tab_el = ttk.LabelFrame(root, text='Position control')
    app_tab_el.columnconfigure(0, weight=1)
    app_tab_el.after(100, update_positions)

    pos_input = InputSlider(app_tab_el, limits=(0, 0), callback=write_position)
    pos_input.elmnt.grid(row=0, column=0, sticky='NSEW')

    set_position_btn = Button(app_tab_el, text='Set', command=set_position)
    set_position_btn.grid(row=0, column=1, sticky='NSEW')

    start_stop_btn = TwoStateBtn(app_tab_el, callbacks=(start, stop), default_state=1)
    start_stop_btn.elmnt.grid(row=0, column=2, sticky='NSEW')

    pos_indicator = ttk.Progressbar(pos_input.elmnt, orient=HORIZONTAL)
    pos_indicator.grid(row=1, column=0, sticky='NSEW')

    act_pos_entry = Entry(pos_input.elmnt, state='readonly', textvariable=act_pos_entry_var, justify='center', width= 8)
    act_pos_entry.configure(font='Verdana 14')
    act_pos_entry.grid(row=1, column=1, sticky='NSEW')
    return app_tab_el
