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
#define OPEN_BOUND 180    // Grip touches backwards
#define CLOSE_BOUND 0     // Grip radius smaller than cone contact point radius
#define UP_BOUND 200      // Arm just above rear wheel
#define DOWN_BOUND 0      // Grip just above ground

// Global variables
int grip_angle = 5;      // Starting angle : completely closed
int height_angle = 20;   // Starting angle : grip parallel to ground
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
  /*for(int i = 0; i < 10; i++) {
    Close();
    Delay(500);
  }
  for(int i = 0; i < 10; i++) {
    Open();
    Delay(500);
  }*/
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
