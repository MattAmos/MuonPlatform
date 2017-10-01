#include <iostream>
#include <wiringPiI2C.h>

#define GX	0x43
#define GY	0x45
#define GZ	0x47

#define PM 	0x6B
#define AD	0x68

#define AX	0x3B
#define AY	0x3D
#define AZ	0x3F

using namespace std;

int main()
{
	int fd, flag=0;
	float x1, x2, y1, y2, z1, z2;

	fd = wiringPiI2CSetup(AD);

	if(fd==-1)
	{
		cout<<"Cannot open communications"<<endl;
		return -1;
	}
	
	wiringPiI2CReadReg8(fd, PM);
	wiringPiI2CWriteReg16(fd, PM, 0);

	while(1)
	{
		x1 = wiringPiI2CReadReg8(fd, GX);
		x2 = wiringPiI2CReadReg8(fd, AX);

		y1 = wiringPiI2CReadReg8(fd, GY);
		y2 = wiringPiI2CReadReg8(fd, AY);

		z1 = wiringPiI2CReadReg8(fd, GZ);
		z2 = wiringPiI2CReadReg8(fd, AZ);
		if(++flag > 50)
		{
			cout << "Gyro: x:" << x1 <<" y:" << y1 << " z:" << z1 <<endl;
//			cout << "Accl: x:" << x2 <<" y:" << y2 << " z:" << z2 << endl; 
			flag = 0;
		}
	}

	return 0;		
}
