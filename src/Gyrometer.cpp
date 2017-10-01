#include "Gyrometer.h"

Gyrometer::Gyrometer()
{
	init();
}

Gyrometer::~Gyrometer()
{
	
}

int Gyrometer::init()
{
	fd = wiringPiI2CSetup(AD);

	if(fd==-1)
	{
		return -1;
	}

	wiringPiI2CReadReg8(fd, PM);
	wiringPiI2CWriteReg16(fd, PM, 0);

	return 1;
}

float Gyrometer::readGX()
{
	xG = wiringPiI2CReadReg8(fd, GX);
	return xG;
}

float Gyrometer::readGY()
{
	yG = wiringPiI2CReadReg8(fd, GY);
	return yG;
}

float Gyrometer::readGZ()
{
	zG = wiringPiI2CReadReg8(fd, GZ);
	return zG;
}

float Gyrometer::readAX()
{
	xA = wiringPiI2CReadReg8(fd, AX);
	return xA;
}

float Gyrometer::readAY()
{
	yA = wiringPiI2CReadReg8(fd, AY);
	return yA;
}

float Gyrometer::readAZ()
{
	zA = wiringPiI2CReadReg8(fd, AZ);
	return zA;
}