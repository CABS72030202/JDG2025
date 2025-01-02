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

// Enums and Structs
typedef struct {
    int is_open;            // 1 is yes, 0 is no
    int servo_pin;          // GPIO Pin to provide 5V power to the motor
    int close_angle;        // Value when closed
    int open_angle;         // Value when open
} Servo;

// Global Variables
extern Servo ramp_servo;        // Servomotors locking boat ramps
extern Servo switch_servo;      // Servomotor triggering a switch

// Function Prototypes
int Servo_Init();                       // Initialize servomotors control
void Toggle_Servo(Servo*);              // Toggle specific servomotor's state (open/close)
void Continuous_Mode(Servo*, int);      // Write an angle larger than 180 degrees
int Map_Angle_To_Pulse(int);            // Get the required pulse for a specific angle in continuous mode 
int Map_Angle_To_Delay(int);            // Get the required turning time for a specific angle in continuous mode 

#endif