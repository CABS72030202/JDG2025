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

    Set_Station_State(&r_station, ACTIVE);
    Set_Station_State(&g_station, ACTIVE);
    Set_Station_State(&b_station, ACTIVE);
    Set_Station_State(&y_station, ACTIVE);
    Set_Station_State(&p_station, ACTIVE); 

    // Testing GPIO
    /*Arm_Control(RED, ACTIVE);
    Delay(2);
    Arm_Control(GREEN, ACTIVE);
    Delay(2);
    Arm_Control(BLUE, ACTIVE);
    Delay(2);
    Arm_Control(YELLOW, ACTIVE);
    Delay(2);
    Arm_Control(PURPLE, ACTIVE);
    Delay(2);
    Arm_Control(PURPLE, INACTIVE);*/

    
        if(Connect_Station(BLUE) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Load_Passengers(YELLOW, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Connect_Station(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Drop_Passengers(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        
        if(Connect_Station(RED) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Load_Passengers(GREEN, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Connect_Station(GREEN) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Drop_Passengers(GREEN) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);

        if(Connect_Station(PURPLE) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Load_Passengers(YELLOW, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Connect_Station(YELLOW) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Drop_Passengers(YELLOW) == ERROR) {
        printf("Exit with error.\n");
        return 0;
    }
    

    /*while (Try_Connect() == OK) {
        Send("INFO:C\n");
        Receive(received);
        printf("Received: %s", received);
    }*/

    ws2811_fini(&ledstring);
    return 0; 
}