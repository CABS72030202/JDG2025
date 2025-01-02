// communication.c
// Created on: 2024-12-22
// Author: Sebastien Cabana

#include "../lib/boat_control.h"

Brushless left_wheel;
Brushless right_wheel;
Brushless left_propeller;
Brushless right_propeller;
const int BOAT_SPEEDS[MAX_SPEED] = {60, 70, 80};
char control_type;

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
    pinMode(WHEEL_POWER_GPIO_PIN, OUTPUT);
    pinMode(PROPELLER_POWER_GPIO_PIN, OUTPUT);

    // Configure PWM clock and range for 50 Hz frequency
    pwmSetMode(PWM_MODE_MS);        // Use Mark:Space mode for stable frequency
    pwmSetClock(PWM_CLOCK_DIV);     // Set clock divisor
    pwmSetRange(PWM_RANGE);         // Set range

    // Initialize motor structs
    Brushless_Init(&left_wheel, 'L', 'W', 0.0, 0, LEFT_MOTOR_PWM_PIN, WHEEL_POWER_GPIO_PIN);
    Brushless_Init(&right_wheel, 'R', 'W', 0.0, 0, RIGHT_MOTOR_PWM_PIN, WHEEL_POWER_GPIO_PIN);
    Brushless_Init(&left_propeller, 'L', 'P', 0.0, 0, LEFT_MOTOR_PWM_PIN, PROPELLER_POWER_GPIO_PIN);
    Brushless_Init(&right_propeller, 'R', 'P', 0.0, 0, RIGHT_MOTOR_PWM_PIN, PROPELLER_POWER_GPIO_PIN);

    // Control the propellers first
    Toggle_Brushless();

    return 0;
}

void Brushless_Init(Brushless* motor, char side, char type, float speed, int multiplier, int motor_pin, int power_pin) {
    motor->side = side;
    motor->type = type;
    motor->speed = speed;
    motor->multiplier = multiplier;
    motor->motor_pin = motor_pin;
    motor->power_pin = power_pin;
    Motor_Startup(motor);
}

void Control_Boat() {
    switch(control_type) {
        case 'p':
        case 'P':
            Set_Motor_Speed(&left_propeller);
            Set_Motor_Speed(&right_propeller);
            break;
        case 'w':
        case 'W':
            Set_Motor_Speed(&left_wheel);
            Set_Motor_Speed(&right_wheel);
            break;
        default:
            printf("Invalid type in Control_Boat().\n");
            break;
    }
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

    // Get speed from multiplier
    if(Get_Motor_Multiplier(motor) <= 0) {
        pwmWrite(motor->motor_pin, DUTY_CYCLE_STOP);
        return;
    }
    motor->speed = BOAT_SPEEDS[motor->multiplier - 1];
    
    // Calculate the previous and new PWM value based on the speed percentage
    int prev_pwmValue = DUTY_CYCLE_STOP + (int)((DUTY_CYCLE_MAX - DUTY_CYCLE_STOP) * prev_speed);
    int pwmValue = DUTY_CYCLE_STOP + (int)((DUTY_CYCLE_MAX - DUTY_CYCLE_STOP) * motor->speed);

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
    switch(control_type) {
        case 'p':
        case 'P':
            Reset_Motor(&left_propeller);
            break;
        case 'w':
        case 'W':
            Reset_Motor(&left_wheel);
            break;
        default:
            printf("Invalid type in Reset_Motors().\n");
            break;
    }
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

void Toggle_Brushless() {
    switch(control_type) {
        case 'w':
        case 'W':
            digitalWrite(WHEEL_POWER_GPIO_PIN, LOW);
            Motor_Startup(&left_propeller);
            Motor_Startup(&right_propeller);
            control_type = 'P';
            break;
        default:
        case 'p':
        case 'P':
            digitalWrite(PROPELLER_POWER_GPIO_PIN, LOW);
            Motor_Startup(&left_wheel);
            Motor_Startup(&right_wheel);
            control_type = 'W';
            break;
    }
}