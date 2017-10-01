#include "Runner.h"

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    sensors.destroy();
    exit(sigNum);
}

int scp(std::string filename) {
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
}

int kbhit()
{
    int ch = getch();

    if(ch!=ERR)
    {
        ungetch(ch);
        return 1;
    }
    else
    {
        return 0;
    }
}

void* pwm_thread(void* threadid) {
    long tid;
    tid                                                           = (long) threadid;
    std::chrono::time_point<std::chrono::high_resolution_clock> t = std::chrono::high_resolution_clock::now();
    std::time_t timePt                                            = std::chrono::high_resolution_clock::to_time_t(t);
    std::cout << "Hello World! Thread ID, " << tid << " " << std::ctime(&timePt) << std::endl;
    while (true) {
        sensors.servo.setValGPIO("1");
        usleep(sensors.servo.getPwmTime());
        sensors.servo.setValGPIO("0");
        usleep(20000 - sensors.servo.getPwmTime());
    }

    pthread_exit(NULL);
}

void* inp_thread(void* threadid) {
    
    while (true) {
        if (joystick.isFound()) {
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
                    case 'w':
                        sensors.dc_2a.setValGPIO("0");
                        sensors.dc_3a.setValGPIO("0");
                        sensors.dc_1a.setValGPIO("1");
                        sensors.dc_4a.setValGPIO("1");
                        break;
                    case 's':
                        sensors.dc_1a.setValGPIO("0");
                        sensors.dc_4a.setValGPIO("0");
                        sensors.dc_2a.setValGPIO("1");
                        sensors.dc_3a.setValGPIO("1");
                        break;
                    case 32:
                        sensors.dc_1a.setValGPIO("0");
                        sensors.dc_4a.setValGPIO("0");
                        sensors.dc_2a.setValGPIO("0");
                        sensors.dc_3a.setValGPIO("0");
                        break;
                    case 27:
                        contFlag = false;
                        break;
                }
            }
        }
    }
}

void* test_thread(void* threadid)
{
    Ranger rFinder = Ranger();
    Sonic ultraBack = Sonic(ECHO1, TRIG1);
//    Sonic ultraLeft = Sonic(ECHO2, TRIG2);
//    Sonic ultraRight = Sonic(ECHO3, TRIG3);

    ultraBack.setup();

    while(true)
    {
        if(!kbhit())
        {
            int distance = rFinder.getDistanceMM();
            int rear = 9999;
            int left;
            int right;
            if(distance>0 && distance < 560)    //if something infront is within the minimum 90 deg turning distance
            {
                while(distance < 560 && rear > 10)  //backup until a turn can be made
                {
                    distance = rFinder.getDistanceMM();
                //    rear = ultraBack.getCM();
                    sensors.dc_2a.setValGPIO("1");
                    sensors.dc_3a.setValGPIO("1");
                    sensors.dc_1a.setValGPIO("0");
                    sensors.dc_4a.setValGPIO("0");
                }
                
                sensors.dc_1a.setValGPIO("0");
                sensors.dc_4a.setValGPIO("0");
                sensors.dc_2a.setValGPIO("0");
                sensors.dc_3a.setValGPIO("0"); 
                usleep(1000000);
                distance = rFinder.getDistanceMM();
                if(distance <560)   //if distance is still too small for a successful simple turn
                {
                    //figure something out
                }
                else
                {
                    std::cout << distance << std::endl;
                    while(true)
                    {
                         //check left and right sensors
                        //choose left by default, choose right if something's within the turning arc
                        left = ultraBack.getCM(); //replace with actual sensor
                         //   right = ultraRight.getCM();
                        if(left > 10)
                        {
                            sensors.servo.setPwmTime(SERVO_PWM_MAX);
                            sensors.servo.setValGPIO("1");
                            usleep(sensors.servo.getPwmTime());
                            sensors.servo.setValGPIO("0");
                            usleep(20000 - sensors.servo.getPwmTime());
                        }
                    }
                   
                }
                //crash mitigation
            }
            else
            {
                sensors.dc_2a.setValGPIO("0");
                sensors.dc_3a.setValGPIO("0");
                sensors.dc_1a.setValGPIO("1");
                sensors.dc_4a.setValGPIO("1");
                //business as usual -- pwm to get lowest possible speed
            }
        }
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    pthread_t threads[NUM_THREADS];
    int rc, i = 0;
    sensors.init();
    wiringPiSetupGpio();

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
 
//    rc = pthread_create(&threads[0], NULL, pwm_thread, &i);
    rc = pthread_create(&threads[1], NULL, inp_thread, &(++i));
    rc = pthread_create(&threads[2], NULL, test_thread, &(++i));
    pthread_exit(NULL);
  
    return 0;
}
