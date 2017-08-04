#ifndef GPIO_H
#define GPIO_H

// TODO: cull these inclusions
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>

class GPIO {
private:
    int exportGPIO();
    int unexportGPIO();
    int valuefd;          // Value File Descriptor: file location of gpio value file
    int directionfd;      // Direction File Descriptor: file location of gpio direction file
    int exportfd;         // Export File Descriptor: file location of gpio export file
    int unexportfd;       // Unexport File Descriptor: file location of gpio unexport file
    std::string gpionum;  // GPIO Number: number of gpio port of object

public:
    GPIO(std::string gnum);
    ~GPIO();
    int setDirGPIO(std::string dir);
    int setValGPIO(std::string val);
    int getValGPIO(std::string& val);
    std::string getGPIONum();
};

#endif
