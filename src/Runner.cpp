#include "Runner.h"

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    sensors.destroy();
    endwin();
    exit(sigNum);
}

int scp(std::string filename){
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
}

bool kbhit(){
    int ch = getch();
    if(ch!=ERR){
        ungetch(ch);
        return true;
    }
    else{
        return false;
    }
}

void* servo_pwm_thread(void* threadid){
    while (true) {
        sensors.servo.setValGPIO("1");
        usleep(sensors.servo.getPwmTime());
        sensors.servo.setValGPIO("0");
        usleep(20000 - sensors.servo.getPwmTime());
    }
}

void* dc_pwm_thread(void* threadid){
    while(true) {
        sensors.dc_move(sensors.move);
        usleep(10000);
        sensors.dc_move(DC_STOP);
        usleep(20000);
    }
}

void* inp_thread(void* threadid){
    while (true) {
        if (joystick.isFound()){
            if (joystick.sample(&event) && event.isAxis()) {
                if (event.number == 0) {  // L3 HORIZONTAL
                    sensors.servo.setPwmTime(mid_t + (event.value / 65535.0) * (SERVO_PWM_MAX - SERVO_PWM_MIN));
                }
                else {
                    std::cout << "Controller event: " << event.value << std::endl;
                }
            }
        }
        else {
            if(kbhit())
            {
                contFlag = true;
                int ch = getch();
                switch (ch) {
                    case 'a': sensors.servo.incPwmTime(100); break;
                    case 'd': sensors.servo.incPwmTime(-100); break;
                    case 'A': sensors.servo.setPwmTime(SERVO_PWM_MAX); break;
                    case 'D': sensors.servo.setPwmTime(SERVO_PWM_MIN); break;
                    case 'w': sensors.move = DC_FRWD; break;
                    case 's': sensors.move = DC_BACK; break;
                    case 32:  sensors.move = DC_STOP; break;
                    case 27:  contFlag = false; break;
                }
            }
        }
    }
}

void* test_thread(void* threadid)
{
    //Ranger rFinder = Ranger();
    int frontDist, leftDist, rightDist, backDist;
    while(true){
        if(!kbhit()){
            //Get sensors
            frontDist = sensors.sonic_front.getCM();
            leftDist  = sensors.sonic_left.getCM();
            rightDist = sensors.sonic_right.getCM();
            backDist  = sensors.sonic_back.getCM();

            //If something is immediately in front of car
            std::cout << "Front: " << frontDist << std::endl;
            if(frontDist > 0 && frontDist < 56){
                while(frontDist < 56 && backDist > 15){  //backup until a turn can be made
                    frontDist = sensors.sonic_front.getCM();
                    backDist  = sensors.sonic_back.getCM();
                    sensors.move = DC_BACK;
                }

      /*          frontDist = sensors.sonic_front.getCM();

                //Finished backing up
                if(frontDist < 56){   //if distance is still too small for a successful simple turn
                    //find which side has most room, go that way
                    //If we have room on the left
                    if(leftDist > rightDist)        { sensors.servo.setPwmTime(SERVO_PWM_MAX); }
                    else if(rightDist > leftDist)   { sensors.servo.setPwmTime(SERVO_PWM_MIN); }
                    else                            { sensors.servo.setPwmTime(SERVO_PWM_MID); }
                }
                sensors.move = DC_FRWD;
            }
            else{
                if(leftDist > rightDist + 5){
                    sensors.servo.setPwmTime(SERVO_PWM_MAX);
                }
                else if(rightDist > leftDist + 5){
                    sensors.servo.setPwmTime(SERVO_PWM_MIN);
                }
                sensors.move = DC_FRWD;
        */    }
        }
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    pthread_t threads[NUM_THREADS];
    int rc[NUM_THREADS], i[NUM_THREADS];
    sensors.init();
    wiringPiSetupGpio();

    //Setup curses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    //Create our threads
    rc[0] = pthread_create(&threads[0], NULL, servo_pwm_thread, &i[0]);
    rc[1] = pthread_create(&threads[1], NULL, inp_thread,       &i[1]);
    rc[2] = pthread_create(&threads[2], NULL, test_thread,      &i[2]);
    rc[3] = pthread_create(&threads[3], NULL, dc_pwm_thread,    &i[3]);
    pthread_exit(NULL);

    return 0;
}
