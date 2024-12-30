/*
 * gripper.h
 * Created on: 2024-12-20
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the hardware-related constants and configurations 
 * for the gripper fixed on the CONE robot. These constants are specific to 
 * the gripper's hardware setup, including pin assignments and servo settings.
 */

#ifndef GRIPPER_H
#define GRIPPER_H

// Includes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Servo.h>

// Pin Definitions
#define ARM_PIN        9        // Pin for arm servo motor control
#define CLAW_PIN       10       // Pin for claw servo motor control

// Gripper Specific Global Constants
#define CONE_ROBOT_ID  5        // ID corresponding to the robot equipped with a gripper

// Servo Angles
#define ANGLE_STEP     3        // Increment for angle adjustments
#define OPEN_BOUND     60       // Maximum open angle for the claw
#define CLOSE_BOUND    0        // Minimum closed angle for the claw
#define UP_BOUND       30       // Arm angle for position above the rear wheel
#define DOWN_BOUND     180      // Arm angle for position just above the ground
#define INIT_ANGLE     80       // Arm angle to reach before attaching claw servo

// Global Variables
String gripper_message = "[0:0]\r\n";       // Incoming command message for gripper control
int claw_angle         = CLOSE_BOUND;       // Initial claw angle: fully closed
int arm_angle          = DOWN_BOUND - 10;   // Initial arm angle: parallel to the ground
bool init_claw         = false;             // Attach claw servo only when arm is raised after setup
Servo grip_claw;
Servo grip_arm;

// Prototypes
void Init_Claw();                     // Attach claw servo at specific moment after setup
void Decode_Gripper();                // Decode the command message and extract gripper control values
void Gripper_Control();               // Execute extracted gripper controls

#endif