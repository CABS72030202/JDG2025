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
#include "station.h"

// Prototypes
void Fictive_State();                   // Allows the user to set the fictitious state of the model (USED ONLY FOR DEBUGGING)

int main(void) {
    Initialize();
    Fictive_State();

    printf("Connect_Station RED\n");
    Connect_Station(RED);

    Print_Count(blackbox_pass);
    Print_Count(curr_station->passengers);

    printf("Load 2 passengers to GREEN\n");
    Load_Passengers(GREEN, 2);

    printf("Drop 2 passengers to GREEN\n");
    Drop_Passengers(GREEN);

    printf("Connect_Station GREEN\n");
    Connect_Station(GREEN);

    printf("Load 1 passengers to RED\n");
    Load_Passengers(RED, 1);

    printf("Drop 1 passengers to RED\n");
    Drop_Passengers(RED);

    return 0;
}

void Fictive_State() {
    // Blackbox
    blackbox_pass[0] =  0;          // R
    blackbox_pass[1] =  0;          // G
    blackbox_pass[2] =  0;          // B
    blackbox_pass[3] =  0;          // Y
    blackbox_pass[4] =  0;          // P

    // Red station
    r_station.state = ACTIVE;
    r_station.passengers[1] = 2;    // G
    r_station.passengers[2] = 0;    // B
    r_station.passengers[3] = 0;    // Y
    r_station.passengers[4] = 0;    // P

    // Green station
    g_station.state = ACTIVE;
    g_station.passengers[0] = 1;    // R
    g_station.passengers[2] = 0;    // B
    g_station.passengers[3] = 0;    // Y
    g_station.passengers[4] = 0;    // P

    // Blue station
    b_station.state = INACTIVE;
    b_station.passengers[0] = 0;    // R
    b_station.passengers[1] = 0;    // G
    b_station.passengers[3] = 0;    // Y
    b_station.passengers[4] = 0;    // P

    // Yellow station
    y_station.state = INACTIVE;
    y_station.passengers[0] = 0;    // R
    y_station.passengers[1] = 0;    // G
    y_station.passengers[2] = 0;    // B
    y_station.passengers[4] = 0;    // P

    // Purple station
    p_station.state = INACTIVE;
    p_station.passengers[0] = 0;    // R
    p_station.passengers[1] = 0;    // G
    p_station.passengers[2] = 0;    // B
    p_station.passengers[3] = 0;    // Y
}