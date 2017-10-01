#include "Sonic.h"
#include <wiringPi.h>

Sonic::Sonic(int e, int t)
{
	echoI = e;
	triggerO = t;
}

Sonic::~Sonic()
{
	
}

void Sonic::setup()
{
	pinMode(triggerO, OUTPUT);
	pinMode(echoI, INPUT);

	digitalWrite(triggerO, LOW);
	delay(30);
}

int Sonic::getCM()
{
	digitalWrite(triggerO, HIGH);
	delayMicroseconds(20);
	digitalWrite(triggerO, LOW);

	while(digitalRead(echoI)==LOW);

	long startTime = micros();
	while(digitalRead(echoI)==HIGH);
	long travelTime = micros() - startTime;

	int distance = travelTime/58;

	return distance;
}
