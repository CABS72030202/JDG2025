// arduino_motors.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

/* 
 * 
 */

// Includes
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define pins
#define DC_FL_PIN 9             // Front left DC motor drive
#define DC_FR_PIN 3             // Front right DC motor drive
#define DC_BL_PIN 10            // Back left DC motor drive
#define DC_BR_PIN 11            // Back right DC motor drive

// Global constants
#define BASE_SPEED 80           // Base speed (tested with 7V)
#define MAX_SPEED 3             // Max speed

// Global variables
int l_speed = 0;                // Analog left wheel speed value
int r_speed = 0;                // Analog right wheel speed value
int speed = 0;                  // Speed factor

// Prototypes
void Forward(int);
void Backward(int);
void Right(int);
void Left(int);
void Stop();

void setup() {
  pinMode(DC_FL_PIN, OUTPUT);
  pinMode(DC_FR_PIN, OUTPUT);
  pinMode(DC_BL_PIN, OUTPUT);
  pinMode(DC_BR_PIN, OUTPUT);
}

void loop() {
  // Increment speed factor
  speed+=1;

  Forward(speed);
  delay(3000);

  Backward(speed);
  delay(3000);

  Right(speed);
  delay(3000);

  Left(speed);
  delay(3000);

  Stop();
  delay(5000);

  // Reset speed after reaching max speed
  if(speed == MAX_SPEED)
    speed = 0;
}

void Forward(int speed) {
  l_speed = speed;
  r_speed = speed;
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_FL_PIN, l_speed * BASE_SPEED);
  analogWrite(DC_FR_PIN, r_speed * BASE_SPEED);
}

void Backward(int speed) {
  l_speed = speed;
  r_speed = speed;
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
  analogWrite(DC_BL_PIN, l_speed * BASE_SPEED);
  analogWrite(DC_BR_PIN, r_speed * BASE_SPEED);
}

void Right(int speed) {
  l_speed = speed;
  r_speed = speed;
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
  analogWrite(DC_FL_PIN, l_speed * BASE_SPEED);
  analogWrite(DC_BR_PIN, r_speed * BASE_SPEED);
}

void Left(int speed) {
  l_speed = speed;
  r_speed = speed;
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_BL_PIN, l_speed * BASE_SPEED);
  analogWrite(DC_FR_PIN, r_speed * BASE_SPEED);
}

void Stop() {
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
}