#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>
#include "../include/opencv2/highgui/highgui.hpp"
#include "../include/opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <cmath>
#include <cstring>
#include <fstream>

#include "GPIO.cpp"

#ifndef RUNNER
#define RUNNER

enum ServoID {
    FL_DRIVE = 0,  // Front-left servo  for motion forwards/backwards
    FL_YAW   = 1,  //                   for turning wheel axis
    FR_DRIVE = 2,  // Front-right servo for motion forwards/backwards
    FR_YAW   = 3,  //                   for turning wheel axis
    BL_DRIVE = 4,  // Back-left servo   for motion forwards/backwards
    BL_YAW   = 5,  //                   for turning wheel axis
    BR_DRIVE = 6,  // Back-right servo  for motion forwards/backwards
    BR_YAW   = 7   //                   for turning wheel axis
};

struct Servo {
    ServoID id;  // Details role of servo
    GPIO* gpio;  // Points to gpio port servo is on
};

// Store all config values and other important global variables here

// Networking config parameters
std::string IMG_DIR = "~/images/";  // Image directory to copy into
std::string USER    = "matthew";    // Host username to copy into
std::string HOST    = "10.1.1.13";  // Host IP to copy over network

#endif
