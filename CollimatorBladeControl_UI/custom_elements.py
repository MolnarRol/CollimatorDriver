from tkinter import *
from tkinter import ttk


class ParamField:
    def __init__(self, root, name='Placeholder', variable=None, entry_state='normal'):
        self.elmnt = ttk.Frame(root)
        if variable is None:
            self.inputVar = StringVar()
        else:
            self.inputVar = variable

        self.elmnt.columnconfigure(0, weight=1)
        self.elmnt.columnconfigure(1, weight=0)
        label = ttk.Label(self.elmnt, text=name, justify=LEFT, anchor='w')
        label.grid(row=0, column=0, sticky='EW')

        input_entry = Entry(self.elmnt, width=14, justify='center', textvariable=self.inputVar, state=entry_state)
        input_entry.grid(row=0, column=1, sticky='EW')
