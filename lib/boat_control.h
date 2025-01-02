/*
 * boat.h
 * Created on: 2024-12-22
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the hardware-related constants and configurations 
 * for the BOAT robot. These constants are specific to the BOAT robot's hardware 
 * setup, including pin assignments and motor settings.
 * 
 * Source: https://www.electronicwings.com/raspberry-pi/raspberry-pi-pwm-generation-using-python-and-c
 */

#ifndef BOAT_CONTROL_H
#define BOAT_CONTROL_H

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include "rpi_servo.h"

// Enums and Structs
typedef struct {
    char side;              // 'R' for Right or 'L' for Left
    char type;              // 'W' for Wheel or 'P' for Propeller
    float speed;            // Current speed as a percentage (0.0 to 1.0)
    int multiplier;         // Current speed as a multiplier
    int motor_pin;          // GPIO Pin for the motor PWM
    int power_pin;          // GPIO Pin to provide 5V power to the motor
} Brushless;

// Pin Definitions
#define LEFT_MOTOR_PWM_PIN          23      // GPIO Pin for the left side PWM
#define WHEEL_POWER_GPIO_PIN        24      // GPIO Pin to provide power to the wheel drive
#define RIGHT_MOTOR_PWM_PIN         26      // GPIO Pin for the right side PWM
#define PROPELLER_POWER_GPIO_PIN    22      // GPIO Pin to provide power to the propeller drive

// Global Constants
#define BOAT_ID             6               // Unique ID for the BOAT robot
#define MAX_SPEED           3               // Maximum speed multiplier for the robot
extern const int BOAT_SPEEDS[MAX_SPEED];    // Preset duty cycle values for brushless motors speed

// PWM Constants
#define PWM_RANGE           1024         // Range for PWM (0-1023)
#define PWM_CLOCK_DIV       384          // Clock divisor for 50 Hz frequency (19.2 MHz / (384 * 1024) = 50 Hz)
#define DUTY_CYCLE_STOP     51           // 5% duty cycle (5% of 1024)
#define DUTY_CYCLE_MAX      102          // 10% duty cycle (10% of 1024)
#define DUTY_CYCLE_LIMIT    93           // Maximum duty cycle accounting for load
#define INIT_DELAY          1500         // Time delay for brushless motor initialization
#define INCREASE_DELAY      1            // Time delay when increasing speed (in ms)

// Global Variables
extern Brushless left_wheel;        // Configuration for the left wheel motor
extern Brushless right_wheel;       // Configuration for the right wheel motor
extern Brushless left_propeller;    // Configuration for the left wheel propeller
extern Brushless right_propeller;   // Configuration for the right wheel propeller
extern char message[];              // Controller command message
extern char control_type;           // Current brushless type ('P' or 'W') control

// Function Prototypes
int PWM_Init();                                                     // Initialize brushless motors control
void Brushless_Init(Brushless*, char, char, float, int, int, int);  // Initialize brushless struct 
void Control_Boat();                                                // Main control logic for the boat
void Motor_Startup(Brushless*);                                     // Startup routine for a motor
void Set_Motor_Speed(Brushless*);                                   // Set the speed of a motor
void Reset_Motors();                                                // Reset both motors to their initial state
void Reset_Motor(Brushless*);                                       // Reset a specific motor to its initial state
int Get_Motor_Multiplier(Brushless*);                               // Get the speed multiplier for a motor from the controller command message
void Toggle_Brushless();                                                 // Toggle between wheel and propeller control

#endif
