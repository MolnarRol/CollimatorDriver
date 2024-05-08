import struct
from crc import Calculator, Crc8
from enum import Enum
from dataclasses import dataclass

HEADER_SIZE = 2

crc_calculator = Calculator(Crc8.CCITT)


class HeaderId(Enum):
    COMMAND_e = 0x01
    COMMAND_RES_e = 0x81
    DATA_TRANSFER_e = 0x02
    TRANSFER_ERR_e = 0xFE
    HELLO_MSG_e = 0xFF
    NOT_VALID_e = 5


GET_MOVEMENT_PARAMETERS_e = 0x0
SET_MOVEMENT_PARAMETERS_e = 0x1
SET_REFERENCE_POSITION_e = 0x2
GET_MECHANICAL_DATA_e = 0x3
GET_ELECTRICAL_DATA_e = 0x4
GET_MAXIMUM_POSITION_e = 0x5
RESET_ERROR_FLAGS_e = 0x6
SET_MOVEMENT_STATE_e = 0x7

@dataclass
class Message:
    header_id: HeaderId
    size: int
    payload: tuple
    crc: int

def construct_message(header_id: Enum, payload_data: bytes, crc=True):
    id_val = header_id.value
    msg_size = HEADER_SIZE + len(payload_data)
    return_bytes = bytearray(struct.pack('<%dB' % msg_size, id_val, msg_size + int(crc), *payload_data))
    if crc is True:
        return_bytes.append(crc_calculator.checksum(return_bytes))
    return return_bytes


def deconstruct_message(data_bytes: bytes):
    loc_msg = Message
    loc_msg.header_id = data_bytes[0]
    loc_msg.size = data_bytes[1]
    loc_msg.payload = data_bytes[2:len(data_bytes) - 1]
    loc_msg.crc = data_bytes[len(data_bytes) - 1]
    return loc_msg

if __name__ == '__main__':
    payload = bytearray([1, 2, 3, 4])
    print(crc_calculator.checksum(payload))
