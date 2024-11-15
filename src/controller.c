// controller.c
// Created on: 2024-11-14
// Author: Sebastien Cabana

/* 
 * 
 */

#include "../lib/controller.h"

int main() {
    
    // Start UART communication
    int uart_fd = serialOpen(UART, BAUD_RATE);
    if (uart_fd == -1) {
        fprintf(stderr, "Unable to open UART: %s\n", strerror(errno));
        return 1;
    }

    // Start controller communication
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

        // Format message
        Format_Message();

        // Send message
        serialPuts(uart_fd, message);
    }

    serialClose(uart_fd);
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

void Format_Message() {

/*
 * Message format: a 9-character string structured as follows:
 * <robot>:<left wheel speed>:<right wheel speed>:<arm control>
 *
 * - The first character represents the robot's color, indicated by the first letter:
 *   'R' (RED), 'G' (GREEN), 'B' (BLUE), 'Y' (YELLOW), 'P' (PURPLE), 'C' (CONE), 'N' (BOAT).
 *
 * - The next two characters specify the left wheel speed:
 *   - The first character is the direction: '+' for forward, '-' for reverse.
 *   - The second character is the base speed multiplier: '0' stops the wheel, '1' for low speed, etc.
 *
 * - The next two characters specify the right wheel speed, using the same format as the left:
 *   direction ('+' or '-') and speed multiplier.
 *
 * - The last character controls the robot arm movement:
 *   'U' to raise the arm and 'D' to lower it.
 */

    // <robot>
    switch (robot) {
        case 0:
            message[0] = 'R';
            break;
        case 1:
            message[0] = 'G';
            break;
        case 2:
            message[0] = 'B';
            break;
        case 3:
            message[0] = 'Y';
            break;
        case 4:
            message[0] = 'P';
            break;
        case 5:
            message[0] = 'C';
            break;
        case 6:
            message[0] = 'N';
            break;
        default:
            printf("ERROR. Invalid color.\n");
            return;
    }

    // <left wheel speed>
    if(l_speed < 0)
        message[2] = '-';
    else
        message[2] = '+';
    message[3] = abs(l_speed);

    // <right wheel speed>
    if(r_speed < 0)
        message[5] = '-';
    else
        message[5] = '+';
    message[6] = abs(r_speed);

    // <arm control>
    switch (dir) {
        case 1:
            message[8] = 'U';
            break;
        case 2:
            message[8] = 'D';
            break;
        case 0:
        case 3:
        case 4:
        default:
            printf("ERROR. Invalid direction.\n");
            return;
    }
}