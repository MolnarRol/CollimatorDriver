import tkinter as tk

from App.config_file_handler import ConfigHandler
from Communication.threaded_serial import SerialInterface

config_handler = ConfigHandler(config_file_name='appconfig.json')
serial_handler = SerialInterface(config_handler.config['serial_port']['port'],
                                 config_handler.config['serial_port']['baud_rate'])

# Root tkinter element
root = tk.Tk()

transaction_lock = True


def set_transaction_lock(new_lock: bool):
    global transaction_lock
    transaction_lock = new_lock


def get_transaction_lock():
    global transaction_lock
    return transaction_lock


INFO_String = ('Collimator blade control interface.\n'
               'Version: 0.1\n'
               'Company: ES3\n'
               'Authors:\n'
               '- Roland Molnar (Team leader)\n\temail: roland.molnar.3@student.tuke.sk\n'
               '- Bc. Samuel Fertal (Developer)\n\t email: samuel.fertal@student.tuke.sk\n'
               '- Bc. Vadym Holysh (Developer)\n\t email: vadym.holysh@student.tuke.sk')

