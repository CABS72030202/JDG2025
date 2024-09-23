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
    curr_station = &null_station;  
}

void Print_Count(int* array) {
    printf("Red:%i\nGreen:%i\nBlue:%i\nYellow:%i\nPurple:%i\n", array[0], array[1], array[2], array[3], array[4]);
}

int Connect_Station(Color station_color) {
    // Get corresponding station from specified color
    Station* temp = Color_To_Station(station_color);

    // Detect and abord if station is inactive
    if(temp->state == INACTIVE) {
        printf("Station %s is inactive.", Color_To_String(station_color));
        return ERROR; 
    }

    // Change current station
    curr_station = temp;
    return OK;
}

int Drop_Passengers(Color drop_station) {
    // Store current station
    Station* temp = curr_station;

    // Detect and abort if error detected while connecting to drop station
    if(Connect_Station(drop_station))
        return ERROR;

    // Send all passengers to station
    char* response = Communication(Format_Str("SEND", drop_station, blackbox_pass[drop_station]));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        curr_station = temp;        // Reconnect to current station
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", blackbox_pass[drop_station], Color_To_String(drop_station));
        Info_Blackbox_Count();      // Update blackbox count
        curr_station = temp;        // Reconnect to current station
        return OK;
    }
}

int Load_Passengers(Color c, int nb) {
    // Update arrays and abort if error detected
    if(Info_Blackbox_Count() || Info_Station_Count())
        return ERROR;

    // Test parameter consistency
    int sum = 0;
    for(int i = 0; i < 5; i++)
        sum += blackbox_pass[0];
    if(blackbox_pass[c] < nb || sum + nb > 5)
        return ERROR;

    // Take passengers from station
    char* response = Communication(Format_Str("TAKE", c, nb));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Color_To_String(c), Color_To_String(c), response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Color_To_String(c), Color_To_String(c));
        Info_Blackbox_Count();      // Update blackbox count
        Info_Station_Count();       // Update station count
        return OK;
    }
}

char* Communication(char* sent) {
    char* response = Response_Simulation(sent);
    return response;  // Caller must free the memory when done
}

char* Response_Simulation(char* sent) {
    char* response = (char*)malloc(100);  // Allocate memory for the response

    if (response == NULL) {  // Check if memory allocation was successful
        printf("Memory allocation failed.\n");
        return NULL;
    }

    const char *infoResponses[] = {
        "OK",
        "ERR:command_too_long",
        "ERR:command_in_progress",
        "ERR:unknown_command",
        "ERR:disconnected"
    };

    const char *sendResponses[] = {
        "OK",
        "ERR:invalid_color",
        "ERR:invalid_count",
        "ERR:not_enough_people_to_send",
        "ERR:not_enough_room_at_station"
    };

    const char *takeResponses[] = {
        "OK",
        "ERR:invalid_color",
        "ERR:invalid_count",
        "ERR:not_enough_room",
        "ERR:not_enough_people_at_station"
    };

    // Display the sent string
    printf("\n--------------------------\n");
    printf("Sent message: %s\n", sent);

    // Determine the response options based on the first four characters
    if (strncmp(sent, "INFO", 4) == 0) {
        printf("Select a response:\n");
        for (int i = 0; i < sizeof(infoResponses) / sizeof(infoResponses[0]); i++) {
            printf("%d: %s\n", i + 1, infoResponses[i]);
        }
    } else if (strncmp(sent, "SEND", 4) == 0) {
        printf("Select a response:\n");
        for (int i = 0; i < sizeof(sendResponses) / sizeof(sendResponses[0]); i++) {
            printf("%d: %s\n", i + 1, sendResponses[i]);
        }
    } else if (strncmp(sent, "TAKE", 4) == 0) {
        printf("Select a response:\n");
        for (int i = 0; i < sizeof(takeResponses) / sizeof(takeResponses[0]); i++) {
            printf("%d: %s\n", i + 1, takeResponses[i]);
        }
    } else {
        printf("Unknown command type.\n");
        strncpy(response, "ERR:unknown_command\nLa commande envoyée n’est pas reconnue", 100);
        return response;
    }

    // Get user selection
    int choice;
    printf("Enter your choice (1-%d): ", 
           (strncmp(sent, "INFO", 4) == 0) ? (sizeof(infoResponses) / sizeof(infoResponses[0])) : 
           (strncmp(sent, "SEND", 4) == 0) ? (sizeof(sendResponses) / sizeof(sendResponses[0])) : 
           (sizeof(takeResponses) / sizeof(takeResponses[0])));
    scanf("%d", &choice);
    getchar(); // Clear newline character from the buffer

    // Validate choice
    if (choice < 1 || choice > ((strncmp(sent, "INFO", 4) == 0) ? (sizeof(infoResponses) / sizeof(infoResponses[0])) : 
                                 (strncmp(sent, "SEND", 4) == 0) ? (sizeof(sendResponses) / sizeof(sendResponses[0])) : 
                                 (sizeof(takeResponses) / sizeof(takeResponses[0])))) {
        printf("Invalid choice.\n");
        strncpy(response, "ERR:invalid_choice\nChoix invalide.", 100);
        return response;
    }

    // Prepare the response based on the user's selection
    if (strncmp(sent, "INFO", 4) == 0) {
        strncpy(response, infoResponses[choice - 1], 100);
        if (response[0] != 'E') {
        switch(sent[5]) { 
            case 'C':  
                strcpy(response, "OK:"); 
                strcat(response, Color_To_String(curr_station->color)); 
                strcat(response, "\n");
                break;
            case 'B':
                snprintf(response, 13, "OK:%i:%i:%i:%i:%i\n", blackbox_pass[0], blackbox_pass[1], blackbox_pass[2], blackbox_pass[3], blackbox_pass[4]);
                break;
            case 'S':
                snprintf(response, 13, "OK:%i:%i:%i:%i:%i\n", curr_station->passengers[0], curr_station->passengers[1], curr_station->passengers[2], curr_station->passengers[3], curr_station->passengers[4]);
                break;    
        }
    }
    } else if (strncmp(sent, "SEND", 4) == 0 || strncmp(sent, "TAKE", 4) == 0) {
        strncpy(response, (strncmp(sent, "SEND", 4) == 0) ? sendResponses[choice - 1] : takeResponses[choice - 1], 100);
    }

    // Reallocate memory to match the actual response size
    size_t actualSize = strlen(response) + 1;  // +1 for the null terminator
    char* temp = realloc(response, actualSize);
    if (temp == NULL) {
        // If realloc fails, return the original response
        printf("Reallocation failed, returning original response.\n");
        return response;
    }
    response = temp;

    // Return the selected response
    printf("Received message: %s", response);
    return response;
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
        printf("\nERROR. INVALID PARAMETER | station.c | Color_To_String(Color)\n\n");
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
            printf("\nERROR. INVALID PARAMETER | station.c | Color_To_Station(Color)\n\n");
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
        printf("\nERROR. INVALID COLOR | station.c | Info_Color\n\n");
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
        printf("\nERROR. INVALID COLOR | station.c | Info_Station_Count\n\n");
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
        printf("\nERROR. INVALID COLOR | station.c | Format_Str\n\n");
        return NULL;
    }
    return str;
}