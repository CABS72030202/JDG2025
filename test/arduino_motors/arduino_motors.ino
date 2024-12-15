// arduino_motors.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

/* 
 * Code to control two DC motors with three speed levels.
 * Assumes external motor driver handles direction.
 */

// Define pins
#define DC_FL_PIN 5             // Front left DC motor drive
#define DC_FR_PIN 3             // Front right DC motor drive
#define DC_BL_PIN 6             // Back left DC motor drive
#define DC_BR_PIN 11            // Back right DC motor drive

// Global constants
#define MAX_SPEED 3             
const int LEFT_SPEEDS[MAX_SPEED] = {180, 210, 240};
const int RIGHT_SPEEDS[MAX_SPEED] = {120, 150, 180};

// Global variables
int l_speed = 0;                // Analog left wheel speed value
int r_speed = 0;                // Analog right wheel speed value
int speed = 0;                  // Speed factor

// Function prototypes
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
  // Cycle through speeds
  Forward(speed);
  delay(2000);

  Backward(speed);
  delay(2000);

  Right(speed);
  delay(2000);

  Left(speed);
  delay(2000);

  Stop();
  delay(3000);

  // Increment speed factor and reset if it exceeds MAX_SPEED
  speed = (speed + 1) % MAX_SPEED;
}

void Forward(int speed) {
  l_speed = LEFT_SPEEDS[speed];
  r_speed = RIGHT_SPEEDS[speed];
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_FL_PIN, l_speed);
  analogWrite(DC_FR_PIN, r_speed);
}

void Backward(int speed) {
  l_speed = LEFT_SPEEDS[speed];
  r_speed = RIGHT_SPEEDS[speed];
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
  analogWrite(DC_BL_PIN, l_speed);
  analogWrite(DC_BR_PIN, r_speed);
}

void Right(int speed) {
  l_speed = LEFT_SPEEDS[speed];
  r_speed = RIGHT_SPEEDS[speed];
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
  analogWrite(DC_FL_PIN, l_speed);
  analogWrite(DC_BR_PIN, r_speed);
}

void Left(int speed) {
  l_speed = LEFT_SPEEDS[speed];
  r_speed = RIGHT_SPEEDS[speed];
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_BL_PIN, l_speed);
  analogWrite(DC_FR_PIN, r_speed);
}

void Stop() {
  analogWrite(DC_BL_PIN, 0);
  analogWrite(DC_BR_PIN, 0);
  analogWrite(DC_FL_PIN, 0);
  analogWrite(DC_FR_PIN, 0);
}
