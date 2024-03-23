import serial

com_device = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.01)


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
    x = []
    for i in range(510):
        x.append(i % 256)

    com_device.write(bytearray(x))
