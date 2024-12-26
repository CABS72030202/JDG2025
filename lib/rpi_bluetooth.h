/*
 * rpi_bluetooth.h
 * Created on: 2024-12-18
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file provides the interface for Bluetooth communication using 
 * the RFCOMM protocol on a Raspberry Pi. It includes constants, dependencies, 
 * and function prototypes necessary for initializing, sending, and receiving 
 * data over Bluetooth. The implementation supports both server and client modes 
 * for flexible communication setups.
 */

#ifndef RPI_BLUETOOTH_H
#define RPI_BLUETOOTH_H

// Includes
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pigpio.h>
#include <errno.h>
#include <string.h>

// Global Constants
#define CHANNEL     1                   // RFCOMM communication channel
#define BUFFER_SIZE 32                  // Buffer size for received messages
#define SERVER_ADR  "B8:27:EB:A9:8C:AB" // Default server Bluetooth address
#define UART        "/dev/serial0"      // UART device path
#define BAUD_RATE   115200              // UART baud rate

// Function Prototypes
int bt_init(int, const char*);          // Initialize Bluetooth as server (1) or client (0)
int bt_send(int, const char*);          // Send a message via Bluetooth
int bt_receive(int, char*, int);        // Receive a message via Bluetooth
int uart_init(const char*, int);        // Initialize UART communication
int uart_send(int, const char*);        // Send data over UART
int uart_receive(int, char*, int);      // Receive data over UART

#endif