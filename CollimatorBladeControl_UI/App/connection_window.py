from tkinter import *
from tkinter import ttk
from Communication.serial_utils import *
from PIL import ImageTk, Image

class ConnectionWindow:
    def __init__(self, root_element, config_handler, serial_handler, toplevel=False, info_text=""):
        self.root_element = root_element
        self.config_handler = config_handler
        self.serial_handler = serial_handler
        self.is_toplevel = toplevel

        self.refresh_btn_img = ImageTk.PhotoImage(Image.open("img/refresh.png").resize((15, 15)))

        # Connection windows widget
        self.connection_window = Toplevel(root_element)
        self.connection_window.title('Connection window')
        self.connection_window.grab_set()
        self.connection_window.attributes("-topmost", True)
        self.connection_window.resizable(False, False)
        icon = ImageTk.PhotoImage(Image.open('img/logo_small.png'))
        self.connection_window.wm_iconphoto(False, icon)

        # Widgets variables
        self.selected_port = StringVar()
        self.selected_baud = StringVar()
        self.output_label_text = StringVar()
        self.output_label_text.set(info_text)
        self.auto_connect_var = IntVar()

        # List of available serial ports
        self.available_ports = []
        get_serial_ports(self.available_ports)

        # Setting input widget variables from config file
        if config_handler.config['serial_port']['auto_connect'] and config_handler.config['serial_port']['port'] != '':
            try:
                port_list_idx = self.available_ports.index(config_handler.config['serial_port']['port'])
                self.selected_port.set(self.available_ports[port_list_idx])
            except ValueError:
                if len(self.available_ports) != 0:
                    self.selected_port.set(self.available_ports[0])

        self.selected_baud.set(str(config_handler.config['serial_port']['baud_rate']))
        self.auto_connect_var.set(int(config_handler.config['serial_port']['auto_connect']))

        # Label widget
        device_port_selection_label = Label(self.connection_window, text='Serial port:')
        device_port_selection_label.grid(row=0, column=0, sticky='E')
        port_baud_label = Label(self.connection_window, text='Baudrate:')
        port_baud_label.grid(row=1, column=0, sticky='E')

        # Input widgets
        self.connection_combobox = ttk.Combobox(self.connection_window, values=self.available_ports, justify='center',
                                                textvariable=self.selected_port)
        self.connection_combobox.option_add('*TCombobox*Listbox.Justify', 'center')
        self.connection_combobox.grid(row=0, column=1, sticky='EW')

        self.baud_rate_input = ttk.Entry(self.connection_window, textvariable=self.selected_baud, justify='center')
        self.baud_rate_input.grid(row=1, column=1, sticky='EW')

        self.auto_reconnect_checkbox = Checkbutton(self.connection_window, text='Auto connect',
                                                   variable=self.auto_connect_var)
        self.auto_reconnect_checkbox.grid(row=2, column=1, sticky='W')

        # Button widgets
        refresh_btn = Button(self.connection_window, text='Refresh', image=self.refresh_btn_img,
                             command=self.__refresh_serial_devices_connections__)
        refresh_btn.grid(row=0, column=2, sticky='NSEW')

        connect_btn = Button(self.connection_window, text="Connect", padx=10, pady=5,
                             command=self.__connect_callback__)  # , background='#0000e0', foreground='#FFFFFF'
        connect_btn.grid(row=3, column=2, padx=(10, 2), pady=(5, 2), sticky='E')

        # Output label
        self.output_label = Label(self.connection_window, textvariable=self.output_label_text, justify="left")
        self.output_label.grid(row=3, column=0, columnspan=2, sticky='NSEW')

        self.connection_window.protocol("WM_DELETE_WINDOW", self.__window_close_handler__)

    def __connect_callback__(self):
        try:
            self.config_handler.config['serial_port']['baud_rate'] = int(self.selected_baud.get())
            self.config_handler.config['serial_port']['port'] = self.selected_port.get()
            self.config_handler.config['serial_port']['auto_connect'] = bool(self.auto_connect_var.get())
        except ValueError:
            self.output_label_text.set('Bad baud rate.')
            return

        self.serial_handler.serial_port = self.selected_port.get()
        self.serial_handler.serial_baud = int(self.selected_baud.get())
        if self.serial_handler.connect() is False:
            self.output_label_text.set('Could not open port ' + self.selected_port.get())
            return
        self.output_label_text.set('Connected')
        self.config_handler.write_config()
        self.connection_window.after(1000, self.connection_window.destroy)

    def __refresh_serial_devices_connections__(self):
        get_serial_ports(self.available_ports)
        self.connection_combobox['values'] = self.available_ports
        if len(self.available_ports) != 0:
            self.connection_combobox.current(0)

    def __window_close_handler__(self):
        if self.is_toplevel:
            self.root_element.destroy()
        else:
            self.connection_window.destroy()
