/*
 * boat.h
 * Created on: 2024-12-22
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the hardware-related constants and configurations 
 * for the BOAT robot. These constants are specific to the BOAT robot's hardware 
 * setup, including pin assignments and motor/servo settings.
 * 
 * Source : https://www.electronicwings.com/raspberry-pi/raspberry-pi-pwm-generation-using-python-and-c
 * 
 */

#ifndef BOAT_CONTROL_H
#define BOAT_CONTROL_H

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>

// Enums and Structs
typedef struct {
    char side;              // 'R' or 'L'
    float speed;            // Current speed as percentage (0.0 to 1.0)
    int multiplier;         // Current speed as multiplier
    int motor_pin;          // GPIO Pin for the motor pwm
    int power_pin;          // GPIO Pin to provide 5V power to the motor
} Brushless;

// Pin Definitions
#define LEFT_MOTOR_PWM_PIN    23      // GPIO Pin for the left motor PWM
#define LEFT_POWER_GPIO_PIN   24      // GPIO Pin to provide 5V power to the left motor
#define RIGHT_MOTOR_PWM_PIN   26      // GPIO Pin for the right motor PWM
#define RIGHT_POWER_GPIO_PIN  22      // GPIO Pin to provide 5V power to the right motor

// Global Constants
#define BOAT_ID         6       // Unique ID to each Arduino : RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
#define MAX_SPEED       5       // Maximum speed multiplier for the robot

// PWM Constants
#define PWM_RANGE       1024    // Range for PWM (0-1023)
#define PWM_CLOCK_DIV   384     // Clock divisor for 50 Hz frequency (19.2 MHz / (384 * 1024) = 50 Hz)
#define DUTY_CYCLE_STOP 51      // 5% duty cycle (5% of 1024)
#define DUTY_CYCLE_MAX  102     // 10% duty cycle (10% of 1024)

// Global Variables
Brushless left_motor;
Brushless right_motor;
extern char* message;

// Function Prototypes
int PWM_Init();
void Control_Boat();
void Motor_Startup(Brushless*);
void Set_Motor_Speed(Brushless*);
void Reset_Motors();
void Reset_Motor(Brushless*);
int Get_Motor_Multiplier(Brushless*);

#endif 