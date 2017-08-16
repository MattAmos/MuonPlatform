#include <time.h>
#include <curses.h>
#include "../include/joystick/joystick.cc"
#include "../src/Runner.h"

float getUptime(float currTime){
    int min_t = 553, max_t = 2450;
    int mid_t = (max_t + min_t) / 2.0;

    float time = (currTime == -1 ? mid_t : currTime);

    // TODO: Determine performance impact, move out of function?
    // Create an instance of Joystick
    Joystick joystick("/dev/input/js0");

    if(joystick.isFound()){
        JoystickEvent event;
        if (joystick.sample(&event) && event.isAxis()) {
            if (event.number == 0) {  // L3 HORIZONTAL
                time = mid_t + (event.value / 65535.0) * (max_t - min_t);
            }
        }
    }
    else{
        int ch = getch();
        switch(ch){
            case 'a':
                time -= 100;
            break;
            case 'd':
                time += 100;
            break;
        }
        if(time < min_t){ time = min_t; }
        if(time > max_t){ time = max_t; }
        move(0,0);
        addstr(("Uptime: " + std::to_string(time)).c_str());
        refresh();
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

        test.setValGPIO("1");
        usleep(time);
        test.setValGPIO("0");
        usleep(20000 - time);
    }

    return 0;
}
