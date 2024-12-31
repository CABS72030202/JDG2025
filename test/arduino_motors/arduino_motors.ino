// arduino_motors.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

/* 
 * Code to control two DC motors with three speed levels.
 * Assumes external motor driver handles direction.
 */

// Define pins
#define DC_FL_PIN 6             // Front left DC motor drive
#define DC_FR_PIN 3            // Front right DC motor drive
#define DC_BL_PIN 5             // Back left DC motor drive
#define DC_BR_PIN 11             // Back right DC motor drive

// Debug constants
#define MIN_TEST_VAL  50
#define MAX_TEST_VAL  300

// Tests values
#define CURRENT_TEST        4   // Select parameter to debug
#define R_MOTOR_SPEED_TEST  1   // Find limit values for right motor
#define L_MOTOR_SPEED_TEST  2   // Find limit values for left motor
#define RATIO_TEST          3   // Find equivalent values for both motors to give the same force
#define STRAIGHT_TEST       4   // Test going forward and backwards with defined speeds

// Global constants
#define MAX_SPEED 3             
const int LEFT_SPEEDS[MAX_SPEED]  = {140, (140+255)/2, 255};
const int RIGHT_SPEEDS[MAX_SPEED] = {100, (100+255)/2, 250};

// Global variables
int l_speed = 0;                // Analog left wheel speed value
int r_speed = 0;                // Analog right wheel speed value
int speed = 0;                  // Speed factor

// Debug prototypes
void Find_Limits(int);          // Find minimum and maximum speed value (0 is right motor, 1 is left motor)

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
  Serial.begin(115200);
}

void loop() {
  switch(CURRENT_TEST) {
    case R_MOTOR_SPEED_TEST:
      Find_Limits(0);
      break;
    case L_MOTOR_SPEED_TEST:
      Find_Limits(1);
      break;
    case RATIO_TEST:
      break;
    case STRAIGHT_TEST:
      Forward(speed);
      delay(3000);
      Backward(speed);
      delay(3000);
      speed = (speed + 1) % MAX_SPEED;
      break;
    default:  Serial.println("ERROR. Invalid test."); delay(60000); break;
  }
}

void Find_Limits(int motor) {
  int front_pin, back_pin, *speeds;
  if(motor == 0) {  // Right
    front_pin = DC_FR_PIN;
    back_pin = DC_BR_PIN;
    speeds = RIGHT_SPEEDS;
  }
  else {            // Left
    front_pin = DC_FL_PIN;
    back_pin = DC_BL_PIN;
    speeds = LEFT_SPEEDS;
  }

  // Test forward
  analogWrite(back_pin, 0);
  for(int i = 0; i < MAX_SPEED; i++) {
    analogWrite(front_pin, speeds[i]);
    Serial.print("Current speed value = ");
    Serial.println(speeds[i]);
    delay(2000);
  }
  for(int i = MIN_TEST_VAL; i <= MAX_TEST_VAL; i++) {
    analogWrite(front_pin, i);
    Serial.print("Current speed value = ");
    Serial.println(i);
    delay(200);
  }

  // Test backward
  analogWrite(front_pin, 0);
  for(int i = 0; i < MAX_SPEED; i++) {
    analogWrite(back_pin, speeds[i]);
    Serial.print("Current speed value = ");
    Serial.println(speeds[i]);
    delay(2000);
  }
  for(int i = MIN_TEST_VAL; i <= MAX_TEST_VAL; i++) {
    analogWrite(back_pin, i);
    Serial.print("Current speed value = ");
    Serial.println(i);
    delay(200);
  }
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
