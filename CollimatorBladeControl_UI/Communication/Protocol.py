import struct
from crc import Calculator, Crc8
from enum import Enum

HEADER_SIZE = 2

crc_calculator = Calculator(Crc8.CCITT)


class HeaderId(Enum):
    COMMAND_e = 0x01
    DATA_TRANSFER_e = 0x02
    TRANSFER_ERR_e = 0xFE
    HELLO_MSG_e = 0xFF
    NOT_VALID_e = 5


def construct_message(header_id: Enum, payload_data: bytes, crc=True):
    id_val = header_id.value
    msg_size = HEADER_SIZE + len(payload_data)
    return_bytes = bytearray(struct.pack('<%dB' % msg_size, id_val, msg_size + int(crc), *payload_data))
    if crc is True:
        return_bytes.append(crc_calculator.checksum(return_bytes))
    return return_bytes


if __name__ == '__main__':
    payload = [0x10, 0x20, 0x30, 0x40]
    out = construct_message(HeaderId.COMMAND_e, payload, crc=True).hex(' ')
    print(out)
