/*
 *  Project Muon - GPIO Interface Class
 *
 *  Code developed from github.com/halherta/RaspberryPi-GPIOClass-v2
 *  Modifications made by Matthew Amos
 *  Last modified: 16/08/17
 */

#include "GPIO.h"

GPIO::GPIO(std::string gpioNumIn)
    : valueFD(NULL), directionFD(NULL), exportFD(NULL), unexportFD(NULL), gpioNum(gpioNumIn) {
    this->unexportGPIO();
    this->exportGPIO();

    std::string dirString = GPIO_DIR + this->gpioNum + "/direction";
    std::string valString = GPIO_DIR + this->gpioNum + "/value";
}

GPIO::~GPIO() {
    this->unexportGPIO();
}

const int GPIO::exportGPIO() {
    std::size_t bytesWritten = -1;
    this->exportFD           = fopen(EXPORT_DIR.c_str(), "w");
    if (!this->exportFD) {
        std::cerr << "Error: Could not open export file (GPIO " << this->gpioNum << ")" << std::endl;
        exit(-1);
    }
    bytesWritten = fwrite((this->gpioNum).c_str(), (this->gpioNum).length(), 1, this->exportFD);
    if (bytesWritten != (this->gpioNum).length()) {
        std::cerr << "Warning: Could not correctly write to export file (GPIO " << this->gpioNum << ")" << std::endl;
    }
    return fclose(this->exportFD);
}

const int GPIO::unexportGPIO() {
    int bytesWritten = -1;
    this->unexportFD = fopen(UNEXPORT_DIR.c_str(), "w");
    if (!this->unexportFD) {
        std::cerr << "Error: Could not open unexport file (GPIO " << this->gpioNum << ")" << std::endl;
        exit(-1);
    }
    bytesWritten = fwrite((this->gpioNum).c_str(), (this->gpioNum).length(), 1, this->unexportFD);
    if (bytesWritten != (this->gpioNum).length()) {
        std::cerr << "Warning: Could not correctly write to unexport file (GPIO " << this->gpioNum << ")" << std::endl;
    }
    return fclose(this->unexportFD);
}

const int GPIO::setDirGPIO(std::string direction) {
    this->directionFD = fopen(this->dirString.c_str(), "w");
    if (!this->directionFD) {
        std::cerr << "Error: Could not open direction file (GPIO " << this->gpioNum << ")" << std::endl;
        exit(-1);
    }
    std::size_t bytesWritten = -1;
    if (direction != "in" && direction != "out") {
        std::cerr << "Warning: Direction specification should be \"in\" or \"out\", received \"" << direction << "\""
                  << std::endl;
    }
    else {
        bytesWritten = fwrite(direction.c_str(), direction.length(), 1, this->directionFD);
        if (bytesWritten != direction.length()) {
            std::cerr << "Warning: Could not correctly write to direction file (GPIO " << this->gpioNum << ")"
                      << std::endl;
        }
    }
    return fclose(this->directionFD);
}

const int GPIO::setValGPIO(std::string value) {
    this->valueFD = fopen(this->valString.c_str(), "w");
    if (!this->valueFD) {
        std::cerr << "Error: Could not open value file (GPIO " << this->gpioNum << ")" << std::endl;
        exit(-1);
    }
    std::size_t bytesWritten = -1;
    if (value != "1" && value != "0") {
        std::cerr << "Warning: Value specification should be \"0\" or \"1\", received \"" << value << "\"" << std::endl;
    }
    else {
        bytesWritten = fwrite(value.c_str(), value.length(), 1, this->valueFD);
        if (bytesWritten != value.length()) {
            std::cerr << "Warning: Could not correctly write to value file (GPIO " << this->gpioNum << ")" << std::endl;
        }
    }
    return fclose(this->valueFD);
}

const int GPIO::getValGPIO(std::string& value) {
    this->valueFD = fopen(this->valString.c_str(), "r");
    if (!this->valueFD) {
        std::cerr << "Error: Could not write to value file (GPIO " << this->gpioNum << ")" << std::endl;
        exit(-1);
    }
    char buff[2];
    size_t bytesRead = fread(&buff, 1, 1, this->valueFD);
    if (bytesRead != 1) {
        std::cerr << "Warning: Could not correctly read from value file (GPIO " << this->gpioNum << ")" << std::endl;
    }
    buff[1] = '\0';
    value   = std::string(buff);
    if (value != "1" && value != "0") {
        std::cerr << "Warning: Value specification should be \"0\" or \"1\", received \"" << value << "\"" << std::endl;
    }
    return fclose(this->valueFD);
}

std::string GPIO::getGPIONum() {
    return gpioNum;
}
