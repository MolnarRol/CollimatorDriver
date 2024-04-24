from tkinter import *
from tkinter import ttk

data_disp_el = None

def data_display(root):
    data_disp_el = ttk.LabelFrame(root, text='Data', width=350)
    return data_disp_el
