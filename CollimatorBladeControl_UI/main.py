from tkinter import *
from tkinter import ttk

import serial
from Communication.serial_utils import *
# com_device = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.01)
# Root tkinter element
root = Tk()


def set_len_on():
    # com_device.write(bytearray([0x1]))
    pass


def set_led_off():
    # com_device.write(bytearray([0x0]))
    pass

def connection_window():
    available_ports = []
    get_serial_ports(available_ports)
    connection_window = Toplevel(root)
    connection_window.title('Connection window')
    connection_window.grab_set()

    l1 = Label(connection_window, text='Sellect device port:')
    l1.grid(row=0, column=0)

    connection_combobox = ttk.Combobox(connection_window, values=available_ports)
    connection_combobox.grid(row=0, column=1)

    refresh_btn = Button(connection_window, text="Refresh devices", command=get_serial_ports(available_ports))
    refresh_btn.grid(row=0, column=2)

    connect_btn = Button(connection_window, text="Connect", padx=10, pady=5) # , background='#0000e0', foreground='#FFFFFF'
    connect_btn.grid(row=1, column=3, padx=(10, 2), pady=(5, 2))

    return None

if __name__ == '__main__':
    # if com_device.is_open:
    #     com_device.close()
    # com_device.open()
    connection_window()


    led_on_btn = Button(root, text="ON", command=set_len_on, padx=10, pady=10)
    led_off_btn = Button(root, text="OFF", command=set_led_off, padx=10, pady=10)
    led_on_btn.grid(row=0, column=0)
    led_off_btn.grid(row=0, column=1)

    root.mainloop()
