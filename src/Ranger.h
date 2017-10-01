#ifndef RANGER_H
#define RANGER_H

#include <stdlib.h>
#include <stdio.h>
#include "../include/VL53L0X/vl53l0x_api.h"
#include "../include/VL53L0X/vl53l0x_platform.h"


#define VERSION_REQUIRED_MAJOR 1
#define VERSION_REQUIRED_MINOR 0
#define VERSION_REQUIRED_BUILD 1

class Ranger
{
	private:
		VL53L0X_Error 			Status = VL53L0X_ERROR_NONE;
		VL53L0X_Dev_t 			MyDevice;
		VL53L0X_Dev_t 			*pMyDevice = &MyDevice;
		VL53L0X_Version_t 		Version;
		VL53L0X_Version_t 		*pVersion   = &Version;
		VL53L0X_DeviceInfo_t 	DeviceInfo;
		int32_t 				status_int;
		VL53L0X_RangingMeasurementData_t RangingMeasurementData;
		VL53L0X_RangingMeasurementData_t *pRangingMeasurementData = &RangingMeasurementData;
		uint32_t refSpadCount;
		uint8_t isApertureSpads;
		uint8_t VhvSettings;
		uint8_t PhaseCal;

	public:
		Ranger();
		~Ranger();
		int rangeInit();
		void print_pal_error();
		void print_range_status();
		void WaitStopCompleted();
		void WaitMeasurementDataReady();
		uint16_t getDistanceMM();
};

#endif