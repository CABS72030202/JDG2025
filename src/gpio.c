// gpio.c
// Created on: 2024-11-25
// Author: Sebastien Cabana

#include "../lib/gpio.h"

void Initialize_GPIO() {
    pinMode(ARM_PIN_0, OUTPUT);
    pinMode(ARM_PIN_1, OUTPUT);
    pinMode(ARM_PIN_2, OUTPUT);
    pinMode(ARM_PIN_3, OUTPUT);
}

void Control_Pins(int bit3, int bit2, int bit1, int bit0) {
    digitalWrite(ARM_PIN_3, bit3);
    digitalWrite(ARM_PIN_2, bit2);
    digitalWrite(ARM_PIN_1, bit1);
    digitalWrite(ARM_PIN_0, bit0);
}

void Control_From_DEC(int num) {
    int bit3 = (num >> 3) & 1;
    int bit2 = (num >> 2) & 1;
    int bit1 = (num >> 1) & 1;
    int bit0 = num & 1;
    Control_Pins(bit3, bit2, bit1, bit0);
}