/*
 * mini_robots.ino
 * Created on: 2024-11-08
 * Author: Sebastien Cabana
 *
 * Description:
 * This Arduino program controls the movement and actions of mini-robots for the Machine 
 * competition at JDG 2025. Robots communicate via UART to receive commands, interpret them, 
 * and perform corresponding actions. Functions manage movement, robotic arm control, and 
 * status display on an LCD screen.
 *
 * Hardware:
 * - LEDs with 220-ohm resistors
 * - Two 3.7V batteries in series
 * 
 * Usage:
 * Include the appropriate configuration header file for each robot before uploading 
 * the code. This ensures compatibility with the robot's hardware setup.
 */

// Includes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <string.h>
#include <Servo.h>
#include <math.h>

/* INCLUDE CORRESPONDING HEADER FILE */
#include "blue.h"

// Global Variables
String message = "0:00:00:0\r\n"; // Incoming command message format
int robot = 0;                  // Current robot ID
int l_speed = 0;                // Left wheel speed
int r_speed = 0;                // Right wheel speed
int arm_state = 0;              // Arm state: 0 = down, 1 = up
Servo myServo;                  // Servo motor object for arm control
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD object for display

// Prototypes
void Get_Message();             // Receive and construct the command message via UART
void Decode_UART();             // Decode the command message and extract control values
void Move();                    // Control the movement of the robot's wheels
void Arm();                     // Control the robotic arm
void Print_LCD();               // Display the robot's status on the LCD
void LED_Control();             // Control the LEDs based on robot and arm status

void setup() {
  pinMode(DC_FL_PIN, OUTPUT);
  pinMode(DC_FR_PIN, OUTPUT);
  pinMode(DC_BL_PIN, OUTPUT);
  pinMode(DC_BR_PIN, OUTPUT);
  pinMode(ARM_LED_PIN, OUTPUT);
  pinMode(ROBOT_LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void loop() {
  Get_Message();
  Decode_UART();
  LED_Control();
  Print_LCD();
  Move();
  Arm();
}

void Get_Message() {
  message = "";
  while (Serial.available() > 0) {
    char received_char = Serial.read();
    message += received_char;
    if (received_char == '\n') break;
  }
}

void Decode_UART() {
  // Validate message length
  if (message.length() != 10 || message[9] != '\n') return;

  // Decode robot ID
  switch (message[0]) {
    case '0': robot = -1; break;
    case 'R': robot = 0; break;
    case 'G': robot = 1; break;
    case 'B': robot = 2; break;
    case 'Y': robot = 3; break;
    case 'P': robot = 4; break;
    case 'C': robot = 5; break;
    case 'S': robot = 6; break;
    default: return;
  }

  // Exit if message is not for this robot
  if (robot != ROBOT_ID) return;

  // Decode left wheel speed
  l_speed = message[3] - '0';
  if (message[2] == '-') l_speed *= -1;

  // Decode right wheel speed
  r_speed = message[6] - '0';
  if (message[5] == '-') r_speed *= -1;

  // Decode arm state
  arm_state = (message[8] == 'U') ? 1 : (message[8] == 'D') ? 0 : -1;

  Serial.print(message);
  message = "";
}

void Move() {
  // Stop wheels if speed is zero
  if (l_speed == 0) {
    analogWrite(DC_FL_PIN, 0);
    analogWrite(DC_BL_PIN, 0);
  }
  if (r_speed == 0) {
    analogWrite(DC_FR_PIN, 0);
    analogWrite(DC_BR_PIN, 0);
  }

  // Control back wheels for negative speed
  if (l_speed < 0) analogWrite(DC_BL_PIN, LEFT_SPEEDS[abs(l_speed) - 1]);
  if (r_speed < 0) analogWrite(DC_BR_PIN, RIGHT_SPEEDS[abs(r_speed) - 1]);

  // Control front wheels for positive speed
  if (l_speed > 0) analogWrite(DC_FL_PIN, LEFT_SPEEDS[l_speed - 1]);
  if (r_speed > 0) analogWrite(DC_FR_PIN, RIGHT_SPEEDS[r_speed - 1]);
}

void Arm() {
  myServo.write(arm_state ? ARM_ANGLE_UP : ARM_ANGLE_DOWN);
}

void Print_LCD() {
  lcd.setCursor(0, 0);
  lcd.print("ID: ");
  lcd.print(robot);
  lcd.setCursor(8, 0);
  lcd.print("ARM: ");
  lcd.print(arm_state);
  lcd.setCursor(0, 1);
  lcd.print("L: ");
  lcd.print(l_speed);
  lcd.setCursor(8, 1);
  lcd.print("R: ");
  lcd.print(r_speed);
}

void LED_Control() {
  digitalWrite(ROBOT_LED_PIN, (robot == ROBOT_ID) ? HIGH : LOW);
  digitalWrite(ARM_LED_PIN, arm_state ? HIGH : LOW);
}