#ifndef SENSOREVENTWRITER_H
#define SENSOREVENTWRITER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "SensorEvent.h"

class SensorEventWriter{
	private:
		int64_t unixTimeStampSeconds();
		std::string writeDir;   // external storage directory - ROOT directory
		std::string pathToFile; // full path to file that is written to by the sensor eventwriter
	public:
		SensorEventWriter();
		SensorEventWriter(std::string rootExternalStorage);
		SensorEventWriter(std::string rootExternalStorage, int sampFreqHz);
		~SensorEventWriter();
		void writeSensorEvent(SensorEvent sensorevent);
		void close();
};


#endif
