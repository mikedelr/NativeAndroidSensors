#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <stdlib.h>

#include "NativeLogger.h"
#include "SensorEventProcess.h"
#include "SensorEvent.h"
#include "Constants.h"

SensorEventProcess::SensorEventProcess(){}
SensorEventProcess::~SensorEventProcess(){}

//// sampling frequency and root directory for file writing of processed outputs
//SensorEventProcess::SensorEventProcess(std::string rootExternalStorage, int sampFreqHz)
//{
//	_smpCtr       = 0;
//	_samplingRate = sampFreqHz;
//	_rootDir      = rootExternalStorage;
//	// allocate memory for filters and buffers etc.
//	memset(_accDataBuffX, 0, sizeof(_accDataBuffX)); // initialise with zeros
//	memset(_accDataBuffY, 0, sizeof(_accDataBuffY));
//	memset(_accDataBuffZ, 0, sizeof(_accDataBuffZ));
//	memset(_gyrDataBuffX, 0, sizeof(_gyrDataBuffX));
//	memset(_gyrDataBuffY, 0, sizeof(_gyrDataBuffY));
//	memset(_gyrDataBuffZ, 0, sizeof(_gyrDataBuffZ));
//	memset(_barDataBuff, 0, sizeof(_barDataBuff));
//
//	memset(_lowPassAcc3, 0, sizeof(_lowPassAcc3));
//	memset(_lowPassDiffAcc3, 0, sizeof(_lowPassDiffAcc3));
//	memset(_bandPassGyr3, 0, sizeof(_bandPassGyr3));
//
//	featStartupOver = false;
//	featVecReady    = false;
//	firstFeatVector = false;
//	featVecIdx      = 0;
//	featVecDeltaCnt = 0;
//}

// sampling frequency and root directory for file writing of processed outputs
SensorEventProcess::SensorEventProcess(int sampFreqHz)
{
	_smpCtr       = 0;
	_samplingRate = sampFreqHz;
	// allocate memory for filters and buffers etc.
	memset(_accDataBuffX, 0, sizeof(_accDataBuffX)); // initialise with zeros
	memset(_accDataBuffY, 0, sizeof(_accDataBuffY));
	memset(_accDataBuffZ, 0, sizeof(_accDataBuffZ));
	memset(_gyrDataBuffX, 0, sizeof(_gyrDataBuffX));
	memset(_gyrDataBuffY, 0, sizeof(_gyrDataBuffY));
	memset(_gyrDataBuffZ, 0, sizeof(_gyrDataBuffZ));
	memset(_barDataBuff, 0, sizeof(_barDataBuff));

	memset(_lowPassAcc3, 0, sizeof(_lowPassAcc3));
	memset(_lowPassDiffAcc3, 0, sizeof(_lowPassDiffAcc3));
	memset(_bandPassGyr3, 0, sizeof(_bandPassGyr3));

	featStartupOver = false;
	featVecReady    = false;
	firstFeatVector = false;
	featVecIdx      = 0;
	featVecDeltaCnt = 0;
}

int64_t SensorEventProcess::unixTimeStampSeconds()
{
    struct timespec t; //seconds and nanoseconds
    clock_gettime(CLOCK_BOOTTIME, &t); // Time since boot, available as of ndk-9c
    int64_t unixTime = (int64_t)time(NULL); //equivalent to System.currentTimeMillis
	return unixTime; //unix timestamp
}

void SensorEventProcess::checkCounters()
{
	if(_smpCtr == Constants::SAMPLE_MAXVAL)
	{
		_smpCtr = Constants::LPF_N;
	}
	if(_barSmpCtr == Constants::BAR_SAMPLE_MAXVAL)
	{
		_barSmpCtr = Constants::DIFF_PRESS_N;
	}
}

// add filtering methods here
void SensorEventProcess::processSensorEvent(SensorEvent sensorevent)
{
	_sensorData[0] = sensorevent.getSensorData(); // local copy
	checkCounters();
	if(sensorevent.isBarLoaded()){
		// filter barometric pressure signal
		fillBarBuff();
		if(_barSmpCtr < Constants::DIFF_PRESS_N-1){
			_diffPressVal=0;
		} else {
			filterBarSignal();
		}
		_barSmpCtr++;
	}
	// ADD AHRS METHODS HERE
	fillAccGyrBuff();
	filterAccGyrSignals();
	// extract feature vectors from _featVecAccGyr = featVector.extractFeatures(_lowPassAcc3,_lowPassDiffAcc3,_bandPassGyr3);
	LOGD("LPF     := %4.4f, %4.4f ,%4.4f",_lowPassAcc3[0],_lowPassAcc3[1],_lowPassAcc3[2]);
	LOGD("LPF_DIFF:= %4.4f, %4.4f ,%4.4f",_lowPassDiffAcc3[0],_lowPassDiffAcc3[1],_lowPassDiffAcc3[2]);
	LOGD("BPF     := %4.4f, %4.4f ,%4.4f",_bandPassGyr3[0],_bandPassGyr3[1],_bandPassGyr3[2]);

	writeFilterValuesToFile(); // _lowPassAcc3, _lowPassDiffAcc3, _bandPassGyr3,_diffPressVal
	//_featVecAccGyr = featVector.extractFeatures(_lowPassAcc3,_lowPassDiffAcc3,_bandPassGyr3);
	writeFeatVecToFile(); 	// _featVecAccGyr  - for debug

	checkFeatCounters();
	bufferFeatures();//_cntBar,_cntAccGyr,_featVecAccGyr,_diffPressVal
	updateAccumulationStatus();
	if (isReadyForExtraction()){
		//windowVector = extractWindow();
		///_activityState = classify(windowVector);
		//writeActivityState();
		// for debug
		//writeWindowVector(windowVector);
	}
	_smpCtr++;
	_cntBar++;
	_cntAccGyr++;
}

void SensorEventProcess::writeFilterValuesToFile()//_lowPassAcc3, _lowPassDiffAcc3, _bandPassGyr3,_diffPressVal
{

}
//_featVecAccGyr = featVector.extractFeatures(_lowPassAcc3,_lowPassDiffAcc3,_bandPassGyr3);
void SensorEventProcess::writeFeatVecToFile() 	//_featVecAccGyr for debug
{

}

bool SensorEventProcess::isReadyForExtraction(){
	return  (_smpCtr>(Constants::DIFF_PRESS_N-1)) && featStartupOver && featVecReady;
}

void SensorEventProcess::fillAccGyrBuff()
{
	int buffAccGyrIdx = _smpCtr & Constants::ACC_GYR_SIZE_MINUS_ONE;
	_accDataBuffX[buffAccGyrIdx] = _sensorData[0].accX;
	_accDataBuffY[buffAccGyrIdx] = _sensorData[0].accY;
	_accDataBuffZ[buffAccGyrIdx] = _sensorData[0].accZ;
	_gyrDataBuffX[buffAccGyrIdx] = _sensorData[0].gyrX;
	_gyrDataBuffY[buffAccGyrIdx] = _sensorData[0].gyrY;
	_gyrDataBuffZ[buffAccGyrIdx] = _sensorData[0].gyrZ;
}

void SensorEventProcess::filterAccGyrSignals()
{
	resetAccGyrFilterValues();
	int buffAccGyrIdx = _smpCtr & (Constants::ACC_GYR_SIZE_MINUS_ONE);
	int fwdLoops = buffAccGyrIdx+1;
	int backPtr=1;
	for(int f=0;f<fwdLoops;f++)// at least once
	{
		//traverse buffers with different coefficients one after the other
		_lowPassAcc3[0]      += _accDataBuffX[buffAccGyrIdx] * Constants::LPF_COEFFICIENTS[f];
		_lowPassAcc3[1]      += _accDataBuffY[buffAccGyrIdx] * Constants::LPF_COEFFICIENTS[f];
		_lowPassAcc3[2]      += _accDataBuffZ[buffAccGyrIdx] * Constants::LPF_COEFFICIENTS[f];
		_lowPassDiffAcc3[0]  += _accDataBuffX[buffAccGyrIdx] * Constants::LPF_DIFF_COEFFICIENTS[f];
		_lowPassDiffAcc3[1]  += _accDataBuffY[buffAccGyrIdx] * Constants::LPF_DIFF_COEFFICIENTS[f];
		_lowPassDiffAcc3[2]  += _accDataBuffZ[buffAccGyrIdx] * Constants::LPF_DIFF_COEFFICIENTS[f];
		_bandPassGyr3[0]     += _gyrDataBuffX[buffAccGyrIdx] * Constants::BPF_COEFFICIENTS[f];
		_bandPassGyr3[1]     += _gyrDataBuffY[buffAccGyrIdx] * Constants::BPF_COEFFICIENTS[f];
		_bandPassGyr3[2]     += _gyrDataBuffZ[buffAccGyrIdx] * Constants::BPF_COEFFICIENTS[f];
		buffAccGyrIdx--;
	}
	for(int b=fwdLoops;b<Constants::LPF_N;b++)
	{
		_lowPassAcc3[0]      += _accDataBuffX[Constants::LPF_N-backPtr] * Constants::LPF_COEFFICIENTS[b];
		_lowPassAcc3[1]      += _accDataBuffY[Constants::LPF_N-backPtr] * Constants::LPF_COEFFICIENTS[b];
		_lowPassAcc3[2]      += _accDataBuffZ[Constants::LPF_N-backPtr] * Constants::LPF_COEFFICIENTS[b];
		_lowPassDiffAcc3[0]  += _accDataBuffX[Constants::LPF_DIFF_N-backPtr] * Constants::LPF_DIFF_COEFFICIENTS[b];
		_lowPassDiffAcc3[1]  += _accDataBuffY[Constants::LPF_DIFF_N-backPtr] * Constants::LPF_DIFF_COEFFICIENTS[b];
		_lowPassDiffAcc3[2]  += _accDataBuffZ[Constants::LPF_DIFF_N-backPtr] * Constants::LPF_DIFF_COEFFICIENTS[b];
		_bandPassGyr3[0]     += _gyrDataBuffX[Constants::BPF_N-backPtr] * Constants::BPF_COEFFICIENTS[b];
		_bandPassGyr3[1]     += _gyrDataBuffY[Constants::BPF_N-backPtr] * Constants::BPF_COEFFICIENTS[b];
		_bandPassGyr3[2]     += _gyrDataBuffZ[Constants::BPF_N-backPtr] * Constants::BPF_COEFFICIENTS[b];
		backPtr++;
	}
}

void SensorEventProcess::resetAccGyrFilterValues()
{
	memset(_lowPassAcc3, 0, sizeof(_lowPassAcc3));
	memset(_lowPassDiffAcc3, 0, sizeof(_lowPassDiffAcc3));
	memset(_bandPassGyr3, 0, sizeof(_bandPassGyr3));
}

void SensorEventProcess::fillBarBuff()
{
	int barBuffIdx = _barSmpCtr & (Constants::BAR_SIZE_MINUS_ONE);
	_barDataBuff[barBuffIdx] = _sensorData[0].barVal;
}

void SensorEventProcess::filterBarSignal()
{
	_diffPressVal=0; // reset before calculating the new value
	int buffBarIdx = _barSmpCtr & (Constants::BAR_SIZE_MINUS_ONE);
	int fwdLoops = buffBarIdx+1;
	int backPtr=1;
	for(int f=0;f<fwdLoops;f++){ // at least once
		//traverse buffers with different coefficients one after the other
		_diffPressVal += _barDataBuff[buffBarIdx]*Constants::DIFF_PRESS_COEFFICIENTS[f];
		buffBarIdx--;
	}
	for(int b=fwdLoops;b<Constants::DIFF_PRESS_N;b++){
		_diffPressVal += _barDataBuff[Constants::DIFF_PRESS_N-backPtr]*Constants::DIFF_PRESS_COEFFICIENTS[b];
		backPtr++;
	}
}

void SensorEventProcess::checkFeatCounters()
{
	if(_cntBar == Constants::BAR_SIZE_RESET_VAL){
		_cntBar = 0;
	}
	if(_cntAccGyr == Constants::ACC_GYR_SIZE_RESET_VAL){
		_cntAccGyr = 0;
	}
}

void SensorEventProcess::bufferFeatures()
{

}


void SensorEventProcess::updateAccumulationStatus(){
	if(isAccumulationComplete()){
		if(!featStartupOver){
			featVecIdx++;
			if (featVecIdx > Constants::STARTUP_WINDOWS){
				featStartupOver = true;
			}
		}
		if(featStartupOver){
			if(!firstFeatVector){
				featVecReady = true;
				featVecDeltaCnt = 0;
				firstFeatVector = true;
			} else {
				if (featVecDeltaCnt == Constants::HALF_PERIOD){
					featVecReady = true;
					featVecDeltaCnt = 0;
				} else {
					featVecDeltaCnt++;
				}
			}
		}
	}
}

bool SensorEventProcess::isAccumulationComplete(){
	bool isComplete = false;
	if ((_smpCtr & Constants::ACCUM_SIZE_ACC_GYR_MINUS_ONE) == Constants::ACCUM_SIZE_ACC_GYR_MINUS_ONE){
		isComplete=true;
	}
	return isComplete;
}


