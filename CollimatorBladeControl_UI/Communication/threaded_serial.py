import serial
import threading
import time


class SerialInterface:
    def __init__(self, serial_port: str, serial_baud: int):
        self.serial_port = serial_port
        self.serial_baud = serial_baud
        self.serial_if = serial.Serial()

        self.tx_thread = threading.Thread()
        self.rx_thread = threading.Thread()

    def connect(self):
        self.serial_if.port = self.serial_port
        self.serial_if.baudrate = self.serial_baud

        try:
            if self.serial_if.is_open:
                self.serial_if.close()
            self.serial_if.open()
        except serial.SerialException:
            return False
        return True
