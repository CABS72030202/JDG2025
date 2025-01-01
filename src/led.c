// led.c
// Created on: 2025-01-01
// Author: Sebastien Cabana

#include "../lib/led.h"

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

int LED_Init() {
    if (ws2811_init(&ledstring)) {
        fprintf(stderr, "WS2811 initialization failed!\n");
        return 1;
    }
    for(int i = 0; i < LED_COUNT; i++)
        Set_LED_Color(i, 255, 0, 0);
    return 0;
}

void Set_LED_Color(int led_num, int red, int green, int blue) {
    ledstring.channel[0].leds[led_num] = (green << 16) | (red << 8) | blue;
    ws2811_render(&ledstring);
}

void Clear_LEDs() {
    for (int i = 0; i < LED_COUNT; i++) {
        Set_LED_Color(i, 0, 0, 0);
    }
    ws2811_render(&ledstring);
}