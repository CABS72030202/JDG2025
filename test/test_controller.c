/*
 * ----------------------------------------------------------------------------
 * File Name: test_controller.c
 *
 * Description:
 * This program is designed to enable control of a robot using a Logitech Wireless 
 * Gamepad F710 on a Raspberry Pi 3B. It captures inputs from the gamepad, interprets 
 * the commands, and sends them as output for controlling various robot functions, 
 * such as resetting the MCU, toggling speed reduction, and handling joystick movements.
 *
 * Features:
 * - Reads input signals from the Logitech F710 gamepad.
 * - Uses those signals to set commands for robot actions (e.g., motor power, direction inversion, 
 *   stepper control, turbine toggle).
 * - MCU reset functionality by pressing the Back button on the controller.
 * 
 * Usage:
 * 1. Compile the program using the following command:
 *    gcc -o test_controller test_controller.c
 *
 * 2. Run the compiled program to begin listening for gamepad inputs:
 *    ./test_controller
 *
 * Note:
 * - Ensure the Logitech F710 gamepad is connected and recognized by the Raspberry Pi 3B.
 * - Requires ROS libraries for handling communication.
 *
 * Requirements:
 * - ROS setup with the necessary messages for Int32MultiArray and Joy message types.
 * - Compilation with a C compiler (e.g., GCC) and linked libraries as specified.
 * 
 * Sources:
 * - https://github.com/chgirard/jdg_jetson/blob/master/src/control_node/src/control.py
 * 
 * ----------------------------------------------------------------------------
 */


#include <stdio.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#define GAMEPAD_PATH "/dev/input/js0"

// Digital button IDs
#define A_BUTTON 0
#define B_BUTTON 1
#define X_BUTTON 2
#define Y_BUTTON 3
#define LB_BUTTON 4
#define RB_BUTTON 5
#define BACK_BUTTON 6
#define START_BUTTON 7
#define HOME_BUTTON 8
#define L_STICK_PRESS 9
#define R_STICK_PRESS 10

// Analog button IDs (axis)
#define LT_BUTTON 2
#define RT_BUTTON 5
#define L_STICK_X 0
#define L_STICK_Y 1
#define R_STICK_X 3
#define R_STICK_Y 4
#define CROSS_X 6
#define CROSS_Y 7

// Analog ranges
#define ANAL_SHIFT_VAL 32767        
#define ANAL_MAX_VAL 65534
#define AXIS_RANGE_COUNT 4          // Includes 0
#define JOYSTICK_THRESHOLD 10000

// Special types
enum direction {NONE, UP, DOWN, LEFT, RIGHT};
typedef enum direction Direction;

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

// Prototypes
int Get_Anal_Range(int val);
Direction Get_Direction(int x, int y);
char* Direction_Str(Direction dir);
void Controller_Event(struct js_event);

int main() {
    int fd = open(GAMEPAD_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open gamepad");
        return 1;
    }
    
    struct js_event e;
    
    while(1) {
        // Read controller event
        if(read(fd, &e, sizeof(struct js_event)) != sizeof(struct js_event)) {
            perror("Failed to read gamepad event. Exit with error 1.\n");
            close(fd);
            return 1;
        }

        // Process controller event
        Controller_Event(e);   
    }

    close(fd);
    return 0;
}

int Get_Anal_Range(int val) {
    val += ANAL_SHIFT_VAL;
    for(int i = 1; i <= AXIS_RANGE_COUNT; i++)
        if(val < ((ANAL_MAX_VAL / AXIS_RANGE_COUNT) * i))
            return i - 1;
    return AXIS_RANGE_COUNT - 1;
}

Direction Get_Direction(int x, int y) {
    if (abs(x) < JOYSTICK_THRESHOLD && abs(y) < JOYSTICK_THRESHOLD)     // Near center, no direction
        return NONE; 
    if (abs(x) > abs(y))                                                // Horizontal movement
        return (x < 0) ? LEFT : RIGHT;
     else                                                               // Vertical movement
        return (y > 0) ? DOWN : UP;
}

char* Direction_Str(Direction dir) {
    switch (dir) {
        case 0:
            return "NONE";
        case 1:
            return "UP";
        case 2:
            return "DOWN";
        case 3:
            return "LEFT";
        case 4:
            return "RIGHT";
        default:
            printf("ERROR. Invalid direction.\n");
            return NULL;
    }
}

void Controller_Event(struct js_event e) {
    int temp_range = 0;
    Direction temp_dir = NONE;

    // Check if button pressed
    if(e.type == JS_EVENT_BUTTON && e.value) {     // 1 is pressed, 0 is released
        switch(e.number) {
            case A_BUTTON:
                printf("Pressed button A\n");
                break;
            case B_BUTTON:
                printf("Pressed button B\n");
                break;
            case X_BUTTON:
                printf("Pressed button X\n");
                break;
            case Y_BUTTON:
                printf("Pressed button Y\n");
                break;
            case LB_BUTTON:
                printf("Pressed button LB\n");
                break;
            case RB_BUTTON:
                printf("Pressed button RB\n");
                break;
            case BACK_BUTTON:
                printf("Pressed button BACK\n");
                break;
            case START_BUTTON:
                printf("Pressed button START\n");
                break;
            case HOME_BUTTON:
                printf("Pressed button HOME\n");
                break;
            case L_STICK_PRESS:
                printf("Pressed LEFT STICK\n");
                break;
            case R_STICK_PRESS:
                printf("Pressed RIGHT STICK\n");
                break;
            default:
                printf("ERROR. Invalid button.\n");
                break;
        }
    }

    // Check if axis moved
    else if(e.type == JS_EVENT_AXIS) {
        switch (e.number) {
            case LT_BUTTON:
                LT_val = e.value;
                temp_range = Get_Anal_Range(LT_val);
                if(LT_range != temp_range) {
                    LT_range = temp_range;
                    if(LT_range > 0)
                        printf("LT pressed. Range is %i.\n", LT_range);
                }
                break;
            case RT_BUTTON:
                RT_val = e.value;
                temp_range = Get_Anal_Range(RT_val);
                if(RT_range != temp_range) {
                    RT_range = temp_range;
                    if(RT_range > 0)
                        printf("RT pressed. Range is %i.\n", RT_range);
                }
                break;
            case L_STICK_X:
                LSX_val = e.value;
                temp_dir = Get_Direction(LSX_val, LSY_val);
                if(LS_dir != temp_dir) {
                    LS_dir = temp_dir;
                    if(LS_dir != NONE)
                        printf("Left stick used. Direction is %s.\n", Direction_Str(LS_dir));
                }
                break;
            case L_STICK_Y:
                LSY_val = e.value;
                temp_dir = Get_Direction(LSX_val, LSY_val);
                if(LS_dir != temp_dir) {
                    LS_dir = temp_dir;
                    if(LS_dir != NONE)
                        printf("Left stick used. Direction is %s.\n", Direction_Str(LS_dir));
                }
                break;
            case R_STICK_X:
                RSX_val = e.value;
                temp_dir = Get_Direction(RSX_val, RSY_val);
                if(RS_dir != temp_dir) {
                    RS_dir = temp_dir;
                    if(RS_dir != NONE)
                        printf("Right stick used. Direction is %s.\n", Direction_Str(RS_dir));
                }
                break;
            case R_STICK_Y:
                RSY_val = e.value;
                temp_dir = Get_Direction(RSX_val, RSY_val);
                if(RS_dir != temp_dir) {
                    RS_dir = temp_dir;
                    if(RS_dir != NONE)
                        printf("Right stick used. Direction is %s.\n", Direction_Str(RS_dir));
                }
                break;
            case CROSS_X:
                CX_val = e.value;
                temp_dir = Get_Direction(CX_val, CY_val);
                if(CR_dir != temp_dir) {
                    CR_dir = temp_dir;
                    if(CR_dir != NONE)
                        printf("Cross used. Direction is %s.\n", Direction_Str(CR_dir));
                }
                break;
            case CROSS_Y:
                CY_val = e.value;
                temp_dir = Get_Direction(CX_val, CY_val);
                if(CR_dir != temp_dir) {
                    CR_dir = temp_dir;
                    if(CR_dir != NONE)
                        printf("Cross used. Direction is %s.\n", Direction_Str(CR_dir));
                }
                break;
            default:
                printf("ERROR. Invalid axis.\n");
                break;
        }
    }
}