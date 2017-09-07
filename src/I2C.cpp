#include "I2C.h"

I2C::I2C(int addrIn) {
    I2C::addr = addrIn;
    // Open I2C bus
    I2C::openI2C();
}

I2C::~I2C() {
    I2C::closeI2C();
}

const int I2C::openI2C() {
    if (I2C::fd = open("/dev/i2c-1", O_RDWR) < 0) {
        std::cout << "[I2C] Failed to open I2C bus. Exiting..." << std::endl;
        exit(-1);
    }
    if (ioctl(I2C::fd, I2C_SLAVE, I2C::addr) < 0) {
        std::cout << "[I2C] Failed to acquire bus access/talk to slave at address " << I2C::addr << ". Exiting..."
                  << std::endl;
        exit(-1);
    }
    return 0;
}

const int I2C::closeI2C() {
    if (I2C::fd >= 0) {
        return close(fd);
    }
    return 0;
}

const int I2C::readI2C(int numBytes, char buffer[]) {
    int errNo;
    if ((errNo = read(fd, buffer, numBytes)) != numBytes) {
        std::cout << "[I2C] Failed to read " << numBytes << " from device \"" << name << "\"." << std::endl;
    }
    else {
        std::cout << "[I2C] Data read: " << buffer << std::endl;
    }
    return errNo;
}

const int I2C::writeI2C(int numBytes, char buffer[]) {
    int errNo;
    if ((errNo = write(fd, buffer, numBytes)) != numBytes) {
        std::cout << "[I2C] Failed to write " << numBytes << " from device \"" << name << "\"." << std::endl;
    }
    return errNo;
}
