// boat_control.c
// Created on: 2024-12-22
// Author: Sebastien Cabana

#include "../lib/boat_control.h"

Brushless left_motor;
Brushless right_motor;

int PWM_Init() {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Unable to initialize pigpio: %s\n", strerror(errno));
        return 1;
    }

    // Set GPIO pin modes
    gpioSetMode(LEFT_MOTOR_PWM_PIN, PI_OUTPUT);
    gpioSetMode(RIGHT_MOTOR_PWM_PIN, PI_OUTPUT);
    gpioSetMode(LEFT_POWER_GPIO_PIN, PI_OUTPUT);
    gpioSetMode(RIGHT_POWER_GPIO_PIN, PI_OUTPUT);

    // Configure PWM range
    gpioSetPWMrange(LEFT_MOTOR_PWM_PIN, PWM_RANGE);
    gpioSetPWMrange(RIGHT_MOTOR_PWM_PIN, PWM_RANGE);

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
    gpioWrite(motor->power_pin, 1);

    // Send a 5% duty cycle signal to initialize the motor
    gpioPWM(motor->motor_pin, DUTY_CYCLE_STOP);

    // Wait for motor initialization
    gpioDelay(INIT_DELAY * 1000); // Delay in microseconds
}

void Set_Motor_Speed(Brushless* motor) {
    // Save previous speed
    float prev_speed = motor->speed;

    // Convert multiplier to percentage
    motor->speed = Get_Motor_Multiplier(motor) / (float)MAX_SPEED;

    // Calculate the previous and new PWM value based on the speed percentage
    int prev_pwmValue = DUTY_CYCLE_STOP + (int)((DUTY_CYCLE_MAX - DUTY_CYCLE_STOP) * prev_speed);
    int pwmValue = DUTY_CYCLE_STOP + (int)((DUTY_CYCLE_MAX - DUTY_CYCLE_STOP) * motor->speed);

    // Slowly increase the PWM value to the motor pin
    for (int i = prev_pwmValue; i < pwmValue; i++) {
        gpioPWM(motor->motor_pin, i);
        gpioDelay(INCREASE_DELAY * 1000); // Delay in microseconds
    }
}

void Reset_Motors() {
    Reset_Motor(&left_motor);
    Reset_Motor(&right_motor);
}

void Reset_Motor(Brushless* motor) {
    gpioWrite(motor->power_pin, 0);
    gpioDelay((INIT_DELAY / 2) * 1000); // Delay in microseconds
    Motor_Startup(motor);
}

int Get_Motor_Multiplier(Brushless* motor) {
    switch (motor->side) {
        case 'R':   motor->multiplier = message[6] - '0'; break;
        case 'L':   motor->multiplier = message[3] - '0'; break;
        default:    printf("Error. Wrong side in Get_Motor_Multiplier.\n"); return -1;
    }
    return motor->multiplier;
}
