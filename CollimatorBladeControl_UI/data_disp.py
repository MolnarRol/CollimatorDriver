from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from custom_elements import ParamField


data_disp_el = None


def data_display(root):
    global data_disp_el
    data_disp_el = ttk.LabelFrame(root, text='Data', width=350)

    # Mechanical data
    mech_data_frame = ttk.LabelFrame(data_disp_el, text='Mechanical')
    mech_data_frame.columnconfigure(0, weight=1)

    speed_data = ParamField(mech_data_frame, 'Speed [rad/s]', entry_state='readonly')
    speed_data.elmnt.grid(row=0, column=0, sticky='NSEW')

    rotor_position = ParamField(mech_data_frame, 'Rotor position [deg]', entry_state='readonly')
    rotor_position.elmnt.grid(row=1, column=0, sticky='NSEW')

    mech_data_frame.grid(row=0, column=0, sticky='NSEW')

    # Electrical data
    el_data_frame = ttk.LabelFrame(data_disp_el, text='Electrical')
    el_data_frame.columnconfigure(0, weight=1)

    iq_data = ParamField(el_data_frame, 'iq [A]', entry_state='readonly')
    iq_data.elmnt.grid(row=0, column=0, sticky='NSEW')

    id_data = ParamField(el_data_frame, 'iq [A]', entry_state='readonly')
    id_data.elmnt.grid(row=1, column=0, sticky='NSEW')

    dc_link_voltage = iq_data = ParamField(el_data_frame, 'Link voltage [V]', entry_state='readonly')
    dc_link_voltage.elmnt.grid(row=2, column=0, sticky='NSEW')

    el_data_frame.grid(row=1, column=0, sticky='NSEW')

    return data_disp_el
