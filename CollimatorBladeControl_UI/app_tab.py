from tkinter import *
from tkinter import ttk

app_tab_el = None


def application_ctrl_tab(root):
    global app_tab_el
    app_tab_el = ttk.LabelFrame(root, text='Position control')
    label = Label(app_tab_el, text='test')
    label.grid(row=0, column=0)
    return app_tab_el
