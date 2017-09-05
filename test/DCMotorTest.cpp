#include <curses.h>
#include <time.h>
#include <csignal>
#include "../include/joystick/joystick.cc"
#include "../src/GPIO.cpp"
#include "../src/PinMap.h"

GPIO dc_a = GPIO(std::to_string(DC_A));
GPIO dc_b = GPIO(std::to_string(DC_B));
GPIO dc_s = GPIO(std::to_string(DC_S));

// Create an instance of Joystick
Joystick joystick("/dev/input/js0");
JoystickEvent event;

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    dc_a.~GPIO();
    dc_b.~GPIO();
    dc_s.~GPIO();
    exit(sigNum);
}

int main(int argc, char** argv) {
    // Register signal handler
    signal(SIGINT, signalHandler);

    // Setup GPIO interface
    dc_a.setDirGPIO("out");
    dc_a.setValGPIO("0");
    dc_b.setDirGPIO("out");
    dc_b.setValGPIO("1");

    return 0;
}
