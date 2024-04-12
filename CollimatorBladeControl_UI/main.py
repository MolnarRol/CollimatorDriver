import time

import serial
import struct
from Communication.Protocol import *
com_device = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.05)


def str_to_bytearray(s):
    nums = []
    for num in s.split():
        if num.startswith("0x"):
            nums.append(int(num, 16))
        else:
            nums.append(int(num))
    return bytearray(nums)


if __name__ == '__main__':
    if com_device.is_open:
        com_device.close()
    com_device.open()

    # while True:
    #     x = com_device.read(1024)
    #     print(x)
    #     # time.sleep(0.5)

    while True:
        print("Numbers to send: ", end="")
        # x = str_to_bytearray(input())
        b = int(input())
        msg_payload = struct.pack('>BI', 0, b)

        x = construct_message(HeaderId.COMMAND_e, msg_payload)
        # print(x.hex(' '), end="\n=======================================================\n")
        com_device.write(x)
        y = com_device.read(256)
        print(y.hex(' '))
        # time.sleep(0.25)
