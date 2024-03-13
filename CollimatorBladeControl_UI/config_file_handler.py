"""
    File: config_file_handler.py
    Description: Class for loading/saving configuration to configuration file
    Author: Bc. Roland Molnar
"""

import json


class ConfigHandler:
    DEFAULT_CONFIG = {
        'serial_port': {
            'port': '',
            'baud_rate': 115200,
            'auto_connect': False
        }
    }
    
    def __init__(self, config_file_name: str):
        self.config_file_name = config_file_name
        self.config = {}
        self.config_file = None

        # File was found
        try:
            self.config_file = open(config_file_name, 'r+')
            self.config = json.loads(self.config_file.read())
            self.config_file.close()

        # Create new config file from default config
        except (FileNotFoundError, json.decoder.JSONDecodeError):
            self.config_file = open(config_file_name, 'w+')
            self.config_file.write(json.dumps(self.DEFAULT_CONFIG))
            self.config_file.close()
            self.config = self.DEFAULT_CONFIG

    def get_config(self):
        return self.config

    def write_config(self, new_config: dict = None):
        if new_config is None:
            new_config = self.config
        self.config_file = open(self.config_file_name, 'w+')
        self.config = new_config
        self.config_file.write(json.dumps(new_config))
        self.config_file.close()

    def close_config_file(self):
        self.config_file.close()


if __name__ == '__main__':
    test_handler = ConfigHandler('config.json')
    pass
