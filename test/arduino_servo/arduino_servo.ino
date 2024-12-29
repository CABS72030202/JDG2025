// arduino_servo.ino
// Created on: 2024-12-14
// Author: Sebastien Cabana

// Includes
#include <stdlib.h>
#include <string.h>
#include <Servo.h>

// Pin Definitions
#define ARM_PIN        9        // Pin for arm servo motor control
#define CLAW_PIN       10       // Pin for claw servo motor control

// Debug Constants
#define LOOP_CNT       50
#define DELAY          50       // In milliseconds

// Gripper Specific Global Constants
#define CONE_ROBOT_ID  5        // ID corresponding to the robot equipped with a gripper

// Servo Angles
#define ANGLE_STEP     5        // Increment for angle adjustments
#define OPEN_BOUND     180      // Maximum open angle for the claw
#define CLOSE_BOUND    10       // Minimum closed angle for the claw
#define UP_BOUND       180      // Arm angle for position above the rear wheel
#define DOWN_BOUND     170        // Arm angle for position just above the ground

// Global Variables
String gripper_message = "[0:0]\r\n";  // Incoming command message for gripper control
int claw_angle         = CLOSE_BOUND;   // Initial claw angle: fully closed
int arm_angle          = DOWN_BOUND;           // Initial arm angle: parallel to the ground
Servo grip_claw;
Servo grip_arm;

// Prototypes
void Gripper_Control();

void setup() {
  grip_claw.attach(CLAW_PIN);
  grip_claw.write(claw_angle);
  grip_arm.attach(ARM_PIN);
  grip_arm.write(arm_angle);
}

void loop() {
  for(int i = 0; i < LOOP_CNT; i++) {
    gripper_message = "[+:0]\r\n";
    Gripper_Control();
    delay(DELAY);
  }

  for(int i = 0; i < LOOP_CNT; i++) {
    gripper_message = "[-:0]\r\n";
    Gripper_Control();
    delay(DELAY);
  }

  delay(5000);

  /*for(int i = 0; i < LOOP_CNT; i++) {
    gripper_message = "[0:+]\r\n";
    Gripper_Control();
    delay(DELAY);
  }

  for(int i = 0; i < LOOP_CNT; i++) {
    gripper_message = "[0:-]\r\n";
    Gripper_Control();
    delay(DELAY);
  }*/
}

void Gripper_Control() {

  /* Arm control
   *   - '+' to lift (increment the angle)
   *   - '-' to drop (decrement the angle)
   */ 
  switch(gripper_message[1]) {
    case '+': if(arm_angle + ANGLE_STEP <= UP_BOUND) arm_angle += ANGLE_STEP; break;
    case '-': if(arm_angle - ANGLE_STEP >= DOWN_BOUND) arm_angle -= ANGLE_STEP; break;
    case '0': break;
    default:  return;
  }
  grip_arm.write(arm_angle); 

  /* Claw control
   *   - '+' to close (decrement the angle)
   *   - '-' to open (increment the angle)
   */ 
  switch(gripper_message[3]) {
    case '-': if(claw_angle - ANGLE_STEP >= CLOSE_BOUND) claw_angle -= ANGLE_STEP; break;
    case '+': if(claw_angle + ANGLE_STEP <= OPEN_BOUND) claw_angle += ANGLE_STEP; break;
    case '0': break;
    default:  return;
  }
  grip_claw.write(claw_angle); 
}