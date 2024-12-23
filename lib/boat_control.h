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
#include <wiringPi.h>

// Pin Definitions
#define LEFT_PWM_PIN    0
#define RIGHT_PWM_PIN   0

// Global Constants
#define ROBOT_ID        6       // Unique ID to each Arduino : RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
#define MAX_SPEED       3       // Maximum speed multiplier for the robot

// Speed Levels
const int LEFT_SPEEDS[MAX_SPEED]  = {180, 210, 240};   // Speed levels for the left motor
const int RIGHT_SPEEDS[MAX_SPEED] = {120, 150, 180};   // Speed levels for the right motor

// Function Prototypes
void PWM_Init();

#endif 