#include <android/Sensor.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

#include "SensorEvent.h"
#include "NativeLogger.h"

/**
 * Implementation of class member functions for class SensorEvent
 */

/*
SensorEvent::SensorEvent(){
	isAccLoaded=false;
	isGyrLoaded=false;
	isMagLoaded=false;
	isBarLoaded=false;
	barVal=0;
	barTimestamp=0;
}
*/

SensorEvent::SensorEvent(){
	bAccLoaded                = false;
	bGyrLoaded                = false;
	bMagLoaded                = false;
	bBarLoaded                = false;
	eventData[0].barVal       = 0;
	eventData[0].bartimestamp = 0; // first index
}

SensorEvent::~SensorEvent(){}

void SensorEvent::updateAccelerometer(ASensorEvent sEvent){
	eventData[0].acctimestamp = sEvent.timestamp;
	eventData[0].accX         = sEvent.acceleration.x;
	eventData[0].accY         = sEvent.acceleration.y;
	eventData[0].accZ         = sEvent.acceleration.z;
	bAccLoaded                = true;
}

void SensorEvent::updateGyroscope(ASensorEvent sEvent){
	eventData[0].gyrtimestamp = sEvent.timestamp;
	eventData[0].gyrX         = sEvent.vector.x;
	eventData[0].gyrY         = sEvent.vector.y;
	eventData[0].gyrZ         = sEvent.vector.z;
	bGyrLoaded                = true;
}
void SensorEvent::updateMagnetometer(ASensorEvent sEvent){
	eventData[0].magtimestamp = sEvent.timestamp;
	eventData[0].magX         = sEvent.magnetic.x;
	eventData[0].magY         = sEvent.magnetic.y;
	eventData[0].magZ         = sEvent.magnetic.z;
	bMagLoaded                = true;
}
void SensorEvent::updateBarometer(ASensorEvent sEvent){
	eventData[0].bartimestamp = sEvent.timestamp;
	eventData[0].barVal       = sEvent.pressure;
	bBarLoaded                = true;
}

bool SensorEvent::isUpdateComplete(){
	return (bAccLoaded && bGyrLoaded && bMagLoaded) || (bAccLoaded && bGyrLoaded && bMagLoaded && bBarLoaded);
}

bool SensorEvent::isAccLoaded(){
	return bAccLoaded;
}
bool SensorEvent::isGyrLoaded(){
	return bGyrLoaded;
}
bool SensorEvent::isMagLoaded(){
	return bMagLoaded;
}
bool SensorEvent::isBarLoaded(){
	return bBarLoaded;
}

sensor_data SensorEvent::getSensorData(){
	return eventData[0];
}

std::string SensorEvent::accToString(){
	std::stringstream accString;
	accString<<eventData[0].acctimestamp;
	accString<<",";
	accString<<eventData[0].accX;
	accString<<",";
	accString<<eventData[0].accY;
	accString<<",";
	accString<<eventData[0].accZ;
	return accString.str();
}
std::string SensorEvent::gyrToString(){
	std::stringstream gyrString;
	gyrString<<eventData[0].gyrtimestamp;
	gyrString<<",";
	gyrString<<eventData[0].gyrX;
	gyrString<<",";
	gyrString<<eventData[0].gyrY;
	gyrString<<",";
	gyrString<<eventData[0].gyrZ;
	return gyrString.str();
}
std::string SensorEvent::magToString(){
	std::stringstream magString;
	magString<<eventData[0].magtimestamp;
	magString<<",";
	magString<<eventData[0].magX;
	magString<<",";
	magString<<eventData[0].magY;
	magString<<",";
	magString<<eventData[0].magZ;
	return magString.str();
}
std::string SensorEvent::barToString(){
	std::stringstream barString;
	barString<<eventData[0].bartimestamp;
	barString<<",";
	barString<<eventData[0].barVal;
	return barString.str();
}


/*
void SensorEvent::updateAccelerometer(ASensorEvent sEvent){
	accTimestamp=sEvent.timestamp;
	accX=sEvent.acceleration.x;
	accY=sEvent.acceleration.y;
	accZ=sEvent.acceleration.z;
	isAccLoaded=true;
}
void SensorEvent::updateGyroscope(ASensorEvent sEvent){
	gyrTimestamp=sEvent.timestamp;
	gyrX=sEvent.vector.x;
	gyrY=sEvent.vector.y;
	gyrZ=sEvent.vector.z;
	isGyrLoaded=true;
}
void SensorEvent::updateMagnetometer(ASensorEvent sEvent){
	magTimestamp=sEvent.timestamp;
	magX=sEvent.magnetic.x;
	magY=sEvent.magnetic.y;
	magZ=sEvent.magnetic.z;
	isMagLoaded=true;
}
void SensorEvent::updateBarometer(ASensorEvent sEvent){
	barTimestamp=sEvent.timestamp;
	barVal=sEvent.pressure;
	isBarLoaded=true;
}

std::string SensorEvent::accToString(){
	std::stringstream accString;
	accString<<accTimestamp;
	accString<<",";
	accString<<accX;
	accString<<",";
	accString<<accY;
	accString<<",";
	accString<<accZ;
	return accString.str();
}
std::string SensorEvent::gyrToString(){
	std::stringstream gyrString;
	gyrString<<gyrTimestamp;
	gyrString<<",";
	gyrString<<gyrX;
	gyrString<<",";
	gyrString<<gyrY;
	gyrString<<",";
	gyrString<<gyrZ;
	return gyrString.str();
}
std::string SensorEvent::magToString(){
	std::stringstream magString;
	magString<<magTimestamp;
	magString<<",";
	magString<<magX;
	magString<<",";
	magString<<magY;
	magString<<",";
	magString<<magZ;
	return magString.str();
}
std::string SensorEvent::barToString(){
	std::stringstream barString;
	barString<<barTimestamp;
	barString<<",";
	barString<<barVal;
	return barString.str();
}

*/








