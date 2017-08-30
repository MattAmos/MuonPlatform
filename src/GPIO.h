#ifndef GPIO_H
#define GPIO_H

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>

class GPIO {
private:
    const std::string EXPORT_DIR   = "/sys/class/gpio/export";
    const std::string UNEXPORT_DIR = "/sys/class/gpio/unexport";
    const std::string GPIO_DIR     = "/sys/class/gpio/gpio";

    const int exportGPIO();
    const int unexportGPIO();
    std::FILE* valueFD;      // Value File Descriptor:      file location of gpio value file
    std::FILE* directionFD;  // Direction File Descriptor:  file location of gpio direction file
    std::FILE* exportFD;     // Export File Descriptor:     file location of gpio export file
    std::FILE* unexportFD;   // Unexport File Descriptor:   file location of gpio unexport file

    std::string gpioNum;    // GPIO Number:                number of gpio port of object
    std::string dirString;  // Direction Directory String: path to direction directory file descriptor
    std::string valString;  // Value Directory String:     path to value directory file descriptor

public:
    GPIO(std::string gnum);
    ~GPIO();
    const int setDirGPIO(const std::string dir);
    const int setValGPIO(const std::string val);
    const int getValGPIO(std::string& val);
    std::string getGPIONum();
};

#endif
