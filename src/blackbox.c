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
    /*Arm_Control(&r_station, ACTIVE);
    Delay(2);
    Arm_Control(&g_station, ACTIVE);
    Delay(2);
    Arm_Control(&b_station, ACTIVE);
    Delay(2);
    Arm_Control(&y_station, ACTIVE);
    Delay(2);
    Arm_Control(&p_station, ACTIVE);
    Delay(2);
    Arm_Control(&p_station, INACTIVE);*/

    
        if(Connect_Station(&g_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Load_Passengers(&r_station, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Connect_Station(&r_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Drop_Passengers(&r_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        
        if(Connect_Station(&p_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Load_Passengers(&g_station, 1) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Connect_Station(&g_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);
        if(Drop_Passengers(&g_station) == ERROR) {
            printf("Exit with error.\n");
            return 0;
        }
        Delay(3);

    /*while (Try_Connect() == OK) {
        Send("INFO:C\n");
        Receive(received);
        printf("Received: %s", received);
    }*/

    //ws2811_fini(&ledstring);
    return 0; 
}