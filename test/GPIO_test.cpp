#include "../src/Runner.h"

int main(int argc, char** argv) {
    Servo test;
    test.id   = FL_DRIVE;
    test.gpio = new GPIO("3");

    std::cout << (test.id) << (test.gpio)->getGPIONum() << std::endl;

    return 0;
}
