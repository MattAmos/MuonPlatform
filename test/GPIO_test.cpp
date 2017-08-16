#include <curses.h>
#include <time.h>
#include "../include/joystick/joystick.cc"
#include "../src/Runner.h"

int min_t = 553, max_t = 2450;
int mid_t = (max_t + min_t) / 2.0;

// Create an instance of Joystick
Joystick joystick("/dev/input/js0");

float getUptime(float currTime) {
    float time = (currTime == -1 ? mid_t : currTime);

    if (joystick.isFound()) {
        JoystickEvent event;
        if (joystick.sample(&event) && event.isAxis()) {
            if (event.number == 0) {  // L3 HORIZONTAL
                time = mid_t + (event.value / 65535.0) * (max_t - min_t);
            }
        }
    }
    else {
        int ch = getch();
        switch (ch) {
            case 'a': time -= 100; break;
            case 'd': time += 100; break;
        }
        if (time < min_t) {
            time = min_t;
        }
        if (time > max_t) {
            time = max_t;
        }
    }
    return time;
}

int main(int argc, char** argv) {
    // Setup GPIO interface
    GPIO test = GPIO("12");
    test.setDirGPIO("out");
    test.setValGPIO("0");

    // Setup curses terminal
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    float time = -1;
    while (true) {
        time = getUptime(time);

        move(0, 0);
        addstr(("Duty Cycle: " + std::to_string(time) + " (us)").c_str());
        refresh();

        test.setValGPIO("1");
        usleep(time);
        test.setValGPIO("0");
        usleep(20000 - time);
    }

    return 0;
}
