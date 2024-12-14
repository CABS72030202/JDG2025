// mini_robots.ino
// Created on: 2024-11-08
// Author: Emeric Desmarais and Sebastien Cabana

/* 
 * This code implements all necessary functions for the movement of mini-robots in the Machine 
 * competition for the JDG 2025 for the Trois-Rivières delegation. The general operation of 
 * these robots involves receiving a command via UART from the main microcontroller, allowing 
 * simultaneous communication with all robots. This command is then processed to determine which 
 * robot should move and according to what parameters. Control of the robotic arm, which enables 
 * the robot to connect to stations, also follows this command structure.
 */

// Includes
#include <stdlib.h>
#include <string.h>
#include <Servo.h>
#include <math.h>

// Define pins
#define DC_FL_PIN 9             // Front left DC motor drive
#define DC_FR_PIN 3             // Front right DC motor drive
#define DC_BL_PIN 10            // Back left DC motor drive
#define DC_BR_PIN 11            // Back right DC motor drive
#define SERVO_PIN 5

// Global constants
#define ROBOT_ID 0              // Unique ID to each Arduino : RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
#define BASE_SPEED 80           // Base speed 
#define ARM_ANGLE_UP 45
#define ARM_ANGLE_DOWN 0

// Global variables
String message = "0:00:00:0\n"; // Format is <Robot>:<Left wheel speed>:<Right wheel speed>:<Arm control>
int robot = 0;                  // RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
int l_speed = 0;                // Analog left wheel speed value
int r_speed = 0;                // Analog right wheel speed value
int arm_state = 0;              // 0 is DOWN, 1 is UP
Servo myServo;

// Prototypes
void Get_Message();
void Decode_UART();
void Move();
void Arm();

void setup() {
  // Set pins
  pinMode(DC_FL_PIN, OUTPUT);
  pinMode(DC_FR_PIN, OUTPUT);
  pinMode(DC_BL_PIN, OUTPUT);
  pinMode(DC_BR_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);

  // Set Timer 1 (16-bit, controls D9 and D10) to ~2 kHz
  //TCCR1B = TCCR1B & B11111000 | B00000010;  // Set prescaler to 8
  
  // Set Timer 2 (8-bit, controls D3 and D11) to ~2 kHz
  //TCCR2B = TCCR2B & B11111000 | B00000010;  // Set prescaler to 8

  // Set serial communication
  Serial.begin(115200);
}

void loop() {
  Get_Message();
  Decode_UART();
  Move();
  Arm();
}

void Get_Message() {
  message = "";
  while (Serial.available() > 0) {
    char received_char = Serial.read();
    message += received_char;
    if(received_char == '\r')
      break;
  }
}

void Decode_UART() {
/*
 * Message format: a 9-character string structured as follows:
 * <robot>:<left wheel speed>:<right wheel speed>:<arm control>
 *
 * - The first character represents the robot's color, indicated by the first letter:
 *   'R' (RED), 'G' (GREEN), 'B' (BLUE), 'Y' (YELLOW), 'P' (PURPLE), 'C' (CONE), 'S' (SHIP).
 *
 * - The next two characters specify the left wheel speed:
 *   - The first character is the direction: '+' for forward, '-' for reverse.
 *   - The second character is the base speed multiplier: '0' stops the wheel, '1' for low speed, etc.
 *
 * - The next two characters specify the right wheel speed, using the same format as the left:
 *   direction ('+' or '-') and speed multiplier.
 *
 * - The last character controls the robot arm movement:
 *   'U' to raise the arm and 'D' to lower it.
 */

 // Return if invalid format
 if(message[10] != '\n' || message == "")
  return;

  // <robot>
  switch (message[0]) {
    case '0': robot = -1; break;
    case 'R': robot = 0; break;
    case 'G': robot = 1; break;
    case 'B': robot = 2; break;
    case 'Y': robot = 3; break;
    case 'P': robot = 4; break;
    case 'C': robot = 5; break;
    case 'S': robot = 6; break;
    default: return;  // Invalid robot, return early
  }

  // Exit if adressing other robot
  if(robot != ROBOT_ID)
    return;

  // <left wheel speed>
  l_speed = message[3] - '0';
  if (message[2] == '-') l_speed *= -1;
  else l_speed = abs(l_speed);

  // <right wheel speed>
  r_speed = message[6] - '0';
  if (message[5] == '-') r_speed *= -1;
  else r_speed = abs(r_speed);

  // <arm control>
  switch (message[8]) {
    case '0': arm_state = -1; break;
    case 'U': arm_state = 1; break;
    case 'D': arm_state = 0; break;
    default: return;  // Invalid arm control
  }

  // Reset message
  message = "";
}

void Move() {
  // Write LOW on both pins if null speed
  if(l_speed == 0) {
    analogWrite(DC_FL_PIN, 0);
    analogWrite(DC_BL_PIN, 0);
  }

  if(r_speed == 0) {
    analogWrite(DC_FR_PIN, 0);
    analogWrite(DC_BR_PIN, 0);
  }

  // Write on back pins if negative speed
  if(l_speed < 0)
    analogWrite(DC_BL_PIN, l_speed * BASE_SPEED);

  if(r_speed < 0)
    analogWrite(DC_BR_PIN, r_speed * BASE_SPEED);

  // Write on front pins if positive speed
  if(l_speed > 0) 
    analogWrite(DC_FL_PIN, l_speed * BASE_SPEED);

  if(r_speed > 0)
    analogWrite(DC_FR_PIN, r_speed * BASE_SPEED);
}

void Arm() {
  if(arm_state)
    myServo.write(ARM_ANGLE_UP);
  else
    myServo.write(ARM_ANGLE_DOWN);
}
