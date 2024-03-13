from tkinter import *
from tkinter import ttk
from config_file_handler import ConfigHandler
from connection_window import *
from Communication.threaded_serial import SerialInterface
from simple_serial_tester import SerialTester
from info_view import *


# Application variables
config_handler = ConfigHandler(config_file_name='appconfig.json')
serial_handler = SerialInterface(config_handler.config['serial_port']['port'],
                                 config_handler.config['serial_port']['baud_rate'])

# Root tkinter element
root = Tk()
root.title('Collimator control interface')
root.rowconfigure(0, weight=1)
root.columnconfigure(0, weight=1)


def __connection_settings_clicked__():
    loc_connection_window = ConnectionWindow(root, config_handler, serial_handler)
    del loc_connection_window
    pass

def __info_menu_clicked__():
    info_window = InfoView(root)
    pass


if __name__ == '__main__':
    # Application menu
    top_bar_menu = Menu(root)
    top_bar_menu.add_command(label='Connection settings', command=__connection_settings_clicked__)
    top_bar_menu.add_command(label='Info', command=__info_menu_clicked__)

    # Serial port configuration
    if config_handler.config['serial_port']['auto_connect'] is False:
        connection_window = ConnectionWindow(root, config_handler, serial_handler)
        del connection_window
    else:
        if serial_handler.connect() is False:
            connection_window = ConnectionWindow(root, config_handler, serial_handler,
                                                 info_text=("Could not connect to " +
                                                            config_handler.config['serial_port']['port']))
            del connection_window

    # serial_tester = SerialTester(root)
    # serial_tester.element.grid(row=0, column=0, sticky='NSEW')
    info_window = InfoView(root)

    root.config(menu=top_bar_menu)
    root.mainloop()
