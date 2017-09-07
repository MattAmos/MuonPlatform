#ifndef GPIO_H
#define GPIO_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

class GPIO {
private:
    const std::string EXPORT_DIR   = "/sys/class/gpio/export";
    const std::string UNEXPORT_DIR = "/sys/class/gpio/unexport";
    const std::string GPIO_DIR     = "/sys/class/gpio/gpio";

    const int exportGPIO();
    const int unexportGPIO();
    int valuefd;      // Value File Descriptor: file location of gpio value file
    int directionfd;  // Direction File Descriptor: file location of gpio direction file
    int exportfd;     // Export File Descriptor: file location of gpio export file
    int unexportfd;   // Unexport File Descriptor: file location of gpio unexport file
    int pwmTime, pwmMin, pwmMax;
    std::string gpionum;  // GPIO Number: number of gpio port of object

public:
    GPIO(std::string gnum);
    ~GPIO();
    const int setDirGPIO(const std::string dir);
    const int setValGPIO(const std::string val);
    const int getValGPIO(std::string& val);
    void setPwmTime(int time);
    int getPwmTime();
    void incPwmTime(int timeInc);
    void setPWMRange(int pwmMinTime, int pwmMaxTime);
    std::string getGPIONum();
};

#endif
