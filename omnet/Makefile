all: Example1Communication.o MysStatus.o RepositoryMySQL.o CommunicationSocket.o UAV.o DroneStatus.o Singleton.o
	g++ -pthread -o scenarios/main Example1Communication.o MysStatus.o RepositoryMySQL.o CommunicationSocket.o UAV.o DroneStatus.o Singleton.o -lmysqlclient

CommunicationSocket.o: communication/CommunicationSocket.cc communication/CommunicationSocket.h communication/ConnServerSocket.cc
	g++ -c communication/CommunicationSocket.cc communication/CommunicationSocket.h communication/ConnServerSocket.cc
SocketConnect.o: communication/SocketConnect.cc
	g++ -c communication/SocketConnect.cc
Example1Communication.o: scenarios/Example1Communication.cc communication/CommunicationSocket.h status/MysStatus.h
	g++ -c scenarios/Example1Communication.cc communication/CommunicationSocket.h communication/CommunicationSocket.cc status/MysStatus.h 
MysStatus.o: status/MysStatus.cc status/MysStatus.h ../src/status/Status.h
	g++ -c status/MysStatus.cc status/MysStatus.h ../src/status/Status.h
Singleton.o: Singleton.cc Singleton.h
	g++ -c Singleton.cc Singleton.h
RepositoryMySQL.o: database/RepositoryMySQL.cc database/RepositoryMySQL.h ../src/utils/Coordinate.h
	g++ -c database/RepositoryMySQL.cc database/RepositoryMySQL.h ../src/utils/Coordinate.h
UAV.o: ../src/utils/UAV.h ../src/utils/UAV.cc
	g++ -c ../src/utils/UAV.h ../src/utils/UAV.cc
Coordinate.o:  ../src/utils/Coordinate.h
	g++ -c ../src/utils/Coordinate.h
DroneStatus.o: status/DroneStatus.h status/DroneStatus.cc
	g++ -c status/DroneStatus.h status/DroneStatus.cc
clean:
	rm -rf *.o scenarios/main

run:
	./scenarios/main