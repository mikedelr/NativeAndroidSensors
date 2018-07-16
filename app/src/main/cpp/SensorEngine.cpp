#include <android/Sensor.h>

#include "SensorEngine.h"
#include "NativeLogger.h"

SensorEngine::SensorEngine() {
	looper = NULL;
	sensorManager = NULL;
	accelerometerSensor = NULL;
	sensorEventQueueAcc = NULL;
	gyroscopeSensor = NULL;
	sensorEventQueueGyr = NULL;
	magnetometerSensor = NULL;
	sensorEventQueueMag = NULL;
	barometricSensor = NULL;
	sensorEventQueueBar = NULL;
}

void SensorEngine::initialiseSensors(int samplingRate, ALooper_callbackFunc get_sensor_events){
	LOOPER_ID_USER=3;
	looper = ALooper_forThread();
	if(looper == NULL) {
		looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	}

	initAccelerometer(samplingRate,get_sensor_events);
	initGyroscope(samplingRate,get_sensor_events);
	initMagnetometer(samplingRate,get_sensor_events);
	initBarometer(samplingRate,get_sensor_events);
}

void SensorEngine::initAccelerometer(int samplingRate,ALooper_callbackFunc get_sensor_events){
	sensorManager = ASensorManager_getInstance();
	accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,ASENSOR_TYPE_ACCELEROMETER);
	sensorEventQueueAcc = ASensorManager_createEventQueue(sensorManager,looper, LOOPER_ID_USER, get_sensor_events, NULL);
	ASensorEventQueue_enableSensor(sensorEventQueueAcc,accelerometerSensor);
	ASensorEventQueue_setEventRate(sensorEventQueueAcc,accelerometerSensor, (1000L/samplingRate)*1000); //1sample/second
}

void SensorEngine::initGyroscope(int samplingRate,ALooper_callbackFunc get_sensor_events){
	sensorManager = ASensorManager_getInstance();
	gyroscopeSensor = ASensorManager_getDefaultSensor(sensorManager,ASENSOR_TYPE_GYROSCOPE);
	sensorEventQueueGyr = ASensorManager_createEventQueue(sensorManager,looper, LOOPER_ID_USER, get_sensor_events,NULL);
	ASensorEventQueue_enableSensor(sensorEventQueueGyr,gyroscopeSensor);
	ASensorEventQueue_setEventRate(sensorEventQueueGyr,gyroscopeSensor, (1000L/samplingRate)*1000);
}

void SensorEngine::initMagnetometer(int samplingRate,ALooper_callbackFunc get_sensor_events){
	sensorManager = ASensorManager_getInstance();
	magnetometerSensor = ASensorManager_getDefaultSensor(sensorManager,ASENSOR_TYPE_MAGNETIC_FIELD);
	sensorEventQueueMag = ASensorManager_createEventQueue(sensorManager,looper, LOOPER_ID_USER, get_sensor_events,NULL);
	ASensorEventQueue_enableSensor(sensorEventQueueMag,magnetometerSensor);
	ASensorEventQueue_setEventRate(sensorEventQueueMag,magnetometerSensor, (1000L/samplingRate)*1000);
}

void SensorEngine::initBarometer(int samplingRate,ALooper_callbackFunc get_sensor_events){
	sensorManager = ASensorManager_getInstance();
	barometricSensor = ASensorManager_getDefaultSensor(sensorManager,6);
	sensorEventQueueBar = ASensorManager_createEventQueue(sensorManager,looper, LOOPER_ID_USER, get_sensor_events,NULL);
	ASensorEventQueue_enableSensor(sensorEventQueueBar,barometricSensor);
	ASensorEventQueue_setEventRate(sensorEventQueueBar,barometricSensor, (1000L/samplingRate)*1000);
}

void SensorEngine::shutDownSensors(){
	if (accelerometerSensor != NULL)	{
        ASensorEventQueue_disableSensor(sensorEventQueueAcc,accelerometerSensor);
        LOGD("Free Accelerometer");
    }

    if (gyroscopeSensor != NULL){
        ASensorEventQueue_disableSensor(sensorEventQueueGyr,gyroscopeSensor);
        LOGD("Free Gyroscope");
    }
    if (magnetometerSensor != NULL){
    	ASensorEventQueue_disableSensor(sensorEventQueueMag,magnetometerSensor);
    	LOGD("Free Magnetometer");
    }
    if (barometricSensor != NULL){
    	ASensorEventQueue_disableSensor(sensorEventQueueBar,barometricSensor);
    	LOGD("Free Barometric Pressure");
    }
}

