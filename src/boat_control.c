// communication.c
// Created on: 2024-12-22
// Author: Sebastien Cabana

#include "../lib/boat_control.h"

int PWM_Init() {

    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    // Set PWM pin mode
    pinMode(LEFT_MOTOR_PWM_PIN, PWM_OUTPUT);
    pinMode(RIGHT_MOTOR_PWM_PIN, PWM_OUTPUT);

    // Set GPIO pin for motor power
    pinMode(LEFT_POWER_GPIO_PIN, OUTPUT);
    pinMode(RIGHT_POWER_GPIO_PIN, OUTPUT);

    // Configure PWM clock and range for 50 Hz frequency
    pwmSetMode(PWM_MODE_MS);     // Use Mark:Space mode for stable frequency
    pwmSetClock(PWM_CLOCK_DIV); // Set clock divisor
    pwmSetRange(PWM_RANGE);     // Set range

    // Initialize motor structs
    left_motor.side = 'L';
    left_motor.speed = 0.0;
    left_motor.multiplier = 0;
    left_motor.motor_pin = LEFT_MOTOR_PWM_PIN;
    left_motor.power_pin = LEFT_POWER_GPIO_PIN;
    right_motor.side = 'R';
    right_motor.speed = 0.0;
    right_motor.multiplier = 0;
    right_motor.motor_pin = RIGHT_MOTOR_PWM_PIN;
    right_motor.power_pin = RIGHT_POWER_GPIO_PIN;

    // Start motors
    Motor_Startup(&left_motor);
    Motor_Startup(&right_motor);

    return 0;
}

void Control_Boat() {
    Set_Motor_Speed(&left_motor);
    Set_Motor_Speed(&right_motor);
}

void Motor_Startup(Brushless* motor) {
    // Provide power to the motor
    digitalWrite(motor->power_pin, HIGH);

    // Send a 5% duty cycle signal to initialize the motor
    pwmWrite(motor->motor_pin, DUTY_CYCLE_STOP);

    // Wait for motor initialization
    delay(2000);
}

void Set_Motor_Speed(Brushless* motor) {
    // Convert multiplier to percentage
    motor->speed = Get_Motor_Multiplier(motor) / (float)MAX_SPEED;

    // Calculate the PWM value based on the speed percentage
    int pwmValue = DUTY_CYCLE_STOP + (int)((DUTY_CYCLE_MAX - DUTY_CYCLE_STOP) * motor->speed);

    // Write the PWM value to the motor pin
    pwmWrite(motor->motor_pin, pwmValue);
}

void Reset_Motors() {
    Reset_Motor(&left_motor);
    Reset_Motor(&right_motor);
}

void Reset_Motor(Brushless* motor) {
    digitalWrite(motor->power_pin, LOW);
    delay(500);
    Motor_Startup(motor);
}

int Get_Motor_Multiplier(Brushless* motor) {
    switch(motor->side) {
        case 'R':   motor->multiplier = message[6] - '0'; break;
        case 'L':   motor->multiplier = message[3] - '0'; break;
        default:    printf("Error. Wrong side in Get_Motor_Multiplier.\n"); return;
    }
    return motor->multiplier;
}