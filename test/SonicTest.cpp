#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include "../src/PinMap.h"
 
void setup() {
        wiringPiSetupGpio();
	pinMode(TRIG1, OUTPUT);
	pinMode(ECHO1, INPUT);

        pinMode(TRIG2, OUTPUT);
        pinMode(ECHO2, INPUT);

	pinMode(TRIG3, OUTPUT);
	pinMode(ECHO3, INPUT);

	pinMode(TRIG4, OUTPUT);
	pinMode(ECHO4, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(TRIG1, LOW);
	digitalWrite(TRIG2, LOW);
	digitalWrite(TRIG3, LOW);
	digitalWrite(TRIG4, LOW);
        delay(30);
}
 
int getCM(int trig, int echo) {
        //Send trig pulse
        digitalWrite(trig, HIGH);
        delayMicroseconds(20);
        digitalWrite(trig, LOW);
 
        //Wait for echo start
        while(digitalRead(echo) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(echo) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
 
int main(void) {
        setup();
	std::cout << "Setup!" << std::endl;
	int result1, result2, result3, result4;
	while(true)
	{
		result1 = getCM(TRIG1, ECHO1);
		result2 = getCM(TRIG2, ECHO2);
		result3 = getCM(TRIG3, ECHO3);
		result4 = getCM(TRIG4, ECHO4);
		//std::cout << "Got result 3" << std::endl;
		std::cout << result1 << ", " << result2 << ", " << result3 << ", " << result4 << std::endl;
 	}
        return 0;
}

