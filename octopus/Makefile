all: Example1Communication.o MysStatusManager.o RepositoryMySQL.o MysCommunication.o UAV.o MissionPlanner.o
	g++ -pthread -o scenarios/main Example1Communication.o MysStatusManager.o RepositoryMySQL.o MysCommunication.o UAV.o MissionPlanner.o -lmysqlclient

MysCommunication.o: communication/MysCommunication.cc communication/MysCommunication.h communication/UAVRegistry.cc
	g++ -c communication/MysCommunication.cc communication/MysCommunication.h communication/UAVRegistry.cc
SocketConnect.o: communication/SocketConnect.cc
	g++ -c communication/SocketConnect.cc
Example1Communication.o: scenarios/Example1Communication.cc communication/MysCommunication.h status/MysStatusManager.h
	g++ -c scenarios/Example1Communication.cc communication/MysCommunication.h communication/MysCommunication.cc status/MysStatusManager.h 
MysStatusManager.o: status/MysStatusManager.cc status/MysStatusManager.h ../src/status/StatusManager.h
	g++ -c status/MysStatusManager.cc status/MysStatusManager.h ../src/status/StatusManager.h
MissionPlanner.o: ../src/mission/MissionPlanner.cc ../src/mission/MissionPlanner.h
	g++ -c ../src/mission/MissionPlanner.cc ../src/mission/MissionPlanner.h
RepositoryMySQL.o: database/RepositoryMySQL.cc database/RepositoryMySQL.h ../src/utils/Coordinate.h
	g++ -c database/RepositoryMySQL.cc database/RepositoryMySQL.h ../src/utils/Coordinate.h
UAV.o: ../src/utils/UAV.h ../src/utils/UAV.cc
	g++ -c ../src/utils/UAV.h ../src/utils/UAV.cc
Coordinate.o:  ../src/utils/Coordinate.h
	g++ -c ../src/utils/Coordinate.h
UAVStatus.o: ../src/status/UAVStatus.h
	g++ -c ../src/status/UAVStatus.h
clean:
	rm -rf *.o scenarios/main

run:
	./scenarios/main