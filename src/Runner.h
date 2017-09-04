/*
 *  Project Muon - Main Runner Class
 *
 *  Last modified: 30/08/17
 */

#ifndef RUNNER_H
#define RUNNER_H

#define NUM_THREADS 2

#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>
#include "../include/opencv2/highgui/highgui.hpp"
#include "../include/opencv2/imgproc/imgproc.hpp"

#include <pthread.h>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstring>

#include "GPIO.cpp"
#include "I2C.cpp"
#include "PinMap.h"

// Store all config values and other important global variables here
struct Sensors {
    // GPIO dcA   = GPIO(std::to_string(DC_A));
    // GPIO dcB   = GPIO(std::to_string(DC_B));
    // GPIO servo = GPIO(std::to_string(SERVO));
    // GPIO pir   = GPIO(std::to_string(PIR));
    void init() {
        // servo.setPWMRange(SERVO_PWM_MIN, SERVO_PWM_MAX);
    }
    void drivePWM(){

    };
    void destroy() {
        // dcA.~GPIO();
        // dcB.~GPIO();
        // servo.~GPIO();
        // pir.~GPIO();
    };
};

Sensors sensors;

// Networking config parameters
std::string IMG_DIR = "~/images/";   // Image directory to copy into
std::string USER    = "matthew";     // Host username to copy into
std::string HOST    = "10.0.0.117";  // Host IP to copy over network

#endif
