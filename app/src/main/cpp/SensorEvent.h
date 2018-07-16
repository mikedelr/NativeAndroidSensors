#ifndef SENSOREVENT_H
#define SENSOREVENT_H

#include <android/sensor.h>
#include <iostream>
/*
 * SensorEvent class will copy all relevent sensor event data to another
 */

typedef struct sensorevent
{
	int64_t acctimestamp;
	double accX;
	double accY;
	double accZ;
	int64_t gyrtimestamp;
	double gyrX;
	double gyrY;
	double gyrZ;
	int64_t magtimestamp;
	double magX;
	double magY;
	double magZ;
	int64_t bartimestamp;
	double barVal;
} sensor_data;

class SensorEvent{
	private:
		bool bAccLoaded;
		bool bGyrLoaded;
		bool bMagLoaded;
		bool bBarLoaded;
		sensor_data eventData[1]; // size 1
	public:
		SensorEvent();
		~SensorEvent();
		void updateAccelerometer(ASensorEvent sEvent);
		void updateGyroscope(ASensorEvent sEvent);
		void updateMagnetometer(ASensorEvent sEvent);
		void updateBarometer(ASensorEvent sEvent);
		sensor_data getSensorData();
		std::string accToString();
		std::string gyrToString();
		std::string magToString();
		std::string barToString();
		bool isUpdateComplete();
		bool isAccLoaded();
		bool isGyrLoaded();
		bool isMagLoaded();
		bool isBarLoaded();
};

#endif

//		double accX;
//		double accY;
//		double accZ;
//		int64_t accTimestamp;
//		double gyrX;
//		double gyrY;
//		double gyrZ;
//		int64_t gyrTimestamp;
//		double magX;
//		double magY;
//		double magZ;
//		int64_t magTimestamp;
//		double barVal;
//		int64_t barTimestamp;

