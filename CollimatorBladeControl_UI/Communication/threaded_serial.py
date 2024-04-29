import serial
import threading
import time


class SerialInterface:
    serial_transaction_timeout = 0.05

    def __init__(self, serial_port: str, serial_baud: int):
        self.serial_port = serial_port
        self.serial_baud = serial_baud
        self.serial_if = serial.Serial()
        self.serial_if.timeout = self.serial_transaction_timeout

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

    def transaction_start(self, out_bytes=[]):
        if len(out_bytes) == 0:
            return None
        self.serial_if.write(out_bytes)
        return self.serial_if.read(256)

