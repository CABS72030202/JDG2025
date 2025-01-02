// communication.c
// Created on: 2024-12-22
// Author: Sebastien Cabana

#include "../lib/boat_control.h"

Brushless left_motors;
Brushless right_motors;
const int BOAT_SPEEDS[MAX_SPEED] = {60, 70, 80};

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
    pwmSetMode(PWM_MODE_MS);        // Use Mark:Space mode for stable frequency
    pwmSetClock(PWM_CLOCK_DIV);     // Set clock divisor
    pwmSetRange(PWM_RANGE);         // Set range

    // Initialize motor structs
    Brushless_Init(&left_motors, 'L', 0.0, 0, LEFT_MOTOR_PWM_PIN, LEFT_POWER_GPIO_PIN);
    Brushless_Init(&right_motors, 'R', 0.0, 0, RIGHT_MOTOR_PWM_PIN, RIGHT_POWER_GPIO_PIN);

    return 0;
}

void Brushless_Init(Brushless* motor, char side, float speed, int multiplier, int motor_pin, int power_pin) {
    motor->side = side;
    motor->speed = speed;
    motor->multiplier = multiplier;
    motor->motor_pin = motor_pin;
    motor->power_pin = power_pin;
    Motor_Startup(motor);
}

void Control_Boat() {
    Set_Motor_Speed(&left_motors);
    Set_Motor_Speed(&right_motors);
}

void Motor_Startup(Brushless* motor) {
    // Provide power to the motor
    digitalWrite(motor->power_pin, HIGH);

    // Send a 5% duty cycle signal to initialize the motor
    pwmWrite(motor->motor_pin, DUTY_CYCLE_STOP);

    // Wait for motor initialization
    delay(INIT_DELAY);
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
    for(int i = prev_pwmValue; i < pwmValue; i++) {
        pwmWrite(motor->motor_pin, i);
        delay(INCREASE_DELAY);
    }
    // Slowly increase the PWM value to the motor pin 
    if(prev_pwmValue < pwmValue)
        for(int i = prev_pwmValue; i < pwmValue; i++) {
            if(i >= DUTY_CYCLE_LIMIT)
                break;
            pwmWrite(motor->motor_pin, i);
            delay(INCREASE_DELAY);
        }
    // Slowly decrease the PWM value to the motor pin
    if(prev_pwmValue > pwmValue)
        for(int i = prev_pwmValue; i > pwmValue; i--) {
            if(i < DUTY_CYCLE_STOP)
                break;
            pwmWrite(motor->motor_pin, i);
            delay(INCREASE_DELAY);
        }
}

void Reset_Motors() {
    Reset_Motor(&left_motors);
    Reset_Motor(&right_motors);
}

void Reset_Motor(Brushless* motor) {
    digitalWrite(motor->power_pin, LOW);
    delay(0.5 * INIT_DELAY);
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