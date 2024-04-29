import serial
import threading
import queue
import time


class SerialInterface:
    serial_transaction_timeout = 0.05

    def __init__(self, serial_port: str, serial_baud: int):
        self.serial_port = serial_port
        self.serial_baud = serial_baud
        self.serial_if = serial.Serial()
        self.serial_if.timeout = self.serial_transaction_timeout

        self.transaction_queue = queue.PriorityQueue(maxsize=10)
        self.transaction_thread = threading.Thread(target=self.__interface_handler_thread__, daemon=True)
        self.transaction_thread.start()


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

    def new_transaction(self, data: list, priority=0, callback=None):
        if (len(data) == 0) or self.transaction_queue.full():
            return False
        self.transaction_queue.put((priority, {'data': data, 'callback': callback}))
        return True

    def __interface_handler_thread__(self):
        while True:
            if self.transaction_queue.qsize() != 0:
                (prio, transaction) = self.transaction_queue.get()
                self.serial_if.write(transaction['data'])
                response = self.serial_if.read(256)
                if transaction['callback'] is not None:
                    transaction['callback'](response)
            time.sleep(0.01)


if __name__ == '__main__':
    q = queue.PriorityQueue(maxsize=10)
    q.put((1, '1'))
    q.put((0, '0'))
    print(q.get())
    print(q.get())
    pass