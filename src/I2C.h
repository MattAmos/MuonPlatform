#ifndef I2C_H
#define I2C_H

#include <iostream>
#include <string>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

class I2C {
private:
    const std::string path = "/dev/";  // Path to interface with I2C devices
    std::string name;                  // Name of I2C device file
    int fd;                            // File descriptor of I2C device file
    int addr;                          // Address of I2C device

public:
    I2C(std::string path, int addrIn);
    ~I2C();
    const int openI2C(std::string busPath);
    const int closeI2C();

    const int readI2C(int numBytes, char buffer[]);
    const int writeI2C(int numBytes, char buffer[]);
};

#endif
