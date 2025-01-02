/*
 * communication.h
 * Created on: 2024-11-04
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file provides the interface for serial communication between the robotic 
 * solution and the blackbox. It defines constants, global variables, and functions 
 * to initialize communication, send and receive data, and manage communication buffers. 
 * It also includes utility functions for filtering and formatting received data.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Includes
#include <wiringSerial.h>
#include <time.h>
#include "../lib/constants.h"
#include "../lib/station.h"

// Global Variables
extern int fd;                         // File descriptor for the serial port
extern char* sent[BUFFER_SIZE];        // Buffer for sent messages
extern char* received[BUFFER_SIZE];    // Buffer for received messages

// Prototypes
int Start_Comm();                      // Initialize the serial communication
int Send(char*);                       // Send a message over the serial connection
int Receive(char*);                    // Receive a message from the serial connection
void Filter_Reception();               // Filter and process received data
void Empty_String(char*);              // Clear the contents of a string
void Delay(int);                       // Pause execution for a specified number of milliseconds

#endif