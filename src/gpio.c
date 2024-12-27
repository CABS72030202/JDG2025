// gpio.c
// Created on: 2024-11-25
// Author: Sebastien Cabana

#include "../lib/gpio.h"

void Initialize_GPIO() {
    pinMode(ARM_OUT_PIN_0, OUTPUT);
    pinMode(ARM_OUT_PIN_1, OUTPUT);
    pinMode(ARM_OUT_PIN_2, OUTPUT);
    pinMode(ARM_OUT_PIN_3, OUTPUT);
    pinMode(ARM_IN_PIN_0, INPUT);
    pinMode(ARM_IN_PIN_1, INPUT);
    pinMode(ARM_IN_PIN_2, INPUT);
    pinMode(ARM_IN_PIN_3, INPUT);
    Write_Arm_DEC(DOWN_ALL);
}

void Write_Arm_BIN(int bit3, int bit2, int bit1, int bit0) {
    digitalWrite(ARM_OUT_PIN_3, bit3);
    digitalWrite(ARM_OUT_PIN_2, bit2);
    digitalWrite(ARM_OUT_PIN_1, bit1);
    digitalWrite(ARM_OUT_PIN_0, bit0);
}

void Write_Arm_DEC(int num) {
    int bit3 = (num >> 3) & 1;
    int bit2 = (num >> 2) & 1;
    int bit1 = (num >> 1) & 1;
    int bit0 = num & 1;
    Write_Arm_BIN(bit3, bit2, bit1, bit0);
}

int Read_Arm_BIN() {
    int bit3 = digitalRead(ARM_OUT_PIN_3);
    int bit2 = digitalRead(ARM_OUT_PIN_2);
    int bit1 = digitalRead(ARM_OUT_PIN_1);
    int bit0 = digitalRead(ARM_OUT_PIN_0);
    return Read_Arm_DEC(bit3, bit2, bit1, bit0);
}

int Read_Arm_DEC(int bit3, int bit2, int bit1, int bit0) {
    return ((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0);
}