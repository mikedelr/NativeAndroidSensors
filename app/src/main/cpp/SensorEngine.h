#ifndef SENSORENGINE_H
#define SENSORENGINE_H

#include <android/sensor.h>

#include "NativeLogger.h"

/**
 * SensorEngine class provides a global reference to the SensorManager
 */


// consider adding code to make this class singleton
class SensorEngine{
	protected:
		void initAccelerometer(int samplingRate,ALooper_callbackFunc get_sensor_events);
		void initGyroscope(int samplingRate,ALooper_callbackFunc get_sensor_events);
		void initMagnetometer(int samplingRate,ALooper_callbackFunc get_sensor_events);
		void initBarometer(int samplingRate,ALooper_callbackFunc get_sensor_events);
		int LOOPER_ID_USER;
	public:
		ALooper* looper;
		ASensorManager* sensorManager; // this is a singleton
		const ASensor* accelerometerSensor;
		ASensorEventQueue* sensorEventQueue;
		ASensorEventQueue* sensorEventQueueAcc;
		const ASensor* gyroscopeSensor;
		ASensorEventQueue* sensorEventQueueGyr;
		const ASensor* magnetometerSensor;
		ASensorEventQueue* sensorEventQueueMag;
		const ASensor* barometricSensor;
		ASensorEventQueue* sensorEventQueueBar;
		SensorEngine();
		void initialiseSensors(int samplingrate, ALooper_callbackFunc get_sensor_events);
		void shutDownSensors();
};



#endif
