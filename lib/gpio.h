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
#include "../lib/constants.h"

// Function Prototypes
void Initialize_GPIO();
void Write_Arm_BIN(int, int, int, int);
void Write_Arm_DEC(int);
int Read_Arm_BIN();
int Read_Arm_DEC(int, int, int, int);

#endif
