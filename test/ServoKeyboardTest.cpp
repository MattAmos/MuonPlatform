#include <unistd.h>
#include <curses.h>
#include <stdio.h>
#include <string>

int main(int argc, char** argv) {
    // Setup Curses Terminal
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int min_t = 553, max_t = 2450;
    int mid_t = (max_t + min_t) / 2.0;

    float time = mid_t;
    bool output;

    while (true) {
        int ch = getch();
        switch(ch){
            case 'a':
                time -= 100;
            break;
            case 'd':
                time += 100;
            break;
        }
        if(time < 553){
            time = 553;
        }
        else if(time > 2450){
            time = 2450;
        }
        move(0,0);
        addstr(("Uptime: " + std::to_string(time)).c_str());
        refresh();
    }
}
