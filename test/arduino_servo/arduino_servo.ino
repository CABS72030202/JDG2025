// arduino_servo.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

// Includes
#include <stdlib.h>
#include <string.h>
#include <Servo.h>

// Define pins
#define GRIP_PIN 9
#define HEIGHT_PIN 10

// Global constants
#define ANGLE_STEP 2
#define OPEN_BOUND 0
#define CLOSE_BOUND 180
#define UP_BOUND 200
#define DOWN_BOUND 0

// Global variables
int grip_angle = 5;      // Starting angle
int height_angle = 20;
Servo grip;
Servo height;

// Prototypes
void Open();
void Close();
void Up();
void Down();

void setup() {
  grip.attach(GRIP_PIN);
  grip.write(grip_angle);
  height.attach(HEIGHT_PIN);
  height.write(height_angle);
}

void loop() {
  for(int i = 0; i < 10; i++) {
    Close();
    Delay(500);
  }
  for(int i = 0; i < 10; i++) {
    Open();
    Delay(500);
  }
}

void Open() {
  if((grip_angle - ANGLE_STEP) < OPEN_BOUND)
    return;
  grip_angle -= ANGLE_STEP;
  grip.write(grip_angle);
}

void Close() {
  if((grip_angle + ANGLE_STEP) > CLOSE_BOUND)
    return;
  grip_angle += ANGLE_STEP;
  grip.write(grip_angle);
}

void Up() {
}

void Close() {
}
