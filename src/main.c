// main.c
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
#include "../lib/station.h"
#include "../lib/communication.h"

// Prototypes
void Exit_Error();

int main(void) {
    Initialize();
    Wait_For_Two();
    //Auto_Load_Drop();

    while(1) {
    if(Connect_Station(BLUE) == ERROR)
        Exit_Error();
    if(Load_Passengers(YELLOW, 1) == ERROR)
        Exit_Error();
    if(Connect_Station(YELLOW) == ERROR)
        Exit_Error();
    if(Drop_Passengers(YELLOW) == ERROR)
        Exit_Error();
    }

    /*while (Try_Connect() == OK) {
        Send("INFO:C\n");
        Receive(received);
        printf("Received: %s", received);
    }*/

    return 0; 
}

void Exit_Error() {
    printf("Exit with error.\n");
    exit(1);
}