// station.h
// Created on: 2024-09-17
// Author: Sebastien Cabana

/* 
 * This code defines the attributes and functions specific to the stations, 
 * as well as the algorithm to automate the loading and unloading of passengers 
 * between stations. The algorithm starts once two stations are connected to 
 * our robotic solution and runs until the end of the on-ground trial. This 
 * code also includes all the necessary functions to enable communication 
 * between our robotic solution and the embedded black box. Note that this project
 * requires its own microcontroller.
 */

#ifndef STATION_H
#define STATION_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

// Debugging parameters (1 is yes, 0 is no)
#define DEBUG_MODE 1
#define ACTIVE_LOOP 1                      
#define MAX_ITERATIONS 1000
#define USER_INPUT 0               

// Enum and struct
enum color {RED, GREEN, BLUE, YELLOW, PURPLE, NONE};
typedef enum color Color;
enum state {INACTIVE, ACTIVE};
typedef enum state State;
typedef struct {
    Color color;        // Station color
    int passengers[5];  // R:G:B:Y:P
    State state;        // ACTIVE when connected
} Station;

// Global variables and constants
#define ERROR 1
#define OK 0
extern Station r_station;      
extern Station g_station;     
extern Station b_station;      
extern Station y_station;      
extern Station p_station;
extern Station null_station;
extern Station* curr_station;
extern int blackbox_pass[5];       // Passenger count for each destination onboard

// Prototypes (station)
void Initialize();                      // Initialize stations
int Connect_Station(Color);             // Change current station
int Drop_Passengers(Color);             // Drop all corresponding passengers onboard to specific station
int Load_Passengers(Color, int);        // Load a specific number of passengers going to a specific station
void Auto_Load_Drop();                  // Automates the loading and dropping off of passengers at ACTIVE locations

// Prototypes (blackbox)
char* Communication(char*);             // Returns the message received after sending a specific message
char* Color_To_String(Color);           // Get string format from enum Color
Station* Color_To_Station(Color);       // Get corresponding station from enum Color
int Get_Max_Count(Station*, Station*);  // Get the maximum number of passengers for boarding from a station depending on blackbox available seats and destination
Color Info_Color();                     // Get current station color
int Info_Station_Count();               // Update current station passenger array
int Info_Blackbox_Count();              // Update blackbox passenger array
char* Format_Str(char*, Color, int);    // Format station attributes and command in a string

// Prototypes (debugging)
void Fictive_State();                   // Allows the user to set the fictitious state of the model (USED ONLY FOR DEBUGGING)
char* Response_Simulation(char*);       // Lets the user choose the expected response from the blackbox (USED ONLY FOR DEBUGGING)
void Print_Count(char*, int*);          // Print the number of passenger going to each station from an array of size 5
void Print_All();                       // Print the number of passenger going to each station of all stations and blackbox

#endif