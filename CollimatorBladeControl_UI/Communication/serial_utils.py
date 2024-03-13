from serial import *
import serial.tools.list_ports


def get_serial_ports(output):
    output.clear()
    for port in serial.tools.list_ports.comports():
        output.append(port.device)


if __name__ == '__main__':
    port = []
    get_serial_ports(port)
    print(port)

