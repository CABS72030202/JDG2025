// main.c
// Created on: 2024-09-14
// Author: Sebastien Cabana

/* 
 * This code defines the functions needed to manage passengers between the machine 
 * and the stations, without considering communication with the blackbox. 
 * The responses are predefined to allow debugging without requiring data from the 
 * blackbox or the stations.
 */

#include<stdio.h>
#include<string.h>

enum color {RED, GREEN, BLUE, YELLOW, PURPLE, NONE};
typedef enum color Color;

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
int r_station_pass[5];
int g_station_pass[5];
int b_station_pass[5];
int y_station_pass[5];
int p_station_pass[5];
int blackbox_pass[5];
Color curr_station;
Color next_station;

// Prototypes
void Initialize();
char* Print_Color(Color);
void Print_Count(int*);
int Update_Station_Color();
int Update_Station_Count();
int Update_Blackbox_Count();
char* Format_Str(char*, Color, int);
int Drop_Passengers();
int Load_Passengers(Color, int);

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

char* Print_Color(Color c) {
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
        printf("\nERROR. INVALID COLOR | main.c | Print_Color\n\n");
        return NULL;
    }
}
void Print_Count(int* array) {
    printf("Red:%i\nGreen:%i\nBlue:%i\nYellow:%i\nPurple:%i\n", array[0], array[1], array[2], array[3], array[4]);
}
int Update_Station_Color() {
    char* response = Response("INFO:C\n");
    if (response[0] == 'E') {
        printf("Cannot update current station color : %s", response);
        curr_station = NONE;
        return ERROR;
    }
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
    char* response = Response("INFO:S\n");
    if (response == NULL || Update_Station_Color() == ERROR)
        return ERROR;
    switch (curr_station)
    {
    case RED:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            r_station_pass[i] = response[j] - '0';
        return OK;
    case GREEN:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            g_station_pass[i] = response[j] - '0';
        return OK;
    case BLUE:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            b_station_pass[i] = response[j] - '0';
        return OK;
    case YELLOW:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            y_station_pass[i] = response[j] - '0';
        return OK;
    case PURPLE:
        for (int i = 0, j = 3; j < strlen(response); i++, j += 2)
            p_station_pass[i] = response[j] - '0';
        return OK;
    case NONE:
        return ERROR;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Update_Station_Count\n\n");
        return ERROR;
    }
}
int Update_Blackbox_Count() {
    char* response = Response("INFO:B\n");
    if (response == NULL || response[0] == 'E') {
        printf("Cannot update blackbox passenger count : %s", response);
        return ERROR;
    }
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
        sprintf(str, "%s:%s:%i\n", comm, Print_Color(c), nb);
        break;
    case GREEN:
        size += 5;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Print_Color(c), nb);
        break;
    case BLUE:
        size += 4;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Print_Color(c), nb);
        break;
    case YELLOW:
        size += 6;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Print_Color(c), nb);
        break;
    case PURPLE:
        size += 6;
        str = (char*)malloc(size * sizeof(char));
        sprintf(str, "%s:%s:%i\n", comm, Print_Color(c), nb);
        break;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Format_Str\n\n");
        return NULL;
    }
    return str;
}
int Drop_Passengers() {
    if(Update_Blackbox_Count() || Update_Station_Color())
        return ERROR;
    char* response = Response(Format_Str("SEND", curr_station, blackbox_pass[curr_station]));
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        return ERROR;       
    }
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", blackbox_pass[curr_station], Print_Color(curr_station));
        blackbox_pass[curr_station] = 0;
        return OK;
    }
}
int Load_Passengers(Color c, int nb) {
    if(Update_Blackbox_Count() || Update_Station_Color())
        return ERROR;
    char* response = Response(Format_Str("TAKE", c, nb));
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Print_Color(c), Print_Color(curr_station), response);
        return ERROR;       
    }
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Print_Color(c), Print_Color(curr_station));
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