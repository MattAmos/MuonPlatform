#include "../src/Gyrometer.cpp"
#include <stdio.h>

int main()
{
	float *gyrout;
	Gyrometer test = Gyrometer();

	while(true)
	{
		printf("%f %f %f\n", test.readGX(), test.readGY(), test.readGZ());
	}

	return 1;
}
