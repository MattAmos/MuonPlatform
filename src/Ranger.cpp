#include "Ranger.h"

Ranger::Ranger()
{
	rangeInit();
}

Ranger::~Ranger()
{
	 if(Status == VL53L0X_ERROR_NONE)
    {
   //     printf ("Call of VL53L0X_StopMeasurement\n");
        Status = VL53L0X_StopMeasurement(pMyDevice);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
  //      printf ("Wait Stop to be competed\n");
        WaitStopCompleted();
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
    	Status = VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    }
	
	VL53L0X_i2c_close();
    print_pal_error();
}

int Ranger::rangeInit()
{
	pMyDevice ->I2cDevAddr = 0x29;
	pMyDevice->fd = VL53L0X_i2c_init("/dev/i2c-1", pMyDevice->I2cDevAddr);
	if (MyDevice.fd<0) 
	{
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        printf ("Failed to init ranger\n");
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        status_int = VL53L0X_GetVersion(pVersion);
        if (status_int != 0)
        {
            Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        }
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        if(pVersion->major != VERSION_REQUIRED_MAJOR || pVersion->minor != VERSION_REQUIRED_MINOR || pVersion->build != VERSION_REQUIRED_BUILD)
        {
//            printf("VL53L0X API Version Error: Your firmware has %d.%d.%d (revision %d). This example requires %d.%d.%d.\n", pVersion->major, pVersion->minor, pVersion->build, pVersion->revision, VERSION_REQUIRED_MAJOR, VERSION_REQUIRED_MINOR, VERSION_REQUIRED_BUILD);
        }
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
      //  printf ("Call of VL53L0X_DataInit\n");
        Status = VL53L0X_DataInit(&MyDevice); // Data initialization
        print_pal_error();
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        Status = VL53L0X_GetDeviceInfo(&MyDevice, &DeviceInfo);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
/*		printf("VL53L0X_GetDeviceInfo:\n");
		printf("Device Name : %s\n", DeviceInfo.Name);
		printf("Device Type : %s\n", DeviceInfo.Type);
		printf("Device ID : %s\n", DeviceInfo.ProductId);
		printf("ProductRevisionMajor : %d\n", DeviceInfo.ProductRevisionMajor);
		printf("ProductRevisionMinor : %d\n", DeviceInfo.ProductRevisionMinor);
*/
        if ((DeviceInfo.ProductRevisionMinor != 1) && (DeviceInfo.ProductRevisionMinor != 1)) 
        {
        	printf("Error expected cut 1.1 but found cut %d.%d\n", DeviceInfo.ProductRevisionMajor, DeviceInfo.ProductRevisionMinor);
        	Status = VL53L0X_ERROR_NOT_SUPPORTED;
        }
    }
    if(Status == VL53L0X_ERROR_NONE)
    {
   //     printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        // StaticInit will set interrupt by default
        print_pal_error();
    }
     if(Status == VL53L0X_ERROR_NONE)
    {
     //   printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal); // Device Initialization
        print_pal_error();
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
//        printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice, &refSpadCount, &isApertureSpads); // Device Initialization
        print_pal_error();
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

//        printf ("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); // Setup in single ranging mode
        print_pal_error();
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
//		printf ("Call of VL53L0X_StartMeasurement\n");
		Status = VL53L0X_StartMeasurement(pMyDevice);
		print_pal_error();
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
    	return 0;
    } 
    else
    {
    	return -1;
    }
}	

void Ranger::print_pal_error()
{
	char buf[VL53L0X_MAX_STRING_LENGTH];
	VL53L0X_GetPalErrorString(Status, buf);
//	printf("API Status: %i : %s\n", Status, buf);
}

void Ranger::print_range_status()
{
	char buf[VL53L0X_MAX_STRING_LENGTH];
	uint8_t RangeStatus;

	RangeStatus = pRangingMeasurementData->RangeStatus;

	VL53L0X_GetRangeStatusString(RangeStatus, buf);
//	printf("Range Status: %i : %s\n", RangeStatus, buf);
}

void Ranger::WaitStopCompleted()
{
	uint32_t StopCompleted=0;
    uint32_t LoopNb;

    if (Status == VL53L0X_ERROR_NONE) 
    {
        LoopNb = 0;
        do 
        {
            Status = VL53L0X_GetStopCompletedStatus(pMyDevice, &StopCompleted);
            if ((StopCompleted == 0x00) || Status != VL53L0X_ERROR_NONE) 
            {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(pMyDevice);
        } 
        while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) 
        {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }
}

void Ranger::WaitMeasurementDataReady() 
{
    uint8_t NewDatReady=0;
    uint32_t LoopNb;

  
    if (Status == VL53L0X_ERROR_NONE) 
    {
        LoopNb = 0;
        do 
        {
            Status = VL53L0X_GetMeasurementDataReady(pMyDevice, &NewDatReady);
            if ((NewDatReady == 0x01) || Status != VL53L0X_ERROR_NONE) 
            {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(pMyDevice);
        } 
        while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) 
        {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }
}

uint16_t Ranger::getDistanceMM()
{
	if(Status == VL53L0X_ERROR_NONE)
	{
	//	uint16_t* pResults = (uint16_t*)malloc(sizeof(uint16_t) * 1);
		uint16_t result;
		WaitMeasurementDataReady();

		if(Status == VL53L0X_ERROR_NONE)
		{
			Status = VL53L0X_GetRangingMeasurementData(pMyDevice, pRangingMeasurementData);
			result = pRangingMeasurementData->RangeMilliMeter;
			VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
			return result; 
		}
	}
}