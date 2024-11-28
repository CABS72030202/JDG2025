// controller.h
// Created on: 2024-11-14
// Author: Sebastien Cabana

/* 
 * Source : https://www.electronicwings.com/raspberry-pi/raspberry-pi-uart-communication-using-python-and-c
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H    
#include <stdio.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include "../lib/gpio.h"

// Global constants
#define GAMEPAD_PATH "/dev/input/js0"
#define UART "/dev/serial0"
#define BAUD_RATE 115200
#define BLOCKING_MODE 1         // 1 = Wait for update on controller
const int robot_order[] = {6, 0, 4, 3, 1, 5, 2};

// Digital button IDs
#define A_BUTTON 0                  // 
#define B_BUTTON 1                  // 
#define X_BUTTON 2                  // 
#define Y_BUTTON 3                  // Change constant speed
#define LB_BUTTON 4                 // Control back left wheel
#define RB_BUTTON 5                 // Control back right wheel
#define BACK_BUTTON 6               // Select previous robot
#define START_BUTTON 7              // Select next robot
#define HOME_BUTTON 8               // Add station in queue
#define L_STICK_PRESS 9             // 
#define R_STICK_PRESS 10            // 

// Analog button IDs (axis)
#define LT_BUTTON 2                 // Control front left wheel (variable speed)
#define RT_BUTTON 5                 // Control front right wheel (variable speed)
#define L_STICK_X 0                 // 
#define L_STICK_Y 1                 // 
#define R_STICK_X 3                 // 
#define R_STICK_Y 4                 // 
#define CROSS_X 6                   // Rotate using one wheel
#define CROSS_Y 7                   // Drive with both wheel

// Analog ranges
#define ANAL_SHIFT_VAL 32767        
#define ANAL_MAX_VAL 65534
#define AXIS_RANGE_COUNT 4          // Includes 0
#define JOYSTICK_THRESHOLD 10000

// Special types
enum direction {NONE, UP, DOWN, LEFT, RIGHT};
typedef enum direction Direction;
enum color {RED, GREEN, BLUE, YELLOW, PURPLE, CONE, SHIP};
typedef enum color Color;

// Axis values 
int LT_val = 0;
int RT_val = 0;
int LSX_val = 0;
int LSY_val = 0;
int RSX_val = 0;
int RSY_val = 0;
int CX_val = 0;
int CY_val = 0;

// Axis ranges 
int LT_range = 0;
int RT_range = 0;

// Axis direction
Direction LS_dir = NONE;
Direction RS_dir = NONE;
Direction CR_dir = NONE;

// Global variables
int constant_speed = 1;                // Speed multiplier for constant controls (no range)
char message[] = "0:00:00:0\r\n";      // Format is <Robot>:<Left wheel speed>:<Right wheel speed>:<Arm control>
Color robot = NONE;
int l_speed = 0;
int r_speed = 0;
Direction arm = DOWN;
int GPIO_command = 0;

// Prototypes
int Get_Anal_Range(int val);
Direction Get_Direction(int x, int y);
char* Direction_Str(Direction dir);
void Controller_Event(struct js_event);
void Format_Message();
void Cycle_Robot(int i);                // -1 is previous, 1 is next
void Get_Speed_From_Dir(Direction dir);
int Check_GPIO_Command();               // 1 if change occured
void Change_Arm_State(int);

#endif