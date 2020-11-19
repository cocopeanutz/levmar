from DataHandler import DataHandler
import time
import levmarBinding



def dataAccumulator(dataHandler):
    start = time.time()
    dataAverage = None
    numOfData = 0
    while True:
        measurementData = dataHandler.readFromPort()
        if measurementData != None :
            acc = [measurementData.rawAx, measurementData.rawAy, measurementData.rawAz]
            numOfData = numOfData + 1
            if dataAverage == None:
                dataAverage = acc
            else:
                for x in range(3):
                    dataAverage[x] = dataAverage[x] + (acc[x]-dataAverage[x])/numOfData
            print(dataAverage)
        curtime = time.time()
        print('iteration ' + str(numOfData))
        if curtime-start > 4:
            return dataAverage

dataHandler = DataHandler('/dev/ttyACM0', 115200)
# print('Start Accumulating Data:')
# data = dataAccumulator(dataHandler)
# print(data)
#
# levmarBinding.fitAccelerometer(data, 1)


dataAppendList = list()
for x in range(27):
    input()
    dataHandler.flush()
    data = dataAccumulator(dataHandler)
    dataAppendList.extend(data)
    print(data)
    print (x)
    print('next')
levmarBinding.fitAccelerometer(dataAppendList, 27)

# while(True):
#     DataHandler
