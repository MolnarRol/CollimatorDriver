import time
from tkinter import *
from tkinter import ttk

from App.custom_elements import InputSlider
from App.custom_elements import TwoStateBtn

import struct
from Communication.Protocol import *
from App.global_vars import serial_handler

app_tab_el = None

position = 0


def start():
    print('start')


def stop():
    print('stop')



def set_position_callback(data):
    try:
        resp_dec = deconstruct_message(data)
        if resp_dec.payload[0] != 0:
            print('Failed')
    except:
        print('Failed')


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


def application_ctrl_tab(root):
    global app_tab_el
    app_tab_el = ttk.LabelFrame(root, text='Position control')
    app_tab_el.columnconfigure(0, weight=1)

    pos_input = InputSlider(app_tab_el, limits=(0, 10000), callback=write_position)
    pos_input.elmnt.grid(row=0, column=0, sticky='NSEW')

    set_position_btn = Button(app_tab_el, text='Set', command=set_position)
    set_position_btn.grid(row=0, column=1, sticky='NSEW')

    start_stop_btn = TwoStateBtn(app_tab_el, callbacks=(start, stop), default_state=1)
    start_stop_btn.elmnt.grid(row=0, column=2, sticky='NSEW')

    pos_indicator = ttk.Progressbar(pos_input.elmnt, orient=HORIZONTAL, maximum=10000)
    pos_indicator.grid(row=1, column=0, sticky='NSEW')
    pos_indicator.step(10000-1)

    return app_tab_el
