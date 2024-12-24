/*
 * controller.h
 * Created on: 2024-11-14
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file provides the interface for managing gamepad input, UART communication, 
 * and GPIO controls for the robotic solution. It includes constants, global variables, 
 * and functions to handle analog and digital gamepad events, send formatted commands 
 * via UART, and manage robot states and directions. 
 * 
 * Source: https://www.electronicwings.com/raspberry-pi/raspberry-pi-uart-communication-using-python-and-c
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H    

// Includes
#include <fcntl.h>
#include <linux/joystick.h>
#include "../lib/gpio.h"
#include "../lib/rpi_bluetooth.h"
#include "../lib/boat_control.h"

// Debug Constants
#define SKIP_BLUETOOTH     0                     // Set to 1 to disable Bluetooth initialization

// Global Constants
#define GAMEPAD_PATH       "/dev/input/js0"      // Path to the gamepad device
#define BLOCKING_MODE      0                     // Set to 1 to wait for controller updates
const int robot_order[] = {6, 0, 4, 3, 1, 5, 2}; // Order of robot selection

// Digital Button IDs
#define A_BUTTON           0                     // ID for button A
#define B_BUTTON           1                     // ID for button B
#define X_BUTTON           2                     // ID for button X
#define Y_BUTTON           3                     // ID for changing constant speed
#define LB_BUTTON          4                     // ID for back left wheel control
#define RB_BUTTON          5                     // ID for back right wheel control
#define BACK_BUTTON        6                     // ID for selecting the previous robot
#define START_BUTTON       7                     // ID for selecting the next robot
#define HOME_BUTTON        8                     // ID for adding a station to the queue
#define L_STICK_PRESS      9                     // ID for pressing the left stick
#define R_STICK_PRESS      10                    // ID for pressing the right stick

// Analog Button IDs (Axis)
#define LT_BUTTON          2                     // ID for controlling front left wheel (variable speed)
#define RT_BUTTON          5                     // ID for controlling front right wheel (variable speed)
#define L_STICK_X          0                     // ID for left stick X-axis
#define L_STICK_Y          1                     // ID for left stick Y-axis
#define R_STICK_X          3                     // ID for right stick X-axis
#define R_STICK_Y          4                     // ID for right stick Y-axis
#define CROSS_X            6                     // ID for rotating using one wheel
#define CROSS_Y            7                     // ID for driving with both wheels

// Analog Ranges
#define ANAL_SHIFT_VAL     32767                 // Shift value for analog inputs
#define ANAL_MAX_VAL       65534                 // Maximum value for analog inputs
#define AXIS_RANGE_COUNT   4                     // Number of axis ranges (includes 0)
#define JOYSTICK_THRESHOLD 10000                 // Threshold to detect joystick movement

// Special Types
enum direction {NONE, UP, DOWN, LEFT, RIGHT};    // Direction enum for movement
typedef enum direction Direction;

enum color {RED, GREEN, BLUE, YELLOW, PURPLE, CONE, SHIP}; // Color enum for robots
typedef enum color Color;

// Axis Values
int LT_val = 0;                                 // Current value of LT axis
int RT_val = 0;                                 // Current value of RT axis
int LSX_val = 0;                                // Current value of left stick X-axis
int LSY_val = 0;                                // Current value of left stick Y-axis
int RSX_val = 0;                                // Current value of right stick X-axis
int RSY_val = 0;                                // Current value of right stick Y-axis
int CX_val = 0;                                 // Current value of cross X-axis
int CY_val = 0;                                 // Current value of cross Y-axis

// Axis Ranges
int LT_range = 0;                               // Current range of LT axis
int RT_range = 0;                               // Current range of RT axis

// Axis Directions
Direction LS_dir = NONE;                        // Current direction of left stick
Direction RS_dir = NONE;                        // Current direction of right stick
Direction CR_dir = NONE;                        // Current direction of cross input

// Global Variables
int constant_speed = 1;                         // Speed multiplier for constant controls
char* message = "0:00:00:0\r\n";                // Formatted message for robot control
char* gripper_message = "[0:0]\r\n";            // Formatted message for gripper arm control
Color robot = NONE;                             // Current selected robot
int l_speed = 0;                                // Current left wheel speed
int r_speed = 0;                                // Current right wheel speed
Direction arm = DOWN;                           // Arm control direction
int GPIO_command = 0;                           // Current GPIO command for arm control
int server_sock;                                // Bluetooth server socket
int client_sock;                                // Bluetooth client socket

// Prototypes
int Get_Anal_Range(int);                       // Get the range of an analog input value
Direction Get_Direction(int, int);             // Get the direction based on x and y axis values
char* Direction_Str(Direction);                // Convert a direction to its string representation
void Controller_Event(struct js_event);        // Handle a controller event
int Format_Message();                          // Format the robot control message if a change in commands occurred (1)
void Cycle_Robot(int);                         // Cycle to the next or previous robot (-1 or 1)
void Get_Speed_From_Dir(Direction);            // Set wheel speeds based on direction
int Check_GPIO_Command();                      // Check if a GPIO command has changed
void Change_Arm_State(int);                    // Change the arm state (e.g., UP/DOWN)
void Control_Gripper();                        // Send gripper arm (0) or caw (1) instructions over Bluetooth to cone robot

#endif