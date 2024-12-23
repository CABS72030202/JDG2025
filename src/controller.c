// controller.c
// Created on: 2024-11-14
// Author: Sebastien Cabana

#include "../lib/controller.h"

int main() {
    
    // Open the serial port
    int uart_fd = serialOpen(UART, BAUD_RATE);
    if (uart_fd < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    // Initialize boat control
    PWM_Init();

    // Initialize Bluetooth communication as server
    if(SKIP_BLUETOOTH) 
        printf("WARNING. BLUETOOTH SERVER IS DISABLED.\n");
    else {
        server_sock = bt_init(1, NULL);
        if (server_sock < 0) return 1;
        client_sock = server_sock;
        struct sockaddr_rc client_addr = { 0 };
        socklen_t opt = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &opt);
        if (client_sock < 0) {
            perror("Failed to accept connection");
            close(server_sock);
            return 1;
        }
        char client_address[18] = { 0 };
        ba2str(&client_addr.rc_bdaddr, client_address);
        printf("Accepted connection from %s\n", client_address);
    }

    // Start controller communication
    int fd = open(GAMEPAD_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open gamepad");
        return 1;
    }
    
    if(!BLOCKING_MODE)
        // Set the gamepad file descriptor to non-blocking mode
        if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1) {
            perror("Failed to set non-blocking mode");
            close(fd);
            return 1;
        }

    struct js_event e;
    
    while(1) {
        // Attempt to read controller event
        ssize_t bytes_read = read(fd, &e, sizeof(struct js_event));
        if (bytes_read == sizeof(struct js_event)) {
            // Process controller event if available
            Controller_Event(e);   

            // Format message
            if(Format_Message(robot, l_speed, r_speed, arm)) {

                // Send message via UART
                for (int i = 0; i < strlen(message); i++) 
                    serialPutchar(uart_fd, message[i]);

                // Send message via Bluetooth
                if (!SKIP_BLUETOOTH && bt_send(client_sock, message) < 0) {
                    close(client_sock);
                    return 1;

                // Send message to boat
                }
            }
        }

        // Execute GPIO commands if change occured
        if(Check_GPIO_Command()) {
            Change_Arm_State(uart_fd);
            Format_Message(robot, l_speed, r_speed, arm);
        }
    }

    serialClose(uart_fd);
    close(fd);
    close(client_sock);
    close(server_sock);
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
    if(e.type == JS_EVENT_BUTTON && e.value)     // 1 is pressed, 0 is released
        switch(e.number) {
            case A_BUTTON:
                //printf("Pressed button A\n");
                break;
            case B_BUTTON:
                //printf("Pressed button B\n");
                break;
            case X_BUTTON:
                //printf("Pressed button X\n");
                break;
            case Y_BUTTON:
                if(constant_speed != AXIS_RANGE_COUNT - 1)      // Change to fast mode
                    constant_speed = AXIS_RANGE_COUNT - 1;
                else                                            // Change to slow mode
                    constant_speed = 1;
                printf("Pressed button Y: Changing constant speed to %s\n", (constant_speed > 1) ? "FAST" : "SLOW");
                break;
            case LB_BUTTON:
                printf("Pressed button LB: Backspin left wheel\n");
                l_speed = -constant_speed;
                break;
            case RB_BUTTON:
                printf("Pressed button RB: Backspin right wheel\n");
                r_speed = -constant_speed;
                break;
            case BACK_BUTTON:
                Cycle_Robot(-1);
                printf("Pressed button BACK: Switched to robot %i\n", robot);
                break;
            case START_BUTTON:
                Cycle_Robot(1);
                printf("Pressed button START: Switched to robot %i\n", robot);
                break;
            case HOME_BUTTON:
                printf("Pressed button HOME: Station robot %i is ready\n", robot);
                switch (robot) {
                    case RED:       Write_Arm_DEC(RED_READY); break;
                    case GREEN:     Write_Arm_DEC(GREEN_READY); break;
                    case BLUE:      Write_Arm_DEC(BLUE_READY); break;
                    case YELLOW:    Write_Arm_DEC(YELLOW_READY); break;
                    case PURPLE:    Write_Arm_DEC(PURPLE_READY); break;
                    default:        break;
                }
                break;
            case L_STICK_PRESS:
                //printf("Pressed LEFT STICK\n");
                break;
            case R_STICK_PRESS:
                //printf("Pressed RIGHT STICK\n");
                break;
            default:
                return;
        }

    // Check if button released
    if(e.type == JS_EVENT_BUTTON && !e.value)
        switch(e.number) {
            case LB_BUTTON:
                printf("Released button LB: Stopping left wheel\n");
                l_speed = 0;
                break;
            case RB_BUTTON:
                printf("Released button RB: Stopping right wheel\n");
                r_speed = 0;
                break;
            default:
                return;
        }

    // Check if axis moved
    else if(e.type == JS_EVENT_AXIS)
        switch (e.number) {
            case LT_BUTTON:
                LT_val = e.value;
                temp_range = Get_Anal_Range(LT_val);
                if(LT_range != temp_range) {
                    LT_range = temp_range;
                    printf("Pressed button LT: Frontspin left wheel with factor %i\n", LT_range);
                    l_speed = LT_range;
                }
                break;
            case RT_BUTTON:
                RT_val = e.value;
                temp_range = Get_Anal_Range(RT_val);
                if(RT_range != temp_range) {
                    RT_range = temp_range;
                    printf("Pressed button RT: Frontspin right wheel with factor %i\n", RT_range);
                    r_speed = RT_range;
                }
                break;
            case L_STICK_X:
                LSX_val = e.value;
                temp_dir = Get_Direction(LSX_val, LSY_val);
                if(LS_dir != temp_dir) {
                    LS_dir = temp_dir;
                    if(LS_dir != NONE && 0)     // DEACTIVATED
                        printf("Left stick used. Direction is %s.\n", Direction_Str(LS_dir));
                }
                break;
            case L_STICK_Y:
                LSY_val = e.value;
                temp_dir = Get_Direction(LSX_val, LSY_val);
                if(LS_dir != temp_dir) {
                    LS_dir = temp_dir;
                    if(robot == CONE) {    // ACTIVATED ONLY IF CONE IS SELECTED
                        printf("Left stick used. Direction is %s.\n", Direction_Str(LS_dir));
                        Control_Gripper();
                    }
                }
                break;
            case R_STICK_X:
                RSX_val = e.value;
                temp_dir = Get_Direction(RSX_val, RSY_val);
                if(RS_dir != temp_dir) {
                    RS_dir = temp_dir;
                    if(RS_dir != NONE && 0)     // DEACTIVATED
                        printf("Right stick used. Direction is %s.\n", Direction_Str(RS_dir));
                }
                break;
            case R_STICK_Y:
                RSY_val = e.value;
                temp_dir = Get_Direction(RSX_val, RSY_val);
                if(RS_dir != temp_dir) {
                    RS_dir = temp_dir;
                    if(robot == CONE) {    // ACTIVATED ONLY IF CONE IS SELECTED
                        printf("Right stick used. Direction is %s.\n", Direction_Str(RS_dir));
                        Control_Gripper();
                    }
                }
                break;
            case CROSS_X:
                CX_val = e.value;
                temp_dir = Get_Direction(CX_val, CY_val);
                if(CR_dir != temp_dir) {
                    CR_dir = temp_dir;
                    printf("Cross used. Direction is %s.\n", Direction_Str(CR_dir));
                    Get_Speed_From_Dir(CR_dir);
                }
                break;
            case CROSS_Y:
                CY_val = e.value;
                temp_dir = Get_Direction(CX_val, CY_val);
                if(CR_dir != temp_dir) {
                    CR_dir = temp_dir;
                    printf("Cross used. Direction is %s.\n", Direction_Str(CR_dir));
                    Get_Speed_From_Dir(CR_dir);
                }
                break;
            default:
                return;
        }
}

int Format_Message(Color c, int l, int r, Direction a) {

/*
 * Message format: a 9-character string structured as follows:
 * <robot>:<left wheel speed>:<right wheel speed>:<arm control>
 *
 * - The first character represents the robot's color, indicated by the first letter:
 *   'R' (RED), 'G' (GREEN), 'B' (BLUE), 'Y' (YELLOW), 'P' (PURPLE), 'C' (CONE), 'S' (SHIP).
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

    char temp[] = "0:00:00:0\r\n";

    // <robot>
    switch (c) {
        case 0:
            temp[0] = 'R';
            if(Read_Arm_BIN() == RED_UP && a == DOWN)
                a = UP;
            break;
        case 1:
            temp[0] = 'G';
            if(Read_Arm_BIN() == GREEN_UP && a == DOWN)
                a = UP;
            break;
        case 2:
            temp[0] = 'B';
            if(Read_Arm_BIN() == BLUE_UP && a == DOWN)
                a = UP;
            break;
        case 3:
            temp[0] = 'Y';
            if(Read_Arm_BIN() == YELLOW_UP && a == DOWN)
                a = UP;
            break;
        case 4:
            temp[0] = 'P';
            if(Read_Arm_BIN() == PURPLE_UP && a == DOWN)
                a = UP;
            break;
        case 5:
            temp[0] = 'C';
            break;
        case 6:
            temp[0] = 'S';
            break;
        default:
            printf("ERROR. Invalid color.\n");
            return 0;
    }

    // <left wheel speed>
    if(l < 0)
        temp[2] = '-';
    else
        temp[2] = '+';
    temp[3] = abs(l) + 48;

    // <right wheel speed>
    if(r < 0)
        temp[5] = '-';
    else
        temp[5] = '+';
    temp[6] = abs(r) + 48;

    // <arm control>
    switch (a) {
        case UP:
            temp[8] = 'U';
            break;
        case DOWN:
            temp[8] = 'D';
            break;
        default:
            printf("ERROR. Invalid arm direction.\n");
            return 0;
    }

    // Change message only if change occurred
    if(strcmp(message, temp) == 0)
        return 0;
    else {
        strcpy(message, temp);
        return 1;
    }
}

void Cycle_Robot(int i) {   // -1 is previous, 1 is next
    // Find current robot position in order
    int curr_robot_pos;
    for(curr_robot_pos = 0; curr_robot_pos < 7; curr_robot_pos++)
        if(robot == robot_order[curr_robot_pos])
            break;
    
    // Get new robot order position
    int new_robot_pos = curr_robot_pos + i;
    if(new_robot_pos == -1)
        new_robot_pos = 6;
    if(new_robot_pos == 7)
        new_robot_pos = 0;

    // Change current robot
    robot = robot_order[new_robot_pos];
}

void Get_Speed_From_Dir(Direction dir) {
    switch (dir)
    {
    case NONE:
        l_speed = 0;
        r_speed = 0;
        break;
    case UP:
        l_speed = constant_speed;
        r_speed = constant_speed;
        break;
    case DOWN:
        l_speed = -constant_speed;
        r_speed = -constant_speed;
        break;
    case LEFT:
        l_speed = -constant_speed;
        r_speed = constant_speed;
        break;
    case RIGHT:
        l_speed = constant_speed;
        r_speed = -constant_speed;
        break;
    default:
        break;
    }
}

int Check_GPIO_Command() {
    int temp = Read_Arm_BIN();
    switch(temp) {
        case RED_READY:     return 0;
        case GREEN_READY:   return 0;
        case BLUE_READY:    return 0;
        case YELLOW_READY:  return 0;
        case PURPLE_READY:  return 0;
        default: break;
    }
    if(temp == GPIO_command)
        return 0;     // No change
    else {
        GPIO_command = temp;
        return 1;
    }
}

void Change_Arm_State(int uart_fd) {
    message[2] = '+';
    message[3] = '0';
    message[5] = '+';
    message[6] = '0';
    message[8] = 'D';
    switch (GPIO_command)
    {
    case DOWN_ALL:
        message[0] = 'R';
        for (int i = 0; i < strlen(message); i++) 
            serialPutchar(uart_fd, message[i]);
        message[0] = 'G';
        for (int i = 0; i < strlen(message); i++) 
            serialPutchar(uart_fd, message[i]);
        message[0] = 'B';
        for (int i = 0; i < strlen(message); i++) 
            serialPutchar(uart_fd, message[i]);
        message[0] = 'Y';
        for (int i = 0; i < strlen(message); i++) 
            serialPutchar(uart_fd, message[i]);
        message[0] = 'P';
        for (int i = 0; i < strlen(message); i++) 
            serialPutchar(uart_fd, message[i]);
        return;
    case RED_UP:
        message[8] = 'U';
    case RED_DOWN:
        message[0] = 'R';
        break;
    case GREEN_UP:
        message[8] = 'U';
    case GREEN_DOWN:
        message[0] = 'G';
        break;
    case BLUE_UP:
        message[8] = 'U';
    case BLUE_DOWN:
        message[0] = 'B';
        break;
    case YELLOW_UP:
        message[8] = 'U';
    case YELLOW_DOWN:
        message[0] = 'Y';
        break;
    case PURPLE_UP:
        message[8] = 'U';
    case PURPLE_DOWN:
        message[0] = 'P';
        break;
    default:
        printf("ERROR. Invalid option in Change_Arm_State(int).\n");
        return;
    }
    for (int i = 0; i < strlen(message); i++) 
        serialPutchar(uart_fd, message[i]);
}

void Control_Gripper() {

/*
 * Message format: a 5-character string structured as follows:
 * [<arm>:<claw>]
 *
 * Possible values for <arm> and <claw> are as follows (inverted angles) :
 *   - '+' to decrement the angle
 *   - '-' to increment the angle
 *   - '0' to keep current angle
 */

    // Return early if Bluetooth is disabled
    if(SKIP_BLUETOOTH)
        return;

    // Return early if invalid robot
    if(robot != CONE)
        return;

    // Format message
    switch(LS_dir) {
        case UP:   gripper_message[1] = '-'; break;
        case DOWN: gripper_message[1] = '+'; break;
        default:   gripper_message[1] = '0'; break;
    }
    switch(RS_dir) {
        case UP:   gripper_message[3] = '-'; break;
        case DOWN: gripper_message[3] = '+'; break;
        default:   gripper_message[3] = '0'; break;
    }
    
    // Send message via Bluetooth
    if (!SKIP_BLUETOOTH)
        bt_send(client_sock, gripper_message);
}