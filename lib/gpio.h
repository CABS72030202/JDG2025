// gpio.h
// Created on: 2024-11-25
// Author: Sebastien Cabana

#ifndef GPIO_H
#define GPIO_H
#include <wiringPi.h>
#include <stdio.h>

// GPIO pins
#define ARM_OUT_PIN_0 22        // GPIO PIN 31
#define ARM_OUT_PIN_1 23        // GPIO PIN 33
#define ARM_OUT_PIN_2 24        // GPIO PIN 35
#define ARM_OUT_PIN_3 25        // GPIO PIN 37
#define ARM_IN_PIN_0 26         // GPIO PIN 32
#define ARM_IN_PIN_1 27         // GPIO PIN 36
#define ARM_IN_PIN_2 28         // GPIO PIN 38
#define ARM_IN_PIN_3 29         // GPIO PIN 40

// State IDs
#define DOWN_ALL 15
#define RED_UP 0
#define RED_DOWN 1
#define GREEN_UP 2
#define GREEN_DOWN 3
#define BLUE_UP 4
#define BLUE_DOWN 5
#define YELLOW_UP 6
#define YELLOW_DOWN 7
#define PURPLE_UP 8
#define PURPLE_DOWN 9

// Global variables and constants

// Prototypes
void Initialize_GPIO();
void Write_Arm_BIN(int, int, int, int);
void Write_Arm_DEC(int);
int Read_Arm_BIN();
int Read_Arm_DEC(int, int, int, int);

#endif