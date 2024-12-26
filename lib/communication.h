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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pigpio.h>
#include <time.h>
#include "../lib/station.h"

// Global Constants
#define SERIAL_PORT "/dev/ttyUSB0"     // Serial port for communication
#define BAUD_RATE 115200               // Baud rate for serial communication
#define BUFFER_SIZE 16                 // Size of communication buffer
#define TIMEOUT 5                      // Communication timeout in seconds

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