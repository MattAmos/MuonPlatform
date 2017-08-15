#include <time.h>
#include "../src/Runner.h"

int main(int argc, char** argv) {
    int usec = 900;

    struct timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = 1000 * usec;

    struct timespec rem;
    rem.tv_sec  = 0;
    tim.tv_nsec = 20000000;

    bool output = true;

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
        if (nanosleep(&rem, NULL) < 0) {
            break;
        }
    }

    // std::cout << (test.id) << (test.gpio)->getGPIONum() << std::endl;

    return 0;
}
