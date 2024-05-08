from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from App.custom_elements import ParamField
from Communication.Protocol import *
from App.global_vars import serial_handler
param_tab_el = None

parameters_strings = {
    'max_speed': None,
    'max_accel': None,
    'max_force': None
}

parameters = {
    'max_speed': float,
    'max_accel': float,
    'max_force': float
}


def save_cmd():
    err_string = ""
    for key in parameters_strings:
        try:
            parameters[key] = float(parameters_strings[key].get()) * 1000
        except ValueError:
            err_string += '[' + key + '] = ' + parameters_strings[key].get() + ' -> Not a number.\n'

    if len(err_string) != 0:
        messagebox.showerror('Value error: Parameter are floating point numbers.', err_string)
    else:
        data = struct.pack('>BIII',
                           SET_MOVEMENT_PARAMETERS_e,
                           int(parameters['max_speed']),
                           int(parameters['max_accel']),
                           int(parameters['max_force']))
        bytes = construct_message(HeaderId.COMMAND_e, data)
        serial_handler.new_transaction(bytes, priority=1)


def load_cmd_callback(data):
    try:
        resp_dec = deconstruct_message(data)
        loc_parameters = struct.unpack('>III', resp_dec.payload[1:])
        (parameters['max_speed'], parameters['max_accel'], parameters['max_force']) = struct.unpack('>III',
                                                                                                    resp_dec.payload[1:])
        for key in parameters_strings:
            parameters_strings[key].set(str(parameters[key] / 1000))
    except:
        pass

def load_cmd():
    global parameters
    data = struct.pack('>B', GET_MOVEMENT_PARAMETERS_e)
    bytes = construct_message(HeaderId.COMMAND_e, data)
    serial_handler.new_transaction(bytes, priority=1, callback=load_cmd_callback)


def parameter_tab(root):
    for key in parameters_strings:
        parameters_strings[key] = StringVar()
        parameters_strings[key].set('Not loaded')

    global param_tab_el
    param_tab_el = ttk.Frame(root)
    param_tab_el.columnconfigure(0, weight=1)
    param_tab_el.rowconfigure(0, weight=1)

    btn_frame = Frame(param_tab_el)
    btn_frame.grid(row=1, column=0, sticky='NSEW')
    btn_frame.columnconfigure(0, weight=1)

    load_btn = Button(btn_frame, text='Load', command=load_cmd)
    load_btn.grid(row=0, column=1)

    save_btn = Button(btn_frame, text='Save', command=save_cmd)
    save_btn.grid(row=0, column=2)

    param_grp1 = ttk.LabelFrame(param_tab_el, text='Movement parameters')
    param_grp1.grid(row=0, column=0, columnspan=2, sticky='NSEW')

    param1 = ParamField(param_grp1, name='Max. speed [rad/s]', variable=parameters_strings['max_speed'])
    param1.elmnt.grid(row=0, column=0, sticky='NSEW')

    param2 = ParamField(param_grp1, name='Max. acceleration [rad/s2]', variable=parameters_strings['max_accel'])
    param2.elmnt.grid(row=1, column=0, sticky='NSEW')

    # param3 = ParamField(param_grp1, name='Max. force [N]', variable=parameters_strings['max_force'])
    # param3.elmnt.grid(row=2, column=0, sticky='NSEW')

    return param_tab_el
