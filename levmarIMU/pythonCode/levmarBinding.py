from ctypes import *
import os

os.chdir("/home/edo/SIPA/projects/levmar/levmarIMU/pythonCode")
coreDll = CDLL("/home/edo/SIPA/projects/levmar/levmarIMU/cCode/liblevmar.so", mode=RTLD_GLOBAL)
wrapperDll = cdll.LoadLibrary("/home/edo/SIPA/projects/levmar/levmarIMU/cCode/build/libLevmarIMUWrapper.so")


# double* fitAccelerometer(double * noisyMeasuredAcc, int n, double * retval);
#notes : retval -> 9 element array, n number of measurement
def fitAccelerometer(inputList, numOfMeasurement):
    outputDataArray = (c_double * 9)()
    # cast(outputDataArray, POINTER(c_double))
    inputArray = (c_double * len(inputList))(*inputList)
    # inputArrayPointer = cast(inputArray, POINTER(c_double))

    wrapperDll.fitAccelerometer(inputArray, numOfMeasurement, outputDataArray)
    outputDataList = [outputDataArray[i] for i in range(9)]
    return outputDataArray
