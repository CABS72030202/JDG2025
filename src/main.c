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

    /*printf("\n\nSEND 2 GREEN FROM R\n");
    printf("\n\n1. Connect to RED\n");
    Connect_Station(RED);
    printf("\n\n2. Take 2\n");
    Load_Passengers(GREEN, Get_Max_Count(&r_station, &g_station));
    printf("\n\n3. Connect to GREEN\n");
    Connect_Station(GREEN);
    printf("\n\n4. Drop 2\n");
    Drop_Passengers(GREEN);*/

    Auto_Load_Drop();

    Print_Count("Blackbox", blackbox_pass);
    Print_Count("RED", r_station.passengers);
    Print_Count("GREEN", g_station.passengers);
    Print_Count("BLUE", b_station.passengers);
    Print_Count("YELLOW", y_station.passengers);
    Print_Count("PURPLE", p_station.passengers);

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
    r_station.passengers[2] = 2;    // B
    r_station.passengers[3] = 2;    // Y
    r_station.passengers[4] = 2;    // P

    // Green station
    g_station.state = INACTIVE;
    g_station.passengers[0] = 1;    // R
    g_station.passengers[2] = 3;    // B
    g_station.passengers[3] = 4;    // Y
    g_station.passengers[4] = 0;    // P

    // Blue station
    b_station.state = ACTIVE;
    b_station.passengers[0] = 3;    // R
    b_station.passengers[1] = 2;    // G
    b_station.passengers[3] = 2;    // Y
    b_station.passengers[4] = 1;    // P

    // Yellow station
    y_station.state = ACTIVE;
    y_station.passengers[0] = 0;    // R
    y_station.passengers[1] = 0;    // G
    y_station.passengers[2] = 4;    // B
    y_station.passengers[4] = 2;    // P

    // Purple station
    p_station.state = ACTIVE;
    p_station.passengers[0] = 3;    // R
    p_station.passengers[1] = 3;    // G
    p_station.passengers[2] = 1;    // B
    p_station.passengers[3] = 1;    // Y
}