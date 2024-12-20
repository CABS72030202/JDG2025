/*
 * gpio.h
 * Created on: 2024-11-25
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file defines the interface for GPIO operations to enable 
 * communication through binary codes between executables running in parallel 
 * on the same platform. It includes pin definitions, state IDs, and function 
 * prototypes for GPIO interactions.
 */

#ifndef GPIO_H
#define GPIO_H

// Includes
#include <wiringPi.h>
#include <stdio.h>

// GPIO Pins
#define ARM_OUT_PIN_0   0    // GPIO 0 - PIN 11
#define ARM_OUT_PIN_1   3    // GPIO 3 - PIN 15
#define ARM_OUT_PIN_2   5    // GPIO 5 - PIN 18
#define ARM_OUT_PIN_3   6    // GPIO 6 - PIN 22
#define ARM_IN_PIN_0    2    // GPIO 2 - PIN 13
#define ARM_IN_PIN_1    4    // GPIO 4 - PIN 16
#define ARM_IN_PIN_2    25   // GPIO 25 - PIN 37
#define ARM_IN_PIN_3    27   // GPIO 27 - PIN 36

// State IDs
#define DOWN_ALL        15
#define RED_UP          0
#define RED_DOWN        1
#define GREEN_UP        2
#define GREEN_DOWN      3
#define BLUE_UP         4
#define BLUE_DOWN       5
#define YELLOW_UP       6
#define YELLOW_DOWN     7
#define PURPLE_UP       8
#define PURPLE_DOWN     9

// Function Prototypes
void Initialize_GPIO();
void Write_Arm_BIN(int, int, int, int);
void Write_Arm_DEC(int);
int Read_Arm_BIN();
int Read_Arm_DEC(int, int, int, int);

#endif
