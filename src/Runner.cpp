#include "Runner.h"

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    sensors.destroy();
    exit(sigNum);
}

int scp(std::string filename) {
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
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
            int ch = getch();
            switch (ch) {
                case 'a': sensors.servo.incPwmTime(10); break;
                case 'd': sensors.servo.incPwmTime(-10); break;
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
            }
        }
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    pthread_t threads[NUM_THREADS];
    int rc, i = 0;
    sensors.init();

    rc = pthread_create(&threads[0], NULL, pwm_thread, &i);
    rc = pthread_create(&threads[1], NULL, inp_thread, &(++i));

    pthread_exit(NULL);
    return 0;
}
