#ifndef MYSTERIO_SRC_FACADE_MYSTERIOFACADE_H_
#define MYSTERIO_SRC_FACADE_MYSTERIOFACADE_H_
#include <vector>
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include "../mission/Command.h"
#include "../mission/Task.h"

class MysterioFacade {
public:
	//Missions and Tasks
	virtual void createMission(std::vector<Task> taskList) = 0;
	virtual void addTask(Task task) = 0;
	virtual void assignTask() = 0;
	virtual void sendCommand(Command command) = 0;
	virtual void goToPosition(UAV uav, Coordinate coord) = 0;
	virtual bool goToNextTask() = 0;
	virtual void startMission() = 0;

	// Status
	virtual Coordinate getUAVLocationStatus() = 0;
	virtual double getUAVVelocityStatus() = 0;
	virtual int getUAVFlightTimeStatus() = 0;
	virtual float getUAVBatteryStatus() = 0;

	// Communication
	virtual bool isAvailableToCommunicate(int idUAV) = 0;
	virtual bool setUAVConnectionsLimit(int numberOfUAVs) = 0;
	virtual changeCommunicationPort(int port) = 0;

	// Database
	virtual void setDatabase(char database[]) = 0;
	virtual void setDatabaseHostname(char database[]) = 0;
	virtual void setDatabaseUsername(char database[]) = 0;
	virtual void setDatabasePassword(char database[]) = 0;
	virtual void enableAutosaveStatusData(bool save) = 0;
	virtual std::vector<Coordinate> getDatabaseStatusLocaltion(int idUAV) = 0;
	virtual std::vector<double> getDatabaseStatusLocaltion(int idUAV) = 0;
	virtual std::vector<int> getDatabaseStatusLocaltion(int idUAV) = 0;
	virtual std::vector<float> getDatabaseStatusLocaltion(int idUAV) = 0;
};

#endif