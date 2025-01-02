/*
 * station.h
 * Created on: 2024-09-17
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the attributes, constants, and functions required 
 * for managing stations in the project. It includes the implementation of the 
 * algorithm to automate the loading and unloading of passengers between stations. 
 * The automation begins once two stations are connected to the robotic solution 
 * and continues until the on-ground trial ends. This file also provides the 
 * interface for communication between the robotic solution and the embedded 
 * black box. Note: This project requires a dedicated microcontroller.
 */

#ifndef STATION_H
#define STATION_H

// Includes
#include "../lib/constants.h"
#include "../lib/communication.h"
#include "../lib/gpio.h"

// Enums and Structs
typedef enum color {
	RED, GREEN, BLUE, YELLOW, PURPLE, NONE
} Color;

typedef enum state {
	INACTIVE, ACTIVE
} State;

typedef struct {
	Color color;          // Station color
	int passengers[5];    // Passenger counts: R:G:B:Y:P
	State state;          // Station state: ACTIVE or INACTIVE
	State arm_state;      // Arm state: ACTIVE or INACTIVE
} Station;

// Global Variables
extern Station r_station;       // Red station
extern Station g_station;       // Green station
extern Station b_station;       // Blue station
extern Station y_station;       // Yellow station
extern Station p_station;       // Purple station
extern Station null_station;    // Null station (placeholder)
extern Station* curr_station;   // Pointer to the current station
extern int blackbox_pass[5];    // Passenger count for each destination onboard
extern int active_count;        // Number of active stations
extern int GPIO_command;		// Current GPIO command for station priority

// Function Prototypes (Station)
void Initialize();			        		// Initialize station attributes and states
void Wait_For_Two();                		// Wait for two stations to connect
int Connect_Station(Station*);         		// Connect to a specific station
int Try_Connect(Station*);          		// Attempt to connect to a station
int Drop_Passengers(Station*);         		// Drop passengers to a specific station
int Load_Passengers(Station*, int);			// Load passengers going to a specific station
void Auto_Load_Drop();              		// Automate loading and unloading of passengers
void Arm_Control(Station*, State); 			// Control station arm using GPIO
void Set_Station_State(Station*, State);	// Set the current state of a station

// Function Prototypes (Blackbox Communication)
char* Communication(char*);             // Send a command and receive a response
char* Color_To_String(Color);           // Convert Color enum to string
Station* Color_To_Station(Color);       // Map Color enum to corresponding station
int Get_Max_Count(Station*, Station*);	// Get max passengers for boarding
Color Info_Color();						// Get the color of the current station
int Info_Station_Count();               // Update passenger count for the current station
int Info_Blackbox_Count();              // Update blackbox passenger count
char* Format_Str(char*, Color, int); 	// Format attributes into a string

// Function Prototypes (Debugging)
void Fictive_State();            	// Set fictitious states for testing
char* Response_Simulation(char*);  	// Simulate blackbox response for debugging
void Print_Count(char*, int*);  	// Print passenger counts for destinations
void Print_All();                 	// Print all passenger counts for stations and blackbox

#endif