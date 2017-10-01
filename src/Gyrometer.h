#ifndef ACCGY_H
#define ACCGY_H

#include <wiringPiI2C.h>

#define GX	0x43
#define GY 	0x45
#define GZ	0x47

#define AX 	0x3B
#define AY 	0x3D
#define AZ	0x3F

#define PM 	0x6B
#define AD 	0x68

class Gyrometer
{
	private:
		int fd;
		float xG, xA, yG, yA, zG, zA;

	public:
		Gyrometer();
		~Gyrometer();
		int init();
		float readGX();
		float readGY();
		float readGZ();
		float readAX();
		float readAY();
		float readAZ();
};

#endif