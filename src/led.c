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
        Set_LED_RGB(i, 255, 0, 0);
    return 0;
}

void Set_LED_RGB(int led_num, int red, int green, int blue) {
    ledstring.channel[0].leds[led_num] = (green << 16) | (red << 8) | blue;
    ws2811_render(&ledstring);
}

void Set_LED_Color(int led_num, int color_id) {
    switch(color_id) {
        case RED_ID:
            Set_LED_RGB(led_num, 255, 0, 0);
            break;
        case GREEN_ID:
            Set_LED_RGB(led_num, 0, 255, 0);
            break;
        case BLUE_ID:
            Set_LED_RGB(led_num, 0, 0, 255);
            break;
        case YELLOW_ID:
            Set_LED_RGB(led_num, 255, 255, 0);
            break;
        case PURPLE_ID:
            Set_LED_RGB(led_num, 127, 0, 255);
            break;
        case WHITE_ID:
            Set_LED_RGB(led_num, 255, 255, 255);
            break;
        case ORANGE_ID:
            Set_LED_RGB(led_num, 255, 127, 0);
            break;
        case PINK_ID:
            Set_LED_RGB(led_num, 255, 0, 127);
            break;
        default:
            printf("Error. Invalid color ID in Set_LED_Color.\n");
            break;
    }
}

void Clear_LEDs() {
    for (int i = 0; i < LED_COUNT; i++) {
        Set_LED_RGB(i, 0, 0, 0);
    }
    ws2811_render(&ledstring);
}