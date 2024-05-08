import tkinter as tk

from App.config_file_handler import ConfigHandler
from Communication.threaded_serial import SerialInterface

config_handler = ConfigHandler(config_file_name='appconfig.json')
serial_handler = SerialInterface(config_handler.config['serial_port']['port'],
                                 config_handler.config['serial_port']['baud_rate'])

# Root tkinter element
root = tk.Tk()

transaction_lock = True
remote_position = 0.0
remote_max_position = 0.0
movement_enable_state = False


def set_movement_enable_state(new_state):
    global movement_enable_state
    movement_enable_state = new_state


def get_movement_enable_state():
    global movement_enable_state
    return movement_enable_state


def set_remote_max_position(max_pos):
    global remote_max_position
    remote_max_position = max_pos


def get_remote_max_position_():
    global remote_max_position
    return remote_max_position


def set_remote_position(pos):
    global remote_position
    remote_position = pos


def get_remote_position():
    global remote_position
    return remote_position


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

