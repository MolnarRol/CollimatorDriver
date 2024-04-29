from App.config_file_handler import ConfigHandler
from Communication.threaded_serial import SerialInterface

config_handler = ConfigHandler(config_file_name='appconfig.json')
serial_handler = SerialInterface(config_handler.config['serial_port']['port'],
                                 config_handler.config['serial_port']['baud_rate'])
