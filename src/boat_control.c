// communication.c
// Created on: 2024-12-22
// Author: Sebastien Cabana

#include "../lib/boat_control.h"

void PWM_Init() {

    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }
}