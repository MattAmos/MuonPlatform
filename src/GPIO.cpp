/*
 *  Project Muon - GPIO Interface Class
 *
 *  Code developed from github.com/halherta/RaspberryPi-GPIOClass-v2
 *  Modifications made by Matthew Amos
 *  Last modified: 16/08/17
 */

#include "GPIO.h"

// TODO: Convert to C++ style file operations

GPIO::GPIO(std::string gnum) : valuefd(-1), directionfd(-1), exportfd(-1), unexportfd(-1), gpionum(gnum) {
    this->unexportGPIO();
    this->exportGPIO();
}

GPIO::~GPIO() {
    this->unexportGPIO();
}

const int GPIO::exportGPIO() {
    int statusValue = -1;
    this->exportfd = statusValue = open(EXPORT_DIR.c_str(), O_WRONLY | O_SYNC);
    statusValue                  = write(this->exportfd, (this->gpionum).c_str(), (this->gpionum).length());
    statusValue                  = close(this->exportfd);
    return statusValue;
}

const int GPIO::unexportGPIO() {
    int statusValue  = -1;
    this->unexportfd = statusValue = open(UNEXPORT_DIR.c_str(), O_WRONLY | O_SYNC);
    statusValue                    = write(this->unexportfd, (this->gpionum).c_str(), (this->gpionum).length());
    statusValue                    = close(this->unexportfd);
    return statusValue;
}

const int GPIO::setDirGPIO(std::string dir) {
    int statusValue       = -1;
    std::string dirString = GPIO_DIR + this->gpionum + "/direction";
    this->directionfd = statusValue = open(dirString.c_str(), O_WRONLY | O_SYNC);
    if (statusValue < 0) {
        perror(("Could not open GPIO direction device (" + this->gpionum + ")").c_str());
        exit(-1);
    }

    if (dir != "in" && dir != "out") {
        perror(("Invalid direction value. Should be \"in\" or \"out\", received \"" + dir + "\"\n").c_str());
        exit(-1);
    }
    statusValue = write(this->directionfd, dir.c_str(), dir.length());
    if (statusValue < 0) {
        perror(("Could not write to GPIO direction device (" + this->gpionum + ")").c_str());
        exit(-1);
    }

    statusValue = close(this->directionfd);
    if (statusValue < 0) {
        perror(("Could not close GPIO direction device (" + this->gpionum + ")").c_str());
        exit(-1);
    }
    return statusValue;
}

const int GPIO::setValGPIO(std::string val) {
    pthread_mutex_lock(&mutex);
    int statusValue       = -1;
    std::string valString = GPIO_DIR + this->gpionum + "/value";
    this->valuefd = statusValue = open(valString.c_str(), O_WRONLY | O_SYNC);
    if (statusValue < 0) {
        perror(("Could not open GPIO value device (" + this->gpionum + ")").c_str());
        pthread_mutex_unlock(&mutex);
	exit(-1);
    }
    if (val != "1" && val != "0") {
        perror(("Invalid value. Should be \"0\" or \"1\", received \"" + val + "\"\n").c_str());
        pthread_mutex_unlock(&mutex);   
	exit(-1);
    }
    statusValue = write(this->valuefd, val.c_str(), val.length());
    if (statusValue < 0) {
        perror(("Could not write to GPIO value device (" + this->gpionum + ")").c_str());
        pthread_mutex_unlock(&mutex);
	exit(-1);
    }
    statusValue = close(this->valuefd);
    if (statusValue < 0) {
        perror(("Could not close GPIO value device (" + this->gpionum + ")").c_str());
     	pthread_mutex_unlock(&mutex);   
	exit(-1);
    }
    pthread_mutex_unlock(&mutex);
    return statusValue;
}

const int GPIO::getValGPIO(std::string& val) {
    int statusValue       = -1;
    std::string valString = GPIO_DIR + this->gpionum + "/value";
    char buff[5];
    this->valuefd = statusValue = open(valString.c_str(), O_RDONLY | O_SYNC);
    if (statusValue < 0) {
        perror(("Could not open GPIO value device (" + this->gpionum + ")").c_str());
        exit(-1);
    }
    statusValue = read(this->valuefd, &buff, 1);
    if (statusValue < 0) {
        perror(("Could not read GPIO value device (" + this->gpionum + ")").c_str());
        exit(-1);
    }
    buff[1] = '\0';
    val     = std::string(buff);
    if (val != "1" && val != "0") {
        perror(("Invalid value read from file. Should be \"0\" or \"1\", read \"" + val + "\"\n").c_str());
        exit(-1);
    }
    statusValue = close(this->valuefd);
    if (statusValue < 0) {
        perror(("Could not close GPIO value device (" + this->gpionum + ")").c_str());
        exit(-1);
    }
    return statusValue;
}

void GPIO::setPwmTime(int time) {
    pthread_mutex_lock(&mutex);
    pwmTime = time;
    pthread_mutex_unlock(&mutex);
}

int GPIO::getPwmTime() {
    return pwmTime;
}

void GPIO::incPwmTime(int timeInc) {
    pthread_mutex_lock(&mutex);
    pwmTime += timeInc;
    if (pwmTime < pwmMin) {
        pwmTime = pwmMin;
    }
    if (pwmTime > pwmMax) {
        pwmTime = pwmMax;
    }
    pthread_mutex_unlock(&mutex);
}

void GPIO::setPWMRange(int pwmMinTime, int pwmMaxTime) {
    pthread_mutex_lock(&mutex);
    if (pwmMinTime >= pwmMaxTime) {
        std::cout << "PWM range invalid. Exiting..." << std::endl;
        exit(-1);
    }
    pwmMin  = pwmMinTime;
    pwmMax  = pwmMaxTime;
    pwmTime = (pwmMin + pwmMax) / 2.0;
    pthread_mutex_unlock(&mutex);
}

std::string GPIO::getGPIONum() {
    return gpionum;
}
