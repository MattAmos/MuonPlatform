#include <time.h>
#include "../src/Runner.h"

int main(int argc, char** argv) {
    int usec = 900;

    struct timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = 1000 * usec;

    bool output = true;

    if (nanosleep(&tim, NULL) < 0) {
	std::cout << "Unsuccessful" << std::endl;
    }

    Servo test;
    test.id   = 16;
    test.gpio = new GPIO("16");

    test.gpio->setDirGPIO("out");
    test.gpio->setValGPIO("1");

    while (true) {
        if (nanosleep(&tim, NULL) < 0) {
            break;
        }
        output = !output;
        test.gpio->setValGPIO(std::to_string(output));
    }

    // std::cout << (test.id) << (test.gpio)->getGPIONum() << std::endl;

    return 0;
}
