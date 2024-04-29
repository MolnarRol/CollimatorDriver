import struct
from tkinter import *
from tkinter import ttk

from App.connection_window import *

from App.param_tab import parameter_tab
from App.data_disp import data_display
from App.app_tab import application_ctrl_tab
from tkinter import messagebox
from Communication.Protocol import *

from App.global_vars import *

INFO_String = ('Collimator blade control interface.\n'
               'Version: 0.1\n'
               'Company: ES3\n'
               'Authors:\n'
               '- Roland Molnar (Team leader)\n\temail: roland.molnar.3@student.tuke.sk\n'
               '- Bc. Samuel Fertal (Developer)\n\t email: samuel.fertal@student.tuke.sk\n'
               '- Bc. Vadym Holysh (Developer)\n\t email: vadym.holysh@student.tuke.sk')

# Application variables


# Root tkinter element
root = Tk()
# root.geometry('640x480')
root.resizable(False, False)
root.title('Collimator control interface')
icon = ImageTk.PhotoImage(Image.open('img/logo_small.png'))
root.wm_iconphoto(False, icon)

root.rowconfigure(0, weight=1)
root.columnconfigure(0, weight=1)


def __connection_settings_clicked__():
    loc_connection_window = ConnectionWindow(root, config_handler, serial_handler)
    del loc_connection_window
    pass


def __info_menu_clicked__():
    messagebox.showinfo('Software information', INFO_String)
    pass

d = 0
def send():
    serial_handler.transaction_queue.put((0, 'test'))
    pass


if __name__ == '__main__':
    # Application menu
    top_bar_menu = Menu(root)
    top_bar_menu.add_command(label='Connection settings', command=__connection_settings_clicked__)
    top_bar_menu.add_command(label='Info', command=__info_menu_clicked__)

    # Serial port configuration
    if config_handler.config['serial_port']['auto_connect'] is False:
        connection_window = ConnectionWindow(root, config_handler, serial_handler, toplevel=True)
        del connection_window
    else:
        if serial_handler.connect() is False:
            connection_window = ConnectionWindow(root, config_handler, serial_handler, toplevel=True,
                                                 info_text=("Could not connect to " +
                                                            config_handler.config['serial_port']['port']))
            del connection_window

    app_control_tab = application_ctrl_tab(root)
    app_control_tab.grid(row=0, column=0, columnspan=2, sticky='NSEW')
    parameter_tab = parameter_tab(root)
    parameter_tab.grid(row=1, column=0, sticky='NSEW')

    data_display_frame = data_display(root)
    data_display_frame.grid(row=1, column=1, sticky='NSEW')

    # Debug
    send_btn = Button(root, text='send', command=send)
    send_btn.grid(row=2, column=1, sticky='NSEW')

    root.config(menu=top_bar_menu)
    root.mainloop()
