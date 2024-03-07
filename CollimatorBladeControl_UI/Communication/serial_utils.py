import serial
import serial.tools.list_ports as ports

def get_serial_ports(output):
    output = []
    for port in serial.tools.list_ports.comports():
        output.append(port.name)
