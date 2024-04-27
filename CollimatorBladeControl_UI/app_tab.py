from tkinter import *
from tkinter import ttk
from custom_elements import InputSlider
from custom_elements import TwoStateBtn
from custom_elements import DoubleEntry

app_tab_el = None


def start():
    print('start')


def stop():
    print('stop')


def application_ctrl_tab(root):
    global app_tab_el
    app_tab_el = ttk.LabelFrame(root, text='Position control')
    app_tab_el.columnconfigure(0, weight=1)

    pos_input = InputSlider(app_tab_el, limits=(0, 10000))
    pos_input.elmnt.grid(row=0, column=0, sticky='NSEW')

    start_stop_btn = TwoStateBtn(app_tab_el, callbacks=(start, stop), default_state=1)
    start_stop_btn.elmnt.grid(row=0, column=1, sticky='NSEW')

    return app_tab_el
