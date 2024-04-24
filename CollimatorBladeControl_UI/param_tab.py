from tkinter import *
from tkinter import ttk
from tkinter import messagebox

class ParamField:
    def __init__(self, root, name='Placeholder', variable=None):
        self.elmnt = ttk.Frame(root)
        if variable is None:
            self.inputVar = StringVar()
        else:
            self.inputVar = variable

        self.elmnt.columnconfigure(0, weight=1)
        self.elmnt.columnconfigure(1, weight=0)
        label = ttk.Label(self.elmnt, text=name, justify=LEFT, anchor='w')
        label.grid(row=0, column=0, sticky='EW')

        input = Entry(self.elmnt, width=14, justify='center', textvariable=self.inputVar)
        input.grid(row=0, column=1, sticky='EW')

    def get_input(self):
        return self.inputVar.get()

    def set_input(self, input_text):
        self.inputVar.set(input_text)

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

    messagebox.showerror('We fcked up', err_string)

    
def parameter_tab(root):
    for key in parameters_strings:
        parameters_strings[key] = StringVar()
        parameters_strings[key].set('Not loaded')

    global param_tab_el
    param_tab_el = ttk.Frame(root)
    load_btn = Button(param_tab_el, text='Load')
    load_btn.grid(row=10, column=9)

    save_btn = Button(param_tab_el, text='Save', command=save_cmd)
    save_btn.grid(row=10, column=10)

    param_grp1 = ttk.LabelFrame(param_tab_el, text='Param group 1')
    param_grp1.grid(row=0, column=0)

    param1 = ParamField(param_grp1, name='Max. speed [rad/s]', variable=parameters_strings['max_speed'])
    param1.elmnt.grid(row=0, column=0, sticky='NSEW')

    param2 = ParamField(param_grp1, name='Max. acceleration [rad/s2]', variable=parameters_strings['max_accel'])
    param2.elmnt.grid(row=1, column=0, sticky='NSEW')

    param3 = ParamField(param_grp1, name='Max. force [N]', variable=parameters_strings['max_force'])
    param3.elmnt.grid(row=2, column=0, sticky='NSEW')

    return param_tab_el
