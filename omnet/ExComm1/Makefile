all: Example1Communication.o StatusC1.o Comm.o UAV.o Coordinate.o main.o
	g++ -o main main.o Example1Communication.o StatusC1.o Comm.o UAV.o Coordinate.o

main.o: main.cc communication/Comm.h
	g++ -c main.cc communication/Comm.h
Comm.o: communication/Comm.cc communication/Comm.h
	g++ -c communication/Comm.cc communication/Comm.h
Example1Communication.o: mysterio/Example1Communication.cc communication/Comm.h status/StatusC1.h
	g++ -c mysterio/Example1Communication.cc communication/Comm.h communication/Comm.cc status/StatusC1.h 
StatusC1.o:	status/StatusC1.cc status/StatusC1.h ../../src/status/Status.h
	g++ -c status/StatusC1.cc status/StatusC1.h ../../src/status/Status.h
UAV.o: ../../src/utils/UAV.h
	g++ -c ../../src/utils/UAV.cc ../../src/utils/UAV.h
Coordinate.o: ../../src/utils/Coordinate.cc ../../src/utils/Coordinate.h
	g++ -c ../../src/utils/Coordinate.cc ../../src/utils/Coordinate.h

clean:
	rm -rf *.o

vai:
	./main