/*
 * WS2811 LED Control with Raspberry Pi 3B
 * 
 * This program demonstrates how to control 5 WS2811 LEDs connected to GPIO pin #12 (BCM pin 18) on a Raspberry Pi 3B.
 * It uses the rpi-ws281x library to control the LEDs with precise timing. The LEDs are controlled using the GRB color format.
 * The program initializes the LEDs, lights them one by one with a delay of 500ms between each LED, cycling through red, green, and blue colors.
 * 
 * Requirements:
 * 1. Install the rpi-ws281x library: 
 *    - cd ~
 *    - git clone https://github.com/jgarff/rpi_ws281x.git
 *    - cd rpi_ws281x
 *    - sudo scons
 * 
 * 2. WiringPi library must be installed for GPIO access.
 * 
 * 3. GPIO #21 is used to control the LEDs.
 * 
 * GPIO : 21 (Pin #40)
 * LED Count: 5
 * LED Frequency: 800kHz
 * Strip Type: WS2811 (GRB)
 * DMA Channel: 10
 * Maximum Brightness: 255
 * 
 * Compile with: 
 *   gcc -o ws2811 ws2811 -lwiringPi -lws2811
 * Run with:
 *   sudo ./ws2811
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include "../rpi_ws281x/ws2811.h"

#define LED_COUNT      5        // Number of LEDs
#define GPIO_PIN       21       // GPIO pin number (BCM mode)
#define LED_FREQ_HZ    800000   // Frequency of WS2811 LEDs
#define LED_DMA         10      // DMA channel
#define LED_BRIGHTNESS 255     // Maximum brightness
#define LED_CHANNEL    0       // Channel for controlling LEDs

// Create a ws2811_t structure
ws2811_t ledstring = {
    .freq = LED_FREQ_HZ,
    .dmanum = LED_DMA,
    .channel = {
        [0] = {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = LED_BRIGHTNESS,
            .strip_type = WS2811_STRIP_GRB
        }
    }
};

void set_led_color(int led_num, int red, int green, int blue) {
    // Set the color of a specific LED
    ledstring.channel[0].leds[led_num] = (green << 16) | (red << 8) | blue;
}

void clear_leds() {
    // Turn off all LEDs
    for (int i = 0; i < LED_COUNT; i++) {
        set_led_color(i, 0, 0, 0);
    }
    ws2811_render(&ledstring);
}

int main() {
    // Initialize wiringPi
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "WiringPi setup failed!\n");
        return 1;
    }

    // Initialize the LED string
    if (ws2811_init(&ledstring)) {
        fprintf(stderr, "WS2811 initialization failed!\n");
        return 1;
    }

    // Light LEDs one by one in red, green, and blue with 500ms delay
    for (int color = 0; color < 3; color++) {
        for (int i = 0; i < LED_COUNT; i++) {
            clear_leds(); // Turn off all LEDs
            if (color == 0) {
                set_led_color(i, 255, 0, 0); // Red
            } else if (color == 1) {
                set_led_color(i, 0, 255, 0); // Green
            } else {
                set_led_color(i, 0, 0, 255); // Blue
            }
            ws2811_render(&ledstring);
            delay(500); // 500ms delay
        }
    }

    // Cleanup
    clear_leds();
    ws2811_fini(&ledstring);

    return 0;
}
