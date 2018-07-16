#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <stdlib.h>

#include "NativeLogger.h"
#include "SensorEventWriter.h"
#include "SensorEvent.h"

/**
 * Default Constructor
 */
SensorEventWriter::SensorEventWriter(){
}

/**
 * Constructor w/one input argument, string specifying outputDirectory
 */
SensorEventWriter::SensorEventWriter(std::string externalStorageDir){
	writeDir=externalStorageDir;
	//get current time
	std::stringstream sstreamTime;
	sstreamTime << unixTimeStampSeconds();
	std::string sTime = sstreamTime.str(); // bug with IDE this works
	std::string fileName = sTime + std::string("_nativeSensorData.dat");
	std::string fileFullPath = externalStorageDir + fileName;
	pathToFile = fileFullPath;
}

/**
 * Constructor w/two input arguments, string specifying outputDirectory and int specifying sampling frequency
 */
SensorEventWriter::SensorEventWriter(std::string externalStorageDir, int sampFreqHz){
	writeDir=externalStorageDir;
	//get current time
	std::stringstream sstreamTime;
	sstreamTime << unixTimeStampSeconds();
	std::string sTime = sstreamTime.str(); // bug with IDE this works

	std::stringstream strstreamHertz;
	strstreamHertz << sampFreqHz;

	std::string fileName = sTime + std::string("_nativeSensorData_")+ strstreamHertz.str() +std::string("hertz.dat");
	std::string fileFullPath = externalStorageDir + fileName;
	pathToFile = fileFullPath;
}

SensorEventWriter::~SensorEventWriter(){
}

/**
 * http://stackoverflow.com/questions/11765301/how-do-i-get-the-unix-timestamp-in-c-as-an-int
 * http://stackoverflow.com/questions/6360210/androidlinux-uptime-using-clock-monotonic
 */

int64_t SensorEventWriter::unixTimeStampSeconds(){
    struct timespec t; //seconds and nanoseconds
    clock_gettime(CLOCK_BOOTTIME, &t); // Time since boot, available as of ndk-9c
//    LOGI("CLOCK_BOOTTIME_tv_sec, %lld", (int64_t)(t.tv_sec));
//    LOGI("CLOCK_BOOTTIME_tv_usec, %ld", t.tv_nsec);
    int64_t unixTime = (int64_t)time(NULL); //equivalent to System.currentTimeMillis
	return unixTime; //unix timestamp
}

void SensorEventWriter::writeSensorEvent(SensorEvent sensorevent){
	sensor_data eventData[1];
	eventData[0] = sensorevent.getSensorData();
	std::ofstream ofs(pathToFile.c_str(), std::ios_base::app | std::ios_base::binary ); // must be char* IN BINARY MODE
	// ====================================================
	/*                  ORIGINAL DO NOT DELETE           */
	ofs.write((char*)(&eventData),sizeof(eventData));
	//=====================================================
	// try to write to binary same as java way, may need to check for endianness on values since java
	//

	//=====================================================
	/* ALTERNATIVE IMPLEMENTATION - INCOMPLETE, needs little endian to big endian flip */
	//=====================================================
	/*
	ofs.write((char*)(&eventData[0].acctimestamp),sizeof(int64_t));
	ofs.write((char*)(&eventData[0].accX),sizeof(double));
	ofs.write((char*)(&eventData[0].accY),sizeof(double));
	ofs.write((char*)(&eventData[0].accZ),sizeof(double));
	ofs.write((char*)(&eventData[0].gyrtimestamp),sizeof(int64_t));
	ofs.write((char*)(&eventData[0].gyrX),sizeof(double));
	ofs.write((char*)(&eventData[0].gyrY),sizeof(double));
	ofs.write((char*)(&eventData[0].gyrZ),sizeof(double));
	ofs.write((char*)(&eventData[0].bartimestamp),sizeof(int64_t));
	ofs.write((char*)(&eventData[0].barVal),sizeof(double));
	ofs.write((char*)(&eventData[0].magtimestamp),sizeof(int64_t));
	ofs.write((char*)(&eventData[0].magX),sizeof(double));
	ofs.write((char*)(&eventData[0].magY),sizeof(double));
	ofs.write((char*)(&eventData[0].magZ),sizeof(double));
	//=====================================================
	*/
	ofs.close();
}

/*
void SensorEventWriter::writeSensorEvent(SensorEvent sensorevent){
	std::ofstream ofs;
	ofs.open(pathToFile.c_str(), std::ios_base::app | std::ios_base::binary ); // must be char* IN BINARY MODE
	ofs<<sensorevent.accToString();
	ofs<<",";
	ofs<<sensorevent.gyrToString();
	ofs<<",";
	ofs<<sensorevent.magToString();
	ofs<<",";
	ofs<<sensorevent.barToString();
	ofs<<"\n";
	ofs.close();
}
*/
void SensorEventWriter::close(){
}
