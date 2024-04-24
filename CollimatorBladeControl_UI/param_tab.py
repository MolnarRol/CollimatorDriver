from tkinter import *
from tkinter import ttk

param_tab_el = None

class ParamField:
    def __init__(self, root, name='Placeholder'):
        self.elmnt = ttk.Frame(root)
        self.elmnt.columnconfigure(0, weight=1)
        label = Label(self.elmnt, text=name, padx=5, pady=2, anchor='e')
        label.grid(row=0, column=0, sticky='E')

        input = Entry(self.elmnt, width=10, justify='center')
        input.grid(row=0, column=1, sticky='W')

def parameter_tab(root):
    param_tab_el = ttk.Frame(root)
    load_btn = Button(param_tab_el, text='Load')
    load_btn.grid(row=10, column=9)

    save_btn = Button(param_tab_el, text='Save')
    save_btn.grid(row=10, column=10)

    param_grp1 = ttk.LabelFrame(param_tab_el, text='Param group 1')
    param_grp1.grid(row=0, column=0)

    param1 = ParamField(param_grp1, name='Max. speed [rad/s]')
    param1.elmnt.grid(row=0, column=0)

    param2 = ParamField(param_grp1, name='Max. acceleration [rad/s2]')
    param2.elmnt.grid(row=1, column=0)

    param3 = ParamField(param_grp1)
    param3.elmnt.grid(row=2, column=0)

    return param_tab_el