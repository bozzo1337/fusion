all: thermalCam

thermalCam: thermalCam.o SparkFun_GridEYE_Arduino_Library.o
	g++ -lwiringPi -o thermalCam thermalCam.o SparkFun_GridEYE_Arduino_Library.o

thermalCam.o: thermalCam.cpp
	g++ -lwiringPi -c thermalCam.cpp 

SparkFun_GridEYE_Arduino_Library.o: SparkFun_GridEYE_Arduino_Library.cpp SparkFun_GridEYE_Arduino_Library.h
	g++ -lwiringPi -c SparkFun_GridEYE_Arduino_Library.cpp

clean:
	rm -f thermalCam *.o *~