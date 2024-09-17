// main.c
// Created on: 2024-09-14
// Author: Sebastien Cabana

/* 
 * This code implements the necessary functions for managing passenger interactions 
 * between the machine and the stations, excluding communication with the black box. 
 * Responses are pre-defined to allow code compilation and debugging without the 
 * inputs from any external components.
 */

#include<stdio.h>
#include<string.h>
#include "station.c"

/*enum color {RED, GREEN, BLUE, YELLOW, PURPLE, NONE};
typedef enum color Color;*/

/* DELETE THIS SECTION ONCE BLACKBOX CONNECTION IS FUNCTIONNAL */
// Predefined blackbox responses (fake infos)
const char station_pass_str[] = "OK:1:0:4:1:2\n";    // R:G:B:Y:P
const char blackbox_pass_str[] = "OK:0:2:1:1:0\n";   // R:G:B:Y:P
const char station_color_str[] = "OK:GREEN\n";
const char expected_take_str[] = "TAKE:PURPLE:1\n";
const char expected_send_str[] = "SEND:GREEN:2\n";
char* Response(char*);                               // Simulates communication (sending string and receiving string)
/* DELETE THIS SECTION ONCE BLACKBOX CONNECTION IS FUNCTIONNAL */

// Global variables and constants
const int ERROR = 1;
const int OK = 0;    
Station r_station;      
Station g_station;     
Station b_station;      
Station y_station;      
Station p_station;      
int blackbox_pass[5];       // Passenger count for each destination onboard
Color curr_station;         // Current station (NONE if not properly connected to one)

// Prototypes
char* Color_To_String(Color);           // Get string format from enum Color
void Print_Count(int*);                 // Print the number of passenger going to each station from an array of size 5
int Update_Station_Color();             // Get current station color
int Update_Station_Count();             // Get current station passenger count
int Update_Blackbox_Count();            // Get current number of passenger onboard
char* Format_Str(char*, Color, int);    // 
int Drop_Passengers();                  // Drop all corresponding passengers onboard to current station
int Load_Passengers(Color, int);        // Load a specific number of passengers going to a specific station

int main() {
    printf("----- start main -----\n\n");
    Update_Blackbox_Count();
    Print_Count(blackbox_pass);
    Drop_Passengers();
    Print_Count(blackbox_pass);
    Load_Passengers(PURPLE, 1);
    Print_Count(blackbox_pass);
    printf("\n----- end main -----\n");
    return OK;
}

char* Color_To_String(Color c) {
    switch (c)
    {
    case RED:
        return "RED";
    case GREEN:
        return "GREEN";
    case BLUE:
        return "BLUE";
    case YELLOW:
        return "YELLOW";
    case PURPLE:
        return "PURPLE";
    case NONE:
        return "NONE";
    default:
        printf("\nERROR. INVALID COLOR | main.c | Color_To_String\n\n");
        return NULL;
    }
}
void Print_Count(int* array) {
    printf("Red:%i\nGreen:%i\nBlue:%i\nYellow:%i\nPurple:%i\n", array[0], array[1], array[2], array[3], array[4]);
}
int Update_Station_Color() {
    // Get message from blackbox using predefined INFO function
    char* response = Response("INFO:C\n");

    // Detect and print error
    if (response[0] == 'E') {
        printf("Cannot update current station color : %s", response);
        curr_station = NONE;
        return ERROR;
    }

    // Decode message and set current station
    char c = response[3];
    switch (c)
    {
    case 'R':
        curr_station = RED;
        return OK;
    case 'G':
        curr_station = GREEN;
        return OK;
    case 'B':
        curr_station = BLUE;
        return OK;
    case 'Y':
        curr_station = YELLOW;
        return OK;
    case 'P':
        curr_station = PURPLE;
        return OK;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Update_Station_Color\n\n");
        break;
    }
}
int Update_Station_Count() {
    // Get message from blackbox using predefined INFO function
    char* response = Response("INFO:S\n");

    // Detect and print error
    if (response == NULL || Update_Station_Color() == ERROR)
        return ERROR;

    // Decode message and update passenger count array for current station
    switch (curr_station)
    {
    case RED:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            r_station.passengers[i] = response[j] - '0';
        return OK;
    case GREEN:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            g_station.passengers[i] = response[j] - '0';
        return OK;
    case BLUE:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            b_station.passengers[i] = response[j] - '0';
        return OK;
    case YELLOW:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            y_station.passengers[i] = response[j] - '0';
        return OK;
    case PURPLE:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            p_station.passengers[i] = response[j] - '0';
        return OK;
    case NONE:
        return ERROR;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Update_Station_Count\n\n");
        return ERROR;
    }
}
int Update_Blackbox_Count() {
    // Get message from blackbox using predefined INFO function
    char* response = Response("INFO:B\n");

    // Detect and print error
    if (response == NULL || response[0] == 'E') {
        printf("Cannot update blackbox passenger count : %s", response);
        return ERROR;
    }

    // Decode message and update passenger count array for blackbox
    for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
        blackbox_pass[i] = response[j] - '0';
    return OK;
}
char* Format_Str(char* comm, Color c, int nb) {
    int size = 8;       // Accounting for TAKE||SEND, two ':', '\n' and passenger count
    char* str;
    switch (c)
    {
    case RED:
        size += 3;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Color_To_String(c), nb);
        break;
    case GREEN:
        size += 5;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Color_To_String(c), nb);
        break;
    case BLUE:
        size += 4;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Color_To_String(c), nb);
        break;
    case YELLOW:
        size += 6;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Color_To_String(c), nb);
        break;
    case PURPLE:
        size += 6;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Color_To_String(c), nb);
        break;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Format_Str\n\n");
        return NULL;
    }
    return str;
}
int Drop_Passengers() {
    // Detect and abort if error detected
    if(Update_Blackbox_Count() || Update_Station_Color())
        return ERROR;

    // Get message from blackbox using predefined SEND function
    char* response = Response(Format_Str("SEND", curr_station, blackbox_pass[curr_station]));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", blackbox_pass[curr_station], Color_To_String(curr_station));
        blackbox_pass[curr_station] = 0;
        return OK;
    }
}
int Load_Passengers(Color c, int nb) {
    // Detect and abort if error detected
    if(Update_Blackbox_Count() || Update_Station_Color())
        return ERROR;

    // Get message from blackbox using predefined TAKE function
    char* response = Response(Format_Str("TAKE", c, nb));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Color_To_String(c), Color_To_String(curr_station), response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Color_To_String(c), Color_To_String(curr_station));
        blackbox_pass[c] += nb;
        return OK;
    }
}
char* Response(char* sent) {
    switch (sent[0])
    {
    case 'T':
        return expected_take_str;
    
    case 'S':
        return expected_send_str;

    case 'I':
        switch(sent[5]) {
            case 'S':
                return station_pass_str;
            case 'B':
                return blackbox_pass_str;
            case 'C':
                return station_color_str;
            default:
                printf("\nERROR. INVALID INFO CHAR | main.c | Response(char* send)\n\n");
                return NULL;
        }
    break;

    default:
        printf("\nERROR. INVALID MESSAGE | main.c | Response(char* send)\n\n");
        return NULL;
    }
}