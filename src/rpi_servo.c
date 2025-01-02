// rpi_servo.c
// Created on: 2024-12-30
// Author: Sebastien Cabana

#include "../lib/rpi_servo.h"

int Servo_Init() {
    pinMode(SERVO_PIN, OUTPUT);
    if(softPwmCreate(SERVO_PIN, SERVO_CLOSE_ANGLE, SERVO_MAX_PULSE_WIDTH) != 0) {
        fprintf(stdout, "Unable to start servo: %s\n", strerror(errno));
        return 1;
    }
    is_servo_open = 0;
    return 0;
}

void Toggle_Servo() {
    if (is_servo_open) {
        // Set the servo to the closed position
        softPwmWrite(SERVO_PIN, SERVO_CLOSE_ANGLE);
        is_servo_open = 0;
    } else {
        // Set the servo to the open position
        softPwmWrite(SERVO_PIN, SERVO_OPEN_ANGLE);
        is_servo_open = 1;
    }
}