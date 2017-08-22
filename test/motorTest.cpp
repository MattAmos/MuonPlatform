#include <time.h>
#include "../src/GPIO.cpp"
/*
   The shield pinouts are
   D12 MOTORLATCH
   D11 PMW motor 1
   D10 Servo 1
   D9  Servo 2
   D8  MOTORDATA

   D7  MOTORENABLE
   D6  PWM motor 4
   D5  PWM motor 3
   D4  MOTORCLK
   D3  PWM motor 2

   The motor states (forward, backward, brake, release) are encoded using the
   MOTOR_ latch pins.  This saves four gpios.
*/
typedef unsigned char uint8_t;
#define BIT(bit) (1 << (bit))

// Define pins to drive shield pins
#define MOTORLATCH 14
#define MOTORCLK 24
#define MOTORENABLE 25
#define MOTORDATA 15

#define MOTOR_1_PWM 7

#define MOTOR1_A 2
#define MOTOR1_B 3

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

static uint8_t latch_state;

GPIO latch("14"), clk("24"), enable("25"), data("15"), pwm("7");

void latch_tx() {
    unsigned char i;

    latch.setValGPIO("0");

    data.setValGPIO("0");

    for (i = 0; i < 8; i++) {
        usleep(10);
        clk.setValGPIO("0");

        if (latch_state & BIT(7 - i)) {
            data.setValGPIO("1");
        }
        else {
            data.setValGPIO("0");
        }

        usleep(10);

        clk.setValGPIO("1");
    }
    latch.setValGPIO("1");
}

void init() {
    latch_state = 0;
    latch_tx();
    enable.setValGPIO("0");
}

void DCMotorInit(uint8_t num) {
    switch (num) {
        case 1: latch_state &= ~BIT(MOTOR1_A) & ~BIT(MOTOR1_B); break;
        default: return;
    }
    latch_tx();
    printf("Latch=%08X\n", latch_state);
}

void DCMotorRun(uint8_t motornum, uint8_t cmd) {
    uint8_t a, b;

    switch (motornum) {
        case 1:
            a = MOTOR1_A;
            b = MOTOR1_B;
            break;
        default: return;
    }

    switch (cmd) {
        case FORWARD:
            latch_state |= BIT(a);
            latch_state &= ~BIT(b);
            break;
        case BACKWARD:
            latch_state &= ~BIT(a);
            latch_state |= BIT(b);
            break;
        case RELEASE:
            latch_state &= ~BIT(a);
            latch_state &= ~BIT(b);
            break;
        default: return;
    }

    latch_tx();

    printf("Latch=%08X\n", latch_state);
}

int main(int argc, char** argv) {
    int i;

    latch.setDirGPIO("out");
    clk.setDirGPIO("out");
    enable.setDirGPIO("out");
    data.setDirGPIO("out");
    pwm.setDirGPIO("out");

    pwm.setValGPIO("1");
    usleep(128);
    pwm.setValGPIO("0");
    usleep(255 - 128);

    init();

    DCMotorRun(1, FORWARD);

    // for (i = 60; i < 160; i += 20) {
    //     gpioPWM(pwm, i);

    //     DCMotorRun(1, FORWARD, latch, data, clk);

    //     sleep(2);

    //     DCMotorRun(1, RELEASE, latch, data, clk);

    //     sleep(2);

    //     gpioPWM(pwm, 220 - i);

    //     DCMotorRun(1, BACKWARD, latch, data, clk);

    //     sleep(2);

    //     DCMotorRun(1, RELEASE, latch, data, clk);
    //     sleep(2);
    // }
    pwm.setValGPIO("0");
    DCMotorRun(1, RELEASE);

    return 0;
}
