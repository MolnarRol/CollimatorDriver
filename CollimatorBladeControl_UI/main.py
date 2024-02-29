from tkinter import ttk
from tkinter import *
from PIL import Image, ImageTk
import serial

root_el = Tk()                                                                      # Root application element
# Icon setupw
root_el.wm_iconphoto(False,                                                  # Project icon setup
                     ImageTk.PhotoImage(Image.open('logo_small.png')))

serial.Serial('/dev/TTYUSB0', 19200, timeout=0)

def app():
    
    # root_el.mainloop()


if __name__ == '__main__':
    app()
