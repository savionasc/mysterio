default: go

all: MysterioCommander.o MysStatusManager.o RepositoryBase.o MysCommunication.o UAV.o MissionPlanner.o
	g++ -pthread -o scenarios/MysterioCommander MysterioCommander.o MysStatusManager.o RepositoryBase.o MysCommunication.o UAV.o MissionPlanner.o -lmysqlclient

MysCommunication.o: communication/MysCommunication.cc communication/MysCommunication.h communication/UAVRegistry.cc
	g++ -c communication/MysCommunication.cc communication/MysCommunication.h communication/UAVRegistry.cc
SocketConnect.o: communication/SocketConnect.cc
	g++ -c communication/SocketConnect.cc
MysterioCommander.o: scenarios/MysterioCommander.cc communication/MysCommunication.h status/MysStatusManager.h
	g++ -c scenarios/MysterioCommander.cc communication/MysCommunication.h communication/MysCommunication.cc status/MysStatusManager.h 
MysStatusManager.o: status/MysStatusManager.cc status/MysStatusManager.h ../src/status/StatusManager.h
	g++ -c status/MysStatusManager.cc status/MysStatusManager.h ../src/status/StatusManager.h
MissionPlanner.o: ../src/mission/MissionPlanner.cc ../src/mission/MissionPlanner.h
	g++ -c ../src/mission/MissionPlanner.cc ../src/mission/MissionPlanner.h
RepositoryBase.o: ../src/database/RepositoryBase.cc ../src/database/RepositoryBase.h ../src/utils/Coordinate.h
	g++ -c ../src/database/RepositoryBase.cc ../src/database/RepositoryBase.h ../src/utils/Coordinate.h
UAV.o: ../src/utils/UAV.h ../src/utils/UAV.cc
	g++ -c ../src/utils/UAV.h ../src/utils/UAV.cc
Coordinate.o:  ../src/utils/Coordinate.h
	g++ -c ../src/utils/Coordinate.h
UAVStatus.o: ../src/status/UAVStatus.h
	g++ -c ../src/status/UAVStatus.h
clean:
	rm -rf *.o scenarios/MysterioCommander

run:
	./scenarios/MysterioCommander

go:
	make clean --no-print-directory && make all --no-print-directory && clear && make run --no-print-directory
