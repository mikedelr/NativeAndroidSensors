#ifndef SENSOREVENTPROCESS_H
#define SENSOREVENTPROCESS_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "SensorEvent.h"

class SensorEventProcess{
	private:
		std::string _rootDir;
		std::string _pathToFile;
		int _samplingRate;
		int _smpCtr;
		int _barSmpCtr;
		int _cntBar;
		int _cntAccGyr;

		sensor_data _sensorData[1];

		double _diffPressVal;

		double _accDataBuffX[256];
		double _accDataBuffY[256];
		double _accDataBuffZ[256];
		double _gyrDataBuffX[256];
		double _gyrDataBuffY[256];
		double _gyrDataBuffZ[256];

		double _barDataBuff[128];

		double _lowPassAcc3[3];
		double _lowPassDiffAcc3[3];
		double _bandPassGyr3[3];

		void checkCounters();
		void fillAccGyrBuff();
		void filterAccGyrSignals();
		void resetAccGyrFilterValues();
		void fillBarBuff();
		void filterBarSignal();
		void checkFeatCounters();
		void bufferFeatures();
		int64_t unixTimeStampSeconds();
		void updateAccumulationStatus();
		bool isAccumulationComplete();
		bool isReadyForExtraction();
		void writeFilterValuesToFile();
		void writeFeatVecToFile();

		// ---------------- Feature Buffer Flags ----------------------------
		bool featStartupOver;
		bool featVecReady;
		bool firstFeatVector;
		int  featVecIdx;
		int  featVecDeltaCnt;
		// -------------------------------------------------------------------
	public:
		SensorEventProcess();
//		SensorEventProcess(std::string rootExternalStorage, int sampFreqHz); // sampling frequency and root directory for file writing of processed outputs
		SensorEventProcess(int sampFreqHz); // sampling frequency and root directory for file writing of processed outputs
		~SensorEventProcess();
		void processSensorEvent(SensorEvent sensorevent);
};


#endif
