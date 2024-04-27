from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from App.custom_elements import ParamField

param_tab_el = None

parameters_strings = {
    'max_speed': None,
    'max_accel': None,
    'max_force': None
}

parameters = {
    'max_speed': None,
    'max_accel': None,
    'max_force': None
}


def save_cmd():
    err_string = ""
    for key in parameters_strings:
        print(key, "->", parameters_strings[key].get())
        try:
            parameters[key] = float(parameters_strings[key].get())
        except ValueError:
            err_string += '[' + key + '] = ' + parameters_strings[key].get() + ' -> Not a number.\n'

    if len(err_string) != 0:
        messagebox.showerror('We fcked up', err_string)


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

    load_btn = Button(btn_frame, text='Load')
    load_btn.grid(row=0, column=1)

    save_btn = Button(btn_frame, text='Save', command=save_cmd)
    save_btn.grid(row=0, column=2)

    param_grp1 = ttk.LabelFrame(param_tab_el, text='Param group 1')
    param_grp1.grid(row=0, column=0, columnspan=2, sticky='NSEW')

    param1 = ParamField(param_grp1, name='Max. speed [rad/s]', variable=parameters_strings['max_speed'])
    param1.elmnt.grid(row=0, column=0, sticky='NSEW')

    param2 = ParamField(param_grp1, name='Max. acceleration [rad/s2]', variable=parameters_strings['max_accel'])
    param2.elmnt.grid(row=1, column=0, sticky='NSEW')

    param3 = ParamField(param_grp1, name='Max. force [N]', variable=parameters_strings['max_force'])
    param3.elmnt.grid(row=2, column=0, sticky='NSEW')

    return param_tab_el
