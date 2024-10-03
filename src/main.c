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

int main(void) {
    Initialize();

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
    Print_All();

    return 0;
}