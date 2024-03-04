from tkinter import *
import serial

com_device = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.01)
# Root tkinter element
root = Tk()


def set_len_on():
    com_device.write(bytearray([0x1]))
    pass


def set_led_off():
    com_device.write(bytearray([0x0]))
    pass


if __name__ == '__main__':
    if com_device.is_open:
        com_device.close()
    com_device.open()

    led_on_btn = Button(root, text="ON", command=set_len_on, padx=10, pady=10)
    led_off_btn = Button(root, text="OFF", command=set_led_off, padx=10, pady=10)
    led_on_btn.grid(row=0, column=0)
    led_off_btn.grid(row=0, column=1)

    root.mainloop()
