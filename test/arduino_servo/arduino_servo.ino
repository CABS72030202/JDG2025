// arduino_servo.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

/* 
 * 
 */

// Includes
#include <stdlib.h>
#include <string.h>
#include <Servo.h>

// Define pins
#define SERVO_PIN 9

// Global constants 
#define ARM_ANGLE_UP 20
#define ARM_ANGLE_DOWN 60

// Global variables
int arm_state = 0;              // 0 is DOWN, 1 is UP
Servo myServo;

// Prototypes
void Arm();

void setup() {
  myServo.attach(SERVO_PIN);
}

void loop() {
  Arm();
  delay(2000);
  arm_state = arm_state ^ 1;
}

void Arm() {
  if(arm_state)
    myServo.write(ARM_ANGLE_UP);
  else
    myServo.write(ARM_ANGLE_DOWN);
}