/*
 *  Project Muon - Main Runner Class
 *
 *  Last modified: 30/08/17
 */

#ifndef RUNNER_H
#define RUNNER_H

#define NUM_THREADS 5

#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>
#include "../include/opencv2/highgui/highgui.hpp"
#include "../include/opencv2/imgproc/imgproc.hpp"
#include "../include/opencv2/objdetect/objdetect.hpp"

#include <curses.h>
#include <pthread.h>
#include <time.h>
#include <wiringPi.h>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstring>

#include "../include/joystick/joystick.cc"
#include "GPIO.cpp"
#include "Gyrometer.cpp"
#include "I2C.cpp"
#include "PinMap.h"
#include "Ranger.cpp"
#include "Sonic.cpp"

enum Movement { DC_FRWD, DC_BACK, DC_STOP };

// Store all config values and other important global variables here
struct Sensors {
    Movement move;
    GPIO dc_1  = GPIO(std::to_string(DC_1));
    GPIO dc_2  = GPIO(std::to_string(DC_2));
    GPIO servo = GPIO(std::to_string(SERVO));
    GPIO pir   = GPIO(std::to_string(PIR));

    Sonic sonic_back  = Sonic(ECHO1, TRIG1);
    Sonic sonic_left  = Sonic(ECHO2, TRIG2);
    Sonic sonic_right = Sonic(ECHO3, TRIG3);
    Sonic sonic_front = Sonic(ECHO4, TRIG4);

    Gyrometer gyro = Gyrometer();
    void init() {
        servo.setPWMRange(SERVO_PWM_MIN, SERVO_PWM_MAX);
        dc_1.setDirGPIO("out");
        dc_2.setDirGPIO("out");
        servo.setDirGPIO("out");
        pir.setDirGPIO("in");

        sonic_back.setup();
        sonic_left.setup();
        sonic_front.setup();
        sonic_right.setup();

        move = DC_STOP;
        dc_move(DC_STOP);
    }
    void dc_move(Movement m) {
        switch (move) {
            case DC_FRWD:
                dc_2.setValGPIO("0");
                dc_1.setValGPIO("1");
                break;
            case DC_BACK:
                dc_1.setValGPIO("0");
                dc_2.setValGPIO("1");
                break;
            case DC_STOP:
                dc_1.setValGPIO("0");
                dc_2.setValGPIO("0");
                break;
        }
    }
    void destroy() {
        dc_1.~GPIO();
        dc_2.~GPIO();
    };
};

Sensors sensors;
int currCh;

Joystick joystick("/dev/input/js0");
JoystickEvent event;
int mid_t = (SERVO_PWM_MAX + SERVO_PWM_MIN) / 2.0;

bool contFlag = false;

// Networking config parameters
std::string IMG_DIR = "~/images/";   // Image directory to copy into
std::string USER    = "matthew";     // Host username to copy into
std::string HOST    = "10.0.0.117";  // Host IP to copy over network

#endif
