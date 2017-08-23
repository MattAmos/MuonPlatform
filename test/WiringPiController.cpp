#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include <softPwm.h>
#include <wiringPi.h>
#include "../src/PinMap.h"

#define MIN_T 0
#define MAX_T 100

int main(int argc, char** argv) {
    if (wiringPiSetupGpio() < 0) {
        std::cout << "Error setting up wiringPi. Exiting..." << std::endl;
        return -1;
    }

    if (softPwmCreate(SERVO, 50, 100) != 0) {
        std::cout << "Error setting up pin" << SERVO << ". Exiting..." << std::endl;
        return -1;
    }

    float time = (MAX_T + MIN_T) / 2.0;

    while (true) {
        int ch = getch();
        switch (ch) {
            case 'w': break;
            case 'a': time -= 10; break;
            case 's': break;
            case 'd': time += 10; break;
        }
        if (time < MIN_T) {
            time = MIN_T;
        }
        else if (time > MAX_T) {
            time = MAX_T;
        }
        move(0, 0);
        addstr(("Uptime: " + std::to_string(time)).c_str());
        refresh();

        softPwmWrite(SERVO, int(time));
    }
}
