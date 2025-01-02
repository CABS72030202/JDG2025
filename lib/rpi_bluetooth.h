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
#include <wiringSerial.h>
#include "../lib/constants.h"

// Function Prototypes
int bt_init(int, const char*);          // Initialize Bluetooth as server (1) or client (0)
int bt_send(int, const char*);          // Send a message via Bluetooth
int bt_receive(int, char*, int);        // Receive a message via Bluetooth

#endif
