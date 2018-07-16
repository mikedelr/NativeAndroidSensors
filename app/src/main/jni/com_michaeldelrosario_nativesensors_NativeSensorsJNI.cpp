//
// Created by Michael Del Rosario on 6/07/2018.
//
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <jni.h>
#include <string>

#include <android/looper.h>
#include <android/sensor.h>
#include <string.h>


#include "../cpp/NativeLogger.h"
#include "../cpp/SensorEngine.h"
#include "../cpp/SensorEvent.h"
#include "../cpp/SensorEventWriter.h"
#include "../cpp/SensorEventProcess.h"
#include "../cpp/GlobalObject.h"
#include "com_michaeldelrosario_nativesensors_NativeSensorsJNI.h"

//specify a call back to handle the sensor event

static int get_sensor_events(int fd, int events, void* data) {
    ASensorEvent event;
    while (bThreadRunning){
        if (ASensorEventQueue_getEvents(engine.sensorEventQueueAcc, &event, 1) > 0){
            if(event.type == ASENSOR_TYPE_ACCELEROMETER){
                sensorevent.updateAccelerometer(event);
                /*
                LOGI("accelerometer, %16.16f, %16.16f, %16.16f, %lld",
                event.acceleration.x, event.acceleration.y,
                event.acceleration.z, event.timestamp);
                */
            }
        }
        if (ASensorEventQueue_getEvents(engine.sensorEventQueueGyr, &event, 1) > 0){
            if(event.type == ASENSOR_TYPE_GYROSCOPE){
                sensorevent.updateGyroscope(event);
                /*
                LOGI("gyroscope, %16.16f, %16.16f, %16.16f, %lld",
                event.vector.x, event.vector.y,
                event.vector.z, event.timestamp);
                */
            }
        }
        if(ASensorEventQueue_getEvents(engine.sensorEventQueueMag, &event, 1) > 0){
            if(event.type == ASENSOR_TYPE_MAGNETIC_FIELD){
                sensorevent.updateMagnetometer(event);
                /*
                LOGI("magnetic, %16.16f, %16.16f, %16.16f, %lld",
                event.magnetic.x, event.magnetic.y,
                event.magnetic.z, event.timestamp);
                */
            }
        }
        if (ASensorEventQueue_getEvents(engine.sensorEventQueueBar, &event, 1) > 0){
            if(event.type == 6){ //BAROMETRIC PRESSURE
                sensorevent.updateBarometer(event);
                /*
                LOGI("pressure, %16.16f, %lld",
                event.pressure, event.timestamp);
                */
            }
        }

        if (sensorevent.isUpdateComplete()){
            /*
            LOGD("Write Data to file now");
            */
//            sensoreventWriter.writeSensorEvent(sensorevent);
            sensoreventProcess.processSensorEvent(sensorevent);
            sensorevent = SensorEvent(); // create new sensor event

        }
        //should return 1 to continue receiving callbacks, or 0 to unregister
        return 1;
    }
    return 0;
}


// ------------ Free Allocated memory resources
JNIEXPORT void JNICALL Java_com_delrosario_nativesensors_NativeService_nativeFree
        (JNIEnv * env, jobject obj){
    engine.shutDownSensors();
}

// --------------- Initialise resources required

JNIEXPORT void JNICALL Java_com_delrosario_nativesensors_NativeService_nativeInit
        (JNIEnv * env, jobject obj){
    int samplingRate=100; // Specify sampling frequency in hertz
    // engine is a SensorEngine object that is responsible for listening for sensor events
    engine.initialiseSensors(samplingRate, get_sensor_events);
    std::string subDirectory = "/NativeService/";
//    std::string fullSubPath = ::externalStorageDirectory+subDirectory;
//    mkdir(fullSubPath.c_str(),0770);
//    sensoreventWriter = SensorEventWriter(fullSubPath,samplingRate);
//    sensoreventProcess = SensorEventProcess(fullSubPath,samplingRate);
    sensoreventProcess = SensorEventProcess(samplingRate);
}

// ------------ Start threads to run -------------------------------------
JNIEXPORT void JNICALL Java_com_delrosario_nativesensors_NativeService_nativeStart
        (JNIEnv * env, jobject obj){
    //bThreadRunning is GLOBAL
    bThreadRunning = true;
}

// ----------- Stop threads that are running ------------------------------------
JNIEXPORT void JNICALL Java_com_delrosario_nativesensors_NativeService_nativeStop
        (JNIEnv * env, jobject obj){
    //bThreadRunning is GLOBAL
    bThreadRunning=false;
}
