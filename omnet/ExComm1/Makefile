all: main.o Loko.o
	g++ -o main main.o Loko.o

Loko.o:	communication/Loko.cc communication/Loko.h ../../src/communication/Communicable.h
	g++ -c main.cc communication/Loko.cc ../../src/communication/Communicable.h

main.o:	main.cc
	g++ -c main.cc

clean:
	rm -rf *.o

vai:
	./Comm