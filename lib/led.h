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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include "../rpi_ws281x/ws2811.h"

// Global Constants
#define LED_COUNT       5        // Number of LEDs
#define GPIO_PIN        21       // GPIO pin number (BCM mode)
#define LED_FREQ_HZ     800000   // Frequency of WS2811 LEDs
#define LED_DMA         10       // DMA channel
#define LED_BRIGHTNESS  255      // Maximum brightness
#define LED_CHANNEL     0        // Channel for controlling LEDs

// Color IDs
#define RED_ID      0
#define GREEN_ID    1
#define BLUE_ID     2
#define YELLOW_ID   3
#define PURPLE_ID   4
#define WHITE_ID    5
#define ORANGE_ID   6
#define PINK_ID     7

// Global variables
extern ws2811_t ledstring;

// Function Prototypes
int LED_Init();                             // Initialize the LEDs
void Set_LED_RGB(int, int, int, int);       // Set the color of a specific LED with RGB format
void Set_LED_Color(int, int);               // Set the color of a specific LED from color ID
void Clear_LEDs();                          // Turn off all LEDs

#endif