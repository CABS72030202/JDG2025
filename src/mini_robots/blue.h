/*
 * blue.h
 * Created on: 2024-12-19
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the hardware-related constants and configurations 
 * for the BLUE robot. These constants are specific to the BLUE robot's hardware 
 * setup, including pin assignments and motor/servo settings.
 */

#ifndef BLUE_H
#define BLUE_H

// Pin Definitions
#define DC_FL_PIN       6       // Pin for front-left DC motor drive
#define DC_FR_PIN       3       // Pin for front-right DC motor drive
#define DC_BL_PIN       5       // Pin for back-left DC motor drive
#define DC_BR_PIN       11      // Pin for back-right DC motor drive
#define SERVO_PIN       9       // Pin for servo motor control
#define ARM_LED_PIN     12      // Pin for arm activity LED
#define ROBOT_LED_PIN   13      // Pin for robot control status LED

// Robot Specific Global Constants
#define ROBOT_ID        2       // Unique ID to each Arduino : RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
#define MAX_SPEED       3       // Maximum speed multiplier for the robot

// Speed Levels
const int LEFT_SPEEDS[MAX_SPEED]  = {140, (140+255)/2, 255};   // Speed levels for the left motor
const int RIGHT_SPEEDS[MAX_SPEED] = {100, (100+255)/2, 250};   // Speed levels for the right motor

// Servo Angles
#define ARM_ANGLE_UP    15      // Servo angle for the arm in the UP position
#define ARM_ANGLE_DOWN  35      // Servo angle for the arm in the DOWN position

#endif 