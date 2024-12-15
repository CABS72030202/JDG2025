// blackbox.c
// Created on: 2024-09-14
// Author: Sebastien Cabana

/* 
 * This code implements the necessary functions for managing passenger interactions 
 * between the machine and the stations, excluding communication with the black box. 
 * Responses are pre-defined to allow code compilation and debugging without the 
 * inputs from any external components.
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include "../lib/station.h"
#include "../lib/communication.h"

// Prototypes

int main(void) {
    Initialize();
    //Wait_For_Two();
    //Auto_Load_Drop();

    r_station.state = ACTIVE;      
    g_station.state = ACTIVE;
    b_station.state = ACTIVE; 
    y_station.state = ACTIVE; 
    p_station.state = ACTIVE; 

    // Testing GPIO
    Arm_Control(RED, ACTIVE);
    Delay(2);
    Arm_Control(GREEN, ACTIVE);
    Delay(2);
    Arm_Control(BLUE, ACTIVE);
    Delay(2);
    Arm_Control(YELLOW, ACTIVE);
    Delay(2);
    Arm_Control(PURPLE, ACTIVE);
    Delay(2);
    Arm_Control(PURPLE, INACTIVE);

    /*while(1) {
        if(Connect_Station(BLUE) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Load_Passengers(YELLOW, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Connect_Station(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Drop_Passengers(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        
        if(Connect_Station(RED) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Load_Passengers(GREEN, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Connect_Station(GREEN) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Drop_Passengers(GREEN) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }

        if(Connect_Station(PURPLE) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Load_Passengers(YELLOW, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Connect_Station(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        if(Drop_Passengers(YELLOW) == ERROR) {
        printf("Exit with error.\n");
        return 0;
    }
    }

    /*while (Try_Connect() == OK) {
        Send("INFO:C\n");
        Receive(received);
        printf("Received: %s", received);
    }*/

    return 0; 
}