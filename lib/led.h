/*
 * led.h
 * Created on: 2025-01-01
 * Author: Sebastien Cabana
 *
 * Description:
 * This program is designed to control 5 WS2811 LEDs connected to GPIO #21 on a Raspberry Pi 3B. 
 * The LEDs are implemented on a boat to display error codes and station robot statuses, providing clear visual feedback to the pilot.
 * 
 * The program utilizes the rpi-ws281x library for precise LED timing and supports the GRB color format.
 * 
 * Requirements:
 * 1. Install the rpi-ws281x library in the HOME folder: 
 *    - git clone https://github.com/jgarff/rpi_ws281x.git
 *    - cd rpi_ws281x
 *    - mkdir build
 *    - cd build
 *    - cmake -D BUILD_SHARED=ON -D BUILD_TEST=OFF ..
 *    - sudo make install
 * 
 * 2. WiringPi library must be installed for GPIO access.
 * 
 * 3. GPIO #21 is used to control the LEDs.
 */

#ifndef LED_H
#define LED_H

// Includes
#include "../lib/constants.h"
#include "../rpi_ws281x/ws2811.h"

// Global variables
extern ws2811_t ledstring;

// Function Prototypes
int LED_Init();                             // Initialize the LEDs
void Set_LED_Color(int, int, int, int);     // Set the color of a specific LED with RGB format
void Clear_LEDs();                          // Turn off all LEDs

#endif