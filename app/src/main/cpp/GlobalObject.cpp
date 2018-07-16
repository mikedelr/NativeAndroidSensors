#include <string.h>

#include "GlobalObject.h"
#include "SensorEngine.h"
#include "SensorEvent.h"
#include "SensorEventWriter.h"
#include "SensorEventProcess.h"

// Define global variables here
char* externalStorageDirectory = new char[100]; // defined here // NULL is not type safe
SensorEngine engine;
SensorEvent sensorevent;
bool bThreadRunning;
SensorEventWriter sensoreventWriter;
SensorEventProcess sensoreventProcess;
