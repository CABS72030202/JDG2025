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
int active_count = 0;
int GPIO_command = 0;

void Initialize() {
    r_station.color = RED;
    Set_Station_State(&r_station, INACTIVE); 
    r_station.arm_state = INACTIVE;      
    g_station.color = GREEN;
    Set_Station_State(&g_station, INACTIVE); 
    g_station.arm_state = INACTIVE;
    b_station.color = BLUE;
    Set_Station_State(&b_station, INACTIVE); 
    b_station.arm_state = INACTIVE;
    y_station.color = YELLOW;
    Set_Station_State(&y_station, INACTIVE); 
    y_station.arm_state = INACTIVE;
    p_station.color = PURPLE;
    Set_Station_State(&p_station, INACTIVE);  
    p_station.arm_state = INACTIVE;
    null_station.color = NONE;
    Set_Station_State(&null_station, INACTIVE); 
    null_station.arm_state = INACTIVE;
    curr_station = &null_station;  

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed\n");
        return -1;
    }

    Initialize_GPIO();

    if(DEBUG_MODE) {
        printf("WARNING. DEBUG MODE IS ON.\n");
        Fictive_State();
        Delay(TIMEOUT);
    }
    else
        while(Start_Comm()) 
            Delay(TIMEOUT);
}

void Wait_For_Two() {
    while(active_count < 2) {
        int temp = 0;
        if(GPIO_command != 0)
        switch(GPIO_command) {
            case RED_READY:     temp = Try_Connect(&r_station); break;
            case GREEN_READY:   temp = Try_Connect(&g_station); break;
            case BLUE_READY:    temp = Try_Connect(&b_station); break;
            case YELLOW_READY:  temp = Try_Connect(&y_station); break;
            case PURPLE_READY:  temp = Try_Connect(&p_station); break;
            default:            temp = ERROR; break;
        }
        if(temp == OK) {
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

int Connect_Station(Station* s) {
    // Detect and abord if station is inactive
    if(s->state == INACTIVE) {
        printf("Station %s is inactive.\n", Color_To_String(s->color));
        return ERROR; 
    }

    // Wait until connection is established
    if(Try_Connect(s) == ERROR)
        return ERROR;

    // Change current station
    curr_station = s;
    return OK;
}

int Try_Connect(Station* s) {
  printf("Trying to connect to station %s. ", Color_To_String(s->color));
  for(int i = 0; i < MAX_ITERATIONS; i++) {
    
    // Lift up corresponding arm
    Arm_Control(s, ACTIVE);
    Delay(1);
    
    // Test connection by getting color 
    Color c = Info_Color();

    // Validate if color matches station
    if(s->color != c) {
        printf("Error. Color (%s) does not match station (%s).\n", Color_To_String(c), Color_To_String(s->color));
        return ERROR;
    }

    // Set active station if no errors
    if(c != NONE) {
        curr_station = s;
        Set_Station_State(curr_station, ACTIVE); 
        printf("Successfully connected to station %s.\n", Color_To_String(curr_station->color));
        return OK;
    }

    // Try again after delay if unsuccessful
    Arm_Control(s, INACTIVE);
    Delay(2);
  }
  printf("Could not connect to the station.\n");
  Set_Station_State(s, INACTIVE); 
  return ERROR;
}

int Drop_Passengers(Station* drop_station) {
    // Send all passengers to station
    char* response = Communication(Format_Str("SEND", drop_station->color, blackbox_pass[drop_station->color]));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot send passengers to station : %s", response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully sent %i passengers to %s station\n", blackbox_pass[drop_station->color], Color_To_String(drop_station->color));

        // Update array
        blackbox_pass[drop_station->color] = 0;
        return OK;
    }
}

int Load_Passengers(Station* load_station, int nb) {
    // Update arrays and abort if error detected
    if(Info_Blackbox_Count() || Info_Station_Count())
        return ERROR;

    // Test parameter consistency
    int sum = 0;
    for(int i = 0; i < 5; i++)
        sum += blackbox_pass[i];
    if(sum + nb > 5)
        return ERROR;

    // Take passengers from station
    char* response = Communication(Format_Str("TAKE", load_station->color, nb));

    // Detect and print error
    if(response[0] == 'E') {
        printf("Cannot take %i passengers going to %s from %s : %s", nb, Color_To_String(load_station->color), Color_To_String(curr_station->color), response);
        return ERROR;       
    }

    // Print confirmation message if successful
    else {
        printf("\nSuccessfully took %i passengers going to %s from %s\n", nb, Color_To_String(load_station->color), Color_To_String(curr_station->color));
        
        // Update arrays
        blackbox_pass[load_station->color] += nb;
        curr_station->passengers[load_station->color] -= nb;

        // Delay to show passengers onboard
        Delay(ONBOARD_TIMEOUT);
        
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

            // 1. Connect to load station and skip if inactive or if a new station is ready
            if(GPIO_command != 0)
                break;
            if(Connect_Station(array[load_station_id]) == ERROR)
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
            if(Load_Passengers(array[drop_station_id], nb) == ERROR)
                return;

            // 4. Connect to drop station
            if(Connect_Station(array[drop_station_id]) == ERROR)
                return;

            // 5. Drop passengers
            if(Drop_Passengers(array[drop_station_id]) == ERROR)
                return;
        }
        // Change load station
        load_station_id++;
        if(load_station_id >= 5)
            load_station_id = 0;

        // If a new station is ready, prioritize this one
        if(GPIO_command != 0) {
            switch(GPIO_command) {
                case RED_READY:     load_station_id = 0; break;
                case GREEN_READY:   load_station_id = 1; break;
                case BLUE_READY:    load_station_id = 2; break;
                case YELLOW_READY:  load_station_id = 3; break;
                case PURPLE_READY:  load_station_id = 4; break;
                default: break;
            }
            Set_Station_State(array[load_station_id], ACTIVE); 
            printf("! Prioritizing station %s !\n", Color_To_String(array[load_station_id]->color));
            GPIO_command = 0;
        }

        // Reset drop station (always start at the following station)
        drop_station_id = load_station_id + 1;
        if(drop_station_id >= 5)
            drop_station_id = 0;
    }
}

void Arm_Control(Station* station, State toggle) {

    // Detect newly available stations
    int temp = Read_Arm_BIN();
    switch(temp) {
        case RED_READY:     
        case GREEN_READY:   
        case BLUE_READY:    
        case YELLOW_READY:  
        case PURPLE_READY:  GPIO_command = temp; break;
        default: break;
    }
    
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
    station->arm_state = toggle;
    switch(station->color) {
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

void Set_Station_State(Station* station, State state) {
    station->state = state;
    switch(station->color) {
        case RED:       digitalWrite(R_QUEUE_PIN, state); break;
        case GREEN:     digitalWrite(B_QUEUE_PIN, state); break;
        case BLUE:      digitalWrite(G_QUEUE_PIN, state); break;
        case YELLOW:    digitalWrite(Y_QUEUE_PIN, state); break;
        case PURPLE:    digitalWrite(P_QUEUE_PIN, state); break;
        case NONE:      break;
        default:        printf("ERROR. Invalid parameter in Set_Station_State\n"); break;
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
    Set_Station_State(&r_station, ACTIVE);
    r_station.passengers[1] = 2;    // G
    r_station.passengers[2] = 2;    // B
    r_station.passengers[3] = 2;    // Y
    r_station.passengers[4] = 2;    // P

    // Green station
    Set_Station_State(&g_station, INACTIVE);
    g_station.passengers[0] = 1;    // R
    g_station.passengers[2] = 3;    // B
    g_station.passengers[3] = 4;    // Y
    g_station.passengers[4] = 0;    // P

    // Blue station
    Set_Station_State(&b_station, ACTIVE);
    b_station.passengers[0] = 3;    // R
    b_station.passengers[1] = 2;    // G
    b_station.passengers[3] = 2;    // Y
    b_station.passengers[4] = 1;    // P

    // Yellow station
    Set_Station_State(&y_station, ACTIVE);
    y_station.passengers[0] = 0;    // R
    y_station.passengers[1] = 0;    // G
    y_station.passengers[2] = 4;    // B
    y_station.passengers[4] = 2;    // P

    // Purple station
    Set_Station_State(&p_station, ACTIVE);
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