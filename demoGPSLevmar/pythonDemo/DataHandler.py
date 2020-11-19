from collections import namedtuple
import serial
MeasData = namedtuple("MeasData", "rawPosi rawPosj rawPosk rawMx rawMy rawMz rawMAvail rawAx rawAy rawAz rawAAvail rawWx rawWy rawWz rawWAvail")
class DataHandler:
    dataBuffer = b''
    serialPort = None
    buff=bytearray()
    def __init__(self, port, baudrate):
        self.baudrate = baudrate
        try:
            serialPort = serial.Serial(port, baud, timeout=0)
        except:

    def readFromPort(ser):
        connected = True
        retval = None
        while serialPort.available():
            # time.sleep(0.1)
            #reading = serial_port.read().decode('ascii')
            reading = serialPort.read()
            # if(reading!=b''):
            #      print(reading)
            if(reading == b'<'):
                buff=bytearray()
            elif(reading==b'>'):
                retval = moveStreamToStruct(buff)
                buff=bytearray()
                return retval
            else:
                buff+=reading
        return retval
    def moveStreamToStruct(buff):
        s=data.decode('ascii')
        dataList = list(map(float,s.split(',')))
        return MeasData(*dataList)
