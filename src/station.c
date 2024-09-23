// station.c
// Created on: 2024-09-17
// Author: Sebastien Cabana

#include "station.h"

Station r_station;      
Station g_station;     
Station b_station;      
Station y_station;      
Station p_station;
Station null_station;
Station* curr_station;
int blackbox_pass[5];

void Initialize() {
    r_station.color = RED;
    r_station.state = INACTIVE;      
    g_station.color = GREEN;
    g_station.state = INACTIVE;
    b_station.color = BLUE;
    b_station.state = INACTIVE; 
    y_station.color = YELLOW;
    y_station.state = INACTIVE; 
    p_station.color = PURPLE;
    p_station.state = INACTIVE; 
    null_station.color = NONE;
    null_station.state = INACTIVE;  
}

void Print_Count(int* array) {
    printf("Red:%i\nGreen:%i\nBlue:%i\nYellow:%i\nPurple:%i\n", array[0], array[1], array[2], array[3], array[4]);
}

int Connect_Station(Color station_color) {
    // Get corresponding station from specified color
    Station* current = Color_To_Station(station_color);

    // Detect and abord if station is inactive
    if(current->state == INACTIVE) {
        printf("Station %s is inactive.", Color_To_String(station_color));
        return ERROR; 
    }

    // Change current station
    curr_station = current;
    return OK;
}

int Drop_Passengers(Color drop_station) {
    // Detect and abort if error detected while connecting to drop station
    if(Info_Blackbox_Count())
        return ERROR;

    // Get message from blackbox using predefined SEND function
    char* response = Communication(Format_Str("SEND", drop_station, Get_Passenger_Count(drop_station)));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", Get_Passenger_Count(drop_station), Color_To_String(drop_station));
        Info_Blackbox_Count();
        return OK;
    }
}

int Load_Passengers(Color c, int nb) {
    // Detect and abort if error detected
    if(Info_Blackbox_Count() || Info_Color())
        return ERROR;

    // Get message from blackbox using predefined TAKE function
    char* response = Communication(Format_Str("TAKE", c, nb));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Color_To_String(c), Color_To_String(c), response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Color_To_String(c), Color_To_String(c));
        Info_Blackbox_Count();
        return OK;
    }
}

char* Communication(char* sent) {
    char* received = "ERROR_TEST";
    return received;
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

Station* Color_To_Station(Color c) {
    switch(c) {
        case RED:
            return &r_station;
        case GREEN:
            return &g_station;
        case BLUE:
            return &b_station;
        case YELLOW:
            return &y_station;
        case PURPLE:
            return &p_station;
        case NONE:
        default:
            printf("\nERROR. INVALID PARAMETER | main.c | Connect_Station(Color station)\n\n");
            return &null_station;
    }
}

Color Info_Color() {
    // Get message from blackbox using predefined INFO function
    char* response = Communication("INFO:C\n");

    // Detect and print error
    if (response[0] == 'E') {
        printf("Cannot get current station color : %s", response);
        return NONE;
    }

    // Decode message and set current station
    char c = response[3];
    switch (c)
    {
    case 'R':
        return RED;
    case 'G':
        return GREEN;
    case 'B':
        return BLUE;
    case 'Y':
        return YELLOW;
    case 'P':
        return PURPLE;
    default:
        printf("\nERROR. INVALID COLOR | main.c | Update_Station_Color\n\n");
        break;
    }
}

int Info_Station_Count() {
    // Get message from blackbox using predefined INFO function
    char* response = Communication("INFO:S\n");

    // Detect and print error
    if (response == NULL)
        return ERROR;

    // Decode message and update passenger count array for current station
    switch (Info_Color())
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
        printf("\nERROR. INVALID COLOR | main.c | Info_Station_Count\n\n");
        return ERROR;
    }
}

int Info_Blackbox_Count() {
    // Get message from blackbox using predefined INFO function
    char* response = Communication("INFO:B\n");

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

const int Get_Passenger_Count(Color c) {
    return blackbox_pass[c];
}