"""
    File: simple_serial_tester.py
    Author: Bc. Roland Molnar
"""
from tkinter import *
from tkinter import ttk


class SerialTester:
    def __init__(self, root):
        self.element = Frame(root)
        self.element.rowconfigure(index=0, weight=1)
        self.element.rowconfigure(index=1, weight=0)
        self.element.columnconfigure(0, weight=1)

        # Communication explorer
        self.comm_exp = ttk.LabelFrame(self.element, text='Communication log')
        self.comm_exp.grid(row=0, column=0, sticky='NSEW', pady=[10, 2])

        # Input frame elements
        self.input_frame = Frame(self.element)
        self.input_frame.grid(row=1, column=0, sticky='NSEW')
        self.input_frame.columnconfigure(0, weight=0)
        self.input_frame.columnconfigure(1, weight=1)
        self.input_frame.columnconfigure(2, weight=0)

        self.input_text = Label(self.input_frame, text='Data:')
        self.input_text.grid(row=0, column=0, sticky='NSEW')

        self.input_box = Entry(self.input_frame)
        self.input_box.grid(row=0, column=1, sticky='NSEW')

        self.send_btn = Button(self.input_frame, text='Send', padx=10, pady=5)
        self.send_btn.grid(row=0, column=2, sticky='NSEW', padx=5)
