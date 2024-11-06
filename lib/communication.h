// communication.h
// Created on: 2024-11-04
// Author: Sebastien Cabana

#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//#include <wiringPi.h>
//#include <wiringSerial.h>
#include <time.h>
#include "../lib/station.h"

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 115200
#define BUFFER_SIZE 16
#define TIMEOUT 5       // seconds

extern int fd;
extern char* sent[BUFFER_SIZE];
extern char* received[BUFFER_SIZE];

int Start_Comm();
int Send(char*);
int Receive(char*);
void Empty_String(char*);
void Delay(int);
int Try_Connect();

#endif