// station.c
// Created on: 2024-09-17
// Author: Sebastien Cabana

#include "../lib/station.h"

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
    r_station.arm_state = INACTIVE;      
    g_station.color = GREEN;
    g_station.state = INACTIVE;
    g_station.arm_state = INACTIVE;
    b_station.color = BLUE;
    b_station.state = INACTIVE; 
    b_station.arm_state = INACTIVE;
    y_station.color = YELLOW;
    y_station.state = INACTIVE; 
    y_station.arm_state = INACTIVE;
    p_station.color = PURPLE;
    p_station.state = INACTIVE; 
    p_station.arm_state = INACTIVE;
    null_station.color = NONE;
    null_station.state = INACTIVE;
    null_station.arm_state = INACTIVE;
    curr_station = &null_station;  

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed\n");
        return -1;
    }

    Initialize_GPIO();

    if(DEBUG_MODE)
        Fictive_State();
    else
        while(Start_Comm()) 
            Delay(TIMEOUT);
}

void Wait_For_Two() {
    int active_count = 0;
    while(active_count < 2) {
        if(Try_Connect() == OK) {
            active_count = 0;
            // Count active stations
            if(r_station.state == ACTIVE)
                active_count++;
            if(g_station.state == ACTIVE)
                active_count++;
            if(b_station.state == ACTIVE)
                active_count++;
            if(y_station.state == ACTIVE)
                active_count++;
            if(p_station.state == ACTIVE)
                active_count++;
            printf("There are currently %i active stations.", active_count);
        }
        else
            Delay(TIMEOUT);
    }
}

int Connect_Station(Color station_color) {
    // Lift up corresponding arm
    Arm_Control(station_color, ACTIVE);

    // Wait until connection is established
    if(Try_Connect() == ERROR)
        return ERROR;

    // Get corresponding station from specified color
    Station* temp = Color_To_Station(station_color);

    // Detect and abord if station is inactive
    if(temp->state == INACTIVE) {
        printf("Station %s is inactive.\n", Color_To_String(station_color));
        return ERROR; 
    }

    // Change current station
    curr_station = temp;
    return OK;
}

int Drop_Passengers(Color drop_station) {
    // Send all passengers to station
    char* response = Communication(Format_Str("SEND", drop_station, blackbox_pass[drop_station]));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", blackbox_pass[drop_station], Color_To_String(drop_station));

        // Update array
        blackbox_pass[drop_station] = 0;
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
    if(sum + nb > 5)
        return ERROR;

    // Take passengers from station
    char* response = Communication(Format_Str("TAKE", c, nb));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Color_To_String(c), Color_To_String(curr_station->color), response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Color_To_String(c), Color_To_String(curr_station->color));
        
        // Update arrays
        blackbox_pass[c] += nb;
        curr_station->passengers[c] -= nb;

        // Delay to show passengers onboard
        Delay(TIMEOUT);
        
        return OK;
    }
}

void Auto_Load_Drop() {

    /* 
    * The general operation of the algorithm is as follows: each station is completely emptied 
    * one at a time. Two loops allow switching between stations and returning after each iteration. 
    * Once a station is empty, the algorithm moves to the next one. However, inactive stations 
    * are excluded from this process. For example: starting at the red station, we pick up all 
    * passengers heading to the green station, then return to the red station to take all passengers 
    * going to the blue station, and so on until the red station is empty. This process is then 
    * repeated for the green station, and so forth. Note that the defined order is R:G:B:Y:P.
    */

    // Exit if deactivated loop
    if(!ACTIVE_LOOP)
        return;

    // Define variables
    int load_station_id;        // Station ID currently being emptied
    int drop_station_id;        // Current dropping station ID
    int cpt1 = 0;               // Count loop iterations
    int cpt2 = 0;               // Count loop iterations
    int nb = 0;
    Station* array[5];          // Array of pointers to all stations

    load_station_id = 0;        // Start emptying at RED
    drop_station_id = 1;        // Start dropping at GREEN
    array[0] = &r_station;
    array[1] = &g_station;
    array[2] = &b_station;
    array[3] = &y_station;
    array[4] = &p_station;

    while(1) {
        for(int i = 0; i < 4; i++) {

            // Exit after max iterations
            cpt1++;
            if(cpt1 == MAX_ITERATIONS)
                return;

            // 1. Connect to load station and skip if inactive
            if(Connect_Station(array[load_station_id]->color) == ERROR)
                break;

            // 2. Find next active/relevant drop station
            cpt2 = 0;
            while(array[drop_station_id]->state == INACTIVE || array[load_station_id]->passengers[drop_station_id] == 0) {
                drop_station_id++;
                cpt2++;
                if(drop_station_id >= 5)
                    drop_station_id = 0;
                // Exit loop if none found
                if(cpt2 >= 5)
                    break;
            }
                // Exit loop if none found
                if(cpt2 >= 5)
                    break;

            // 3. Load passengers
            nb = Get_Max_Count(array[load_station_id], array[drop_station_id]);
            if(Load_Passengers(array[drop_station_id]->color, nb) == ERROR)
                return;

            // 4. Connect to drop station
            if(Connect_Station(array[drop_station_id]->color) == ERROR)
                return;

            // 5. Drop passengers
            if(Drop_Passengers(array[drop_station_id]->color) == ERROR)
                return;
        }
        // Change load station
        load_station_id++;
        if(load_station_id >= 5)
            load_station_id = 0;

        // Reset drop station (always start at the following station)
        drop_station_id = load_station_id + 1;
        if(drop_station_id >= 5)
            drop_station_id = 0;
    }
}

void Arm_Control(Color station_color, State toggle) {
    
    // Deactivate any active arm
    if(r_station.arm_state == ACTIVE) {
        Write_Arm_DEC(RED_DOWN);
        r_station.arm_state = INACTIVE;
    }
    if(g_station.arm_state == ACTIVE) {
        Write_Arm_DEC(GREEN_DOWN);
        g_station.arm_state = INACTIVE;
    }
    if(b_station.arm_state == ACTIVE) {
        Write_Arm_DEC(BLUE_DOWN);
        b_station.arm_state = INACTIVE;
    }
    if(y_station.arm_state == ACTIVE) {
        Write_Arm_DEC(YELLOW_DOWN);
        y_station.arm_state = INACTIVE;
    }
    if(p_station.arm_state == ACTIVE) {
        Write_Arm_DEC(PURPLE_DOWN);
        p_station.arm_state = INACTIVE;
    }
    Delay(1);

    // Activate only one
    Color_To_Station(station_color)->arm_state = toggle;
    switch(station_color) {
        case RED:
            toggle == ACTIVE ? Write_Arm_DEC(RED_UP) : Write_Arm_DEC(RED_DOWN);
            break;
        case GREEN:
            toggle == ACTIVE ? Write_Arm_DEC(GREEN_UP) : Write_Arm_DEC(GREEN_DOWN);
            break;
        case BLUE:
            toggle == ACTIVE ? Write_Arm_DEC(BLUE_UP) : Write_Arm_DEC(BLUE_DOWN);
            break;
        case YELLOW:
            toggle == ACTIVE ? Write_Arm_DEC(YELLOW_UP) : Write_Arm_DEC(YELLOW_DOWN);
            break;
        case PURPLE:
            toggle == ACTIVE ? Write_Arm_DEC(PURPLE_UP) : Write_Arm_DEC(PURPLE_DOWN);
            break;
        case NONE:
        default:
            printf("ERROR. Invalid parameter in Arm_Control(State))\n");
            break;
    }
}

char* Communication(char* sent) {
    if (DEBUG_MODE) {
        char* response = Response_Simulation(sent);
        return response;  // Caller must free the memory when done
    }

    // NOTE : NULL parameter means to wait for message from blackbox
    if(Send(sent) == OK)
        if(Receive(received) == OK)
            return received;

    return NULL;
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
        printf("ERROR. Invalid parameter in Color_To_String(Color)\n");
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
            printf("ERROR. Invalid parameter in Color_To_Station(Color)\n");
            return &null_station;
    }
}

int Get_Max_Count(Station* load_station, Station* drop_station) {
    int available_pass = load_station->passengers[drop_station->color];
    int available_seats = 5;
    for(int i = 0; i < 5; i++)
        available_pass -= blackbox_pass[i];
    return (available_pass > available_seats) ? available_seats : available_pass;
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
        printf("ERROR. Invalid color in Info_Color()\n");
        return NONE;
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
        printf("ERROR. Invalid color in Format_Str(char*, Color, int)\n");
        return NULL;
    }
    return str;
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

char* Response_Simulation(char* sent) {

    // Exit if debug mode is OFF
    if(!DEBUG_MODE) {
        printf("ERROR. Debug mode is OFF.\n");
        return NULL;
    }

    // Allocate memory for the response
    char* response = (char*)malloc(100);  
    if (response == NULL) {               
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
    printf("--------------------------\n");
    printf("Sent message: %s", sent);

    int choice;
    if(USER_INPUT) {
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
        printf("Enter your choice (1-%d): ", 
            (strncmp(sent, "INFO", 4) == 0) ? (sizeof(infoResponses) / sizeof(infoResponses[0])) : 
            (strncmp(sent, "SEND", 4) == 0) ? (sizeof(sendResponses) / sizeof(sendResponses[0])) : 
            (sizeof(takeResponses) / sizeof(takeResponses[0])));
        scanf("%d", &choice);
        getchar(); // Clear newline character from the buffer
    }
    else
        choice = 1;

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
                strcat(response, "\n");
                break;
            case 'S':
                snprintf(response, 13, "OK:%i:%i:%i:%i:%i\n", curr_station->passengers[0], curr_station->passengers[1], curr_station->passengers[2], curr_station->passengers[3], curr_station->passengers[4]);
                strcat(response, "\n");
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

void Print_Count(char* tag, int* array) {
    printf("\nPassengers at %s\nRed:%i\nGreen:%i\nBlue:%i\nYellow:%i\nPurple:%i\n", tag, array[0], array[1], array[2], array[3], array[4]);
}

void Print_All() {
    Print_Count("Blackbox", blackbox_pass);
    Print_Count("RED", r_station.passengers);
    Print_Count("GREEN", g_station.passengers);
    Print_Count("BLUE", b_station.passengers);
    Print_Count("YELLOW", y_station.passengers);
    Print_Count("PURPLE", p_station.passengers);
}