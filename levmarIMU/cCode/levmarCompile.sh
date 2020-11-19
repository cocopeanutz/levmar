gcc -c -o build/levmarIMUWrapper.o -Wall -fpic levmarIMUWrapper.c
# gcc -shared -o build/libLevmarIMUWrapper.so build/levmarIMUWrapper.o
gcc -shared -o build/libLevmarIMUWrapper.so build/levmarIMUWrapper.o


gcc -static -o build/levmar demolevmar.c levmarIMUWrapper.c -L. -llevmar -lm
