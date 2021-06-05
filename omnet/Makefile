all: Example1Communication.o MysStatus.o RepositoryMySQL.o CommunicationSocket.o UAV.o StatusModule.o
	g++ -pthread -o mysterio/main Example1Communication.o MysStatus.o RepositoryMySQL.o CommunicationSocket.o UAV.o StatusModule.o -lmysqlclient

CommunicationSocket.o: communication/CommunicationSocket.cc communication/CommunicationSocket.h communication/ConnSocket.cc
	g++ -c communication/CommunicationSocket.cc communication/CommunicationSocket.h communication/ConnSocket.cc
SocketConnect.o: communication/SocketConnect.cc
	g++ -c communication/SocketConnect.cc
Example1Communication.o: mysterio/Example1Communication.cc communication/CommunicationSocket.h status/MysStatus.h
	g++ -c mysterio/Example1Communication.cc communication/CommunicationSocket.h communication/CommunicationSocket.cc status/MysStatus.h 
MysStatus.o:	status/MysStatus.cc status/MysStatus.h ../src/status/Status.h
	g++ -c status/MysStatus.cc status/MysStatus.h ../src/status/Status.h
RepositoryMySQL.o: database/RepositoryMySQL.cc database/RepositoryMySQL.h
	g++ -c database/RepositoryMySQL.cc database/RepositoryMySQL.h
UAV.o: ../src/utils/UAV.h ../src/utils/UAV.cc
	g++ -c ../src/utils/UAV.h ../src/utils/UAV.cc
Coordinate.o:  ../src/utils/Coordinate.h
	g++ -c ../src/utils/Coordinate.h
StatusModule.o: common/StatusModule.h common/StatusModule.cc
	g++ -c common/StatusModule.h common/StatusModule.cc
clean:
	rm -rf *.o

vai:
	./mysterio/main