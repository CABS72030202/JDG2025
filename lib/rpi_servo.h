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

// Enums and Structs
typedef struct {
    int is_open;            // 1 is yes, 0 is no
    int servo_pin;          // GPIO Pin to provide 5V power to the motor
    int close_angle;        // Value when closed
    int open_angle;         // Value when open
} Servo;

// Pin Definitions
#define RAMP_SERVO_PIN 7

// Global Constants
#define SERVO_MIN_PULSE_WIDTH   0       // Minimum pulse width for the servo (µs)
#define SERVO_MAX_PULSE_WIDTH   100     // Maximum pulse width for the servo (µs)
#define TURN_TIME_PER_DEGREE    10      // Typical time in milliseconds to turn 1 degree

// Servo angle values
#define RAMP_SERVO_OPEN         10      // Pulse width for open position
#define RAMP_SERVO_CLOSE        20      // Pulse width for close position

// Global Variables
extern Servo ramp_servo;                // Servomotors locking boat ramps

// Function Prototypes
int Servo_Init();                       // Initialize servomotors control
void Toggle_Servo(Servo*);              // Toggle specific servomotor's state (open/close)
void Continuous_Mode(Servo*, int);      // Write an angle larger than 180 degrees
int Map_Angle_To_Pulse(int);            // Get the required pulse for a specific angle in continuous mode 
int Map_Angle_To_Delay(int);            // Get the required turning time for a specific angle in continuous mode 

#endif