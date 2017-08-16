#include <time.h>
#include "../include/joystick/joystick.cc"
#include "../src/Runner.h"

int main(int argc, char** argv) {
    // min 553u max 2450u
    int min_t = 553, max_t = 2450;
    int mid_t = (max_t + min_t) / 2.0;

    // Create an instance of Joystick
    Joystick joystick("/dev/input/js0");

    // Ensure that it was found and that we can use it
    if (!joystick.isFound()) {
        printf("No controller found. Exiting...\n");
        exit(1);
    }

    float time  = mid_t;
    bool output = true;

    while (true) {
        // Attempt to sample an event from the joystick
        JoystickEvent event;
        if (joystick.sample(&event) && event.isAxis()) {
            if (event.number == 0) {  // L3 HORIZONTAL
                time = mid_t + (event.value / 65535.0) * (max_t - min_t);
            }
        }

        output = true;
        usleep(time);
        output = false;
        usleep(20000 - time);
        std::cout << "On: " << time << " Off: " << 20000 - time << std::endl;
    }

    return 0;
}
