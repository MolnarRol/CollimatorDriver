from tkinter import *
from tkinter import ttk
from Communication.serial_utils import *

class ConnectionWindow:
    def __init__(self, root_element):
        self.available_ports = []
        self.sellected_port = None

        get_serial_ports(self.available_ports)

        connection_window = Toplevel(root_element)
        connection_window.title('Connection window')
        connection_window.grab_set()

        l1 = Label(connection_window, text='Sellect device port:')
        l1.grid(row=0, column=0)

        connection_combobox = ttk.Combobox(connection_window, values=self.available_ports)
        connection_combobox.grid(row=0, column=1)

        refresh_btn = Button(connection_window, text="Refresh devices", command=get_serial_ports(self.available_ports))
        refresh_btn.grid(row=0, column=2)

        connect_btn = Button(connection_window, text="Connect", padx=10,
                             pady=5)  # , background='#0000e0', foreground='#FFFFFF'
        connect_btn.grid(row=1, column=3, padx=(10, 2), pady=(5, 2))

    def __connect_callback__(self):
        pass