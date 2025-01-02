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
#include "../lib/constants.h"
#include "../lib/rpi_servo.h"

// Enums and Structs
typedef struct {
    char side;              // 'R' for Right or 'L' for Left
    float speed;            // Current speed as a percentage (0.0 to 1.0)
    int multiplier;         // Current speed as a multiplier
    int motor_pin;          // GPIO Pin for the motor PWM
    int power_pin;          // GPIO Pin to provide 5V power to the motor
} Brushless;

// Global Variables
extern Brushless left_motor;        // Configuration for the left motor
extern Brushless right_motor;       // Configuration for the right motor
extern char message[];              // Controller command message

// Function Prototypes
int PWM_Init();                         // Initialize brushless motors control
void Control_Boat();                    // Main control logic for the boat
void Motor_Startup(Brushless*);         // Startup routine for a motor
void Set_Motor_Speed(Brushless*);       // Set the speed of a motor
void Reset_Motors();                    // Reset both motors to their initial state
void Reset_Motor(Brushless*);           // Reset a specific motor to its initial state
int Get_Motor_Multiplier(Brushless*);   // Get the speed multiplier for a motor from the controller command message

#endif
