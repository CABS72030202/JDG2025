// gpio.c
// Created on: 2024-11-25
// Author: Sebastien Cabana

#include "../lib/gpio.h"

void Initialize_GPIO() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed.\n");
        return;
    }

    // Set pin modes
    gpioSetMode(ARM_OUT_PIN_0, PI_OUTPUT);
    gpioSetMode(ARM_OUT_PIN_1, PI_OUTPUT);
    gpioSetMode(ARM_OUT_PIN_2, PI_OUTPUT);
    gpioSetMode(ARM_OUT_PIN_3, PI_OUTPUT);
    gpioSetMode(ARM_IN_PIN_0, PI_INPUT);
    gpioSetMode(ARM_IN_PIN_1, PI_INPUT);
    gpioSetMode(ARM_IN_PIN_2, PI_INPUT);
    gpioSetMode(ARM_IN_PIN_3, PI_INPUT);
    Write_Arm_DEC(DOWN_ALL);
}

void Write_Arm_BIN(int bit3, int bit2, int bit1, int bit0) {
    gpioWrite(ARM_OUT_PIN_3, bit3);
    gpioWrite(ARM_OUT_PIN_2, bit2);
    gpioWrite(ARM_OUT_PIN_1, bit1);
    gpioWrite(ARM_OUT_PIN_0, bit0);
}

void Write_Arm_DEC(int num) {
    int bit3 = (num >> 3) & 1;
    int bit2 = (num >> 2) & 1;
    int bit1 = (num >> 1) & 1;
    int bit0 = num & 1;
    Write_Arm_BIN(bit3, bit2, bit1, bit0);
}

int Read_Arm_BIN() {
    int bit3 = gpioRead(ARM_IN_PIN_3);
    int bit2 = gpioRead(ARM_IN_PIN_2);
    int bit1 = gpioRead(ARM_IN_PIN_1);
    int bit0 = gpioRead(ARM_IN_PIN_0);
    return Read_Arm_DEC(bit3, bit2, bit1, bit0);
}

int Read_Arm_DEC(int bit3, int bit2, int bit1, int bit0) {
    return ((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0);
}