#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "../src/PinMap.h"
 
void setup() {
        wiringPiSetupGpio();
        pinMode(TRIG2, OUTPUT);
        pinMode(ECHO2, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(TRIG2, LOW);
        delay(30);
}
 
int getCM() {
        //Send trig pulse
        digitalWrite(TRIG2, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG2, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO2) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO2) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
 
int main(void) {
        setup();
	while(1)
	{ 
        	printf("Distance: %dcm\n", getCM());
 	}
        return 0;
}

