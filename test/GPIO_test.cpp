#include <time.h>
#include "../src/Runner.h"

int main(int argc, char** argv) {
    // int usec = 900;
    struct timespec rem;
    rem.tv_sec  = 0;
    tim.tv_nsec = 20000000;

    int minTime = 1;
    int maxTime = 5;

    bool output = true;

    Servo test;
    test.id   = 12;
    test.gpio = new GPIO("12");

    test.gpio->setDirGPIO("out");
    test.gpio->setValGPIO("1");

    for (int i = minTime; i < maxTime; i++) {
        struct timespec tim;
        tim.tv_sec  = 0;
        tim.tv_nsec = 1000000 * i;

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
    }

    // std::cout << (test.id) << (test.gpio)->getGPIONum() << std::endl;

    return 0;
}
