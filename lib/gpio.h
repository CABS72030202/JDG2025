// gpio.h
// Created on: 2024-11-25
// Author: Sebastien Cabana

#ifndef GPIO_H
#define GPIO_H
#include <wiringPi.h>
#include <stdio.h>

// GPIO pins
#define ARM_PIN_0 22        // GPIO PIN 31
#define ARM_PIN_1 23        // GPIO PIN 33
#define ARM_PIN_2 24        // GPIO PIN 35
#define ARM_PIN_3 25        // GPIO PIN 37

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
void Control_Pins(int, int, int, int);
void Control_From_DEC(int);

#endif