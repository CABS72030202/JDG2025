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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <softPwm.h>

// Pin Definitions
#define SERVO_PIN 7

// Global Constants
#define SERVO_MIN_PULSE_WIDTH   0       // Minimum pulse width for the servo (µs)
#define SERVO_MAX_PULSE_WIDTH   100     // Maximum pulse width for the servo (µs)
#define SERVO_OPEN_ANGLE        10      // Pulse width for open position
#define SERVO_CLOSE_ANGLE       20      // Pulse width for close position

// Global Variables
static int is_servo_open = 0;           // Current servomotors state

// Function Prototypes
int Servo_Init();                       // Initialize servomotors control
void Toggle_Servo();                    // Toggle servomotors state (open/close)

#endif