#ifndef I2C_H
#define I2C_H

#include <stdio.h>

#include <iostream>
#include <string>

#include <fcntl.h>          //Needed for I2C port
#include <linux/i2c-dev.h>  //Needed for I2C port
#include <sys/ioctl.h>      //Needed for I2C port
#include <unistd.h>         //Needed for I2C port

class I2C {
private:
    const std::string path = "/dev/";
    std::string name;
    int fd;
    int addr;

public:
    I2C(std::string path, int addrIn);
    ~I2C();
    const int openI2C(std::string busPath);
    const int closeI2C();

    const int readI2C(int numBytes, char buffer[]);
    const int writeI2C(int numBytes, char buffer[]);
};

#endif
