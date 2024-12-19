// rpi_bluetooth.h
// Created on: 2024-12-18
// Author: Sebastien Cabana

#ifndef RPI_BLUETOOTH_H
#define RPI_BLUETOOTH_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global constants
#define CHANNEL 1           // RFCOMM Channel
#define BUFFER_SIZE 32
#define SERVER_ADR "B8:27:EB:A9:8C:AB"

// Prototypes
int bt_init(int server, const char *dest);
int bt_send(int sock, const char *message);
int bt_receive(int sock, char *buffer, int buffer_size);

#endif