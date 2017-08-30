/*
 *  Project Muon - Main Runner Class
 *
 *  Last modified: 16/08/17
 */

#include <eigen3/Eigen/Core>
//#include <opencv2/opencv.hpp>
//#include "../include/opencv2/highgui/highgui.hpp"
//#include "../include/opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <cmath>
#include <cstring>
#include <fstream>

#include "GPIO.cpp"
#include "I2C.cpp"

#ifndef RUNNER
#define RUNNER

// Store all config values and other important global variables here

// Networking config parameters
std::string IMG_DIR = "~/images/";   // Image directory to copy into
std::string USER    = "matthew";     // Host username to copy into
std::string HOST    = "10.0.0.117";  // Host IP to copy over network

#endif
