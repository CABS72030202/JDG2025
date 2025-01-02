/*
 * rpi_servo.h
 * Created on: 2024-12-30
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the hardware-related constants and configurations for the 
 * BOAT robot servomotors that control the ramp. These constants are specific to the  
 * BOAT robot's hardware setup, including pin assignments and servo settings.
 * 
 * Source: https://www.electronicwings.com/raspberry-pi/raspberry-pi-pwm-generation-using-python-and-c
 */

#ifndef RPI_SERVO_H
#define RPI_SERVO_H

// Includes
#include "../lib/constants.h"
#include <softPwm.h>

// Global Variables
static int is_servo_open = 0;           // Current servomotors state

// Function Prototypes
int Servo_Init();                       // Initialize servomotors control
void Toggle_Servo();                    // Toggle servomotors state (open/close)

#endif