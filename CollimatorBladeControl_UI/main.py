import time

import serial
from Communication.Protocol import *
com_device = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)


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
        x = str_to_bytearray(input())

        x = construct_message(HeaderId.HELLO_MSG_e, x, crc=False)
        print(x.hex(' '), end="\n=======================================================\n")
        com_device.write(x)
