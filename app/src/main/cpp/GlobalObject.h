#ifndef GLOBALOBJECT_H
#define GLOBALOBJECT_H

#include "SensorEvent.h"
#include "SensorEngine.h"
#include "SensorEventWriter.h"
#include "SensorEventProcess.h"

	//Global variables defined in globalObject.cpp, i.e. all externs defined there
	// extern keyword signifies that they are defined elsewhere
	extern char* externalStorageDirectory;
	extern SensorEngine engine;
	extern SensorEvent sensorevent;
	extern bool bThreadRunning;
	extern SensorEventWriter sensoreventWriter;
	extern SensorEventProcess sensoreventProcess;
#endif
