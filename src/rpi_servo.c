// rpi_servo.c
// Created on: 2024-12-30
// Author: Sebastien Cabana

#include "../lib/rpi_servo.h"

Servo ramp_servo;
Servo switch_servo;

int Servo_Init() {
    // Initialize struct values
    ramp_servo.is_open = 0;
    ramp_servo.servo_pin = RAMP_SERVO_PIN;
    ramp_servo.close_angle = RAMP_SERVO_CLOSE;
    ramp_servo.open_angle = RAMP_SERVO_OPEN;
    switch_servo.is_open = 0;
    switch_servo.servo_pin = SWITCH_SERVO_PIN;
    switch_servo.close_angle = SWITCH_SERVO_CLOSE;
    switch_servo.open_angle = SWITCH_SERVO_OPEN;

    // Initialize ramp servo
    pinMode(ramp_servo.servo_pin, OUTPUT);
    if(softPwmCreate(ramp_servo.servo_pin, ramp_servo.close_angle, SERVO_MAX_PULSE_WIDTH) != 0) {
        fprintf(stdout, "Unable to start ramp servo: %s\n", strerror(errno));
        return 1;
    }

    // Initialize switch servo
    pinMode(switch_servo.servo_pin, OUTPUT);
    if(softPwmCreate(switch_servo.servo_pin, switch_servo.close_angle, SERVO_MAX_PULSE_WIDTH) != 0) {
        fprintf(stdout, "Unable to start ramp servo: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

void Toggle_Servo(Servo* s) {
    // Use continuous mode if angle is larger than 180 degrees
    int use_c_mode = 0;
    if(s->open_angle - s->close_angle > 180 || s->close_angle - s->open_angle > 180)
        use_c_mode = 1;

    if (s->is_open) {
        // Set the servo to the closed position
        if(use_c_mode)
            Continuous_Mode(s, s->close_angle);
        else
            softPwmWrite(s->servo_pin, s->close_angle);
        s->is_open = 0;
    } else {
        // Set the servo to the open position
        if(use_c_mode)
            Continuous_Mode(s, s->open_angle);
        else
            softPwmWrite(s->servo_pin, s->open_angle);        
        is_servo_open = 1;
    }
}

void Continuous_Mode(Servo* s, int target_angle) {
    int current_angle = s->is_open ? s->open_angle : s->close_angle;
    int step = (target_angle > current_angle) ? 1 : -1;
    
    for (int angle = current_angle; angle != target_angle; angle += step) {
        softPwmWrite(s->servo_pin, Map_Angle_To_Pulse(angle));
        delay(TURN_TIME_PER_DEGREE);
    }
}


int Map_Angle_To_Pulse(int angle) {
    return SERVO_MIN_PULSE_WIDTH + (int)((angle / 180.0) * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH));
}

int Map_Angle_To_Delay(int angle) {
    if(angle < 0)
        angle *= -1;
    return angle * TURN_TIME_PER_DEGREE;
}