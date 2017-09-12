#include <curses.h>
#include <time.h>
#include <csignal>
#include "../include/joystick/joystick.cc"
#include "../src/I2C.cpp"
#include "../src/PinMap.h"

I2C range = I2C(RANGE_ADDR);

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    test.~GPIO();
    exit(sigNum);
}

int main(int argc, char** argv) {
    // Register signal handler
    signal(SIGINT, signalHandler);

    // Setup GPIO interface

/*   char buff[9];
    while (true) {
        readI2C(8, buff);
        std::cout << "Output: " << buff << std::endl;
    }
*/
    return 0;
}
