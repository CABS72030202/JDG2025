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
#define DELAY          5       // In milliseconds

// Gripper Specific Global Constants
#define CONE_ROBOT_ID  5        // ID corresponding to the robot equipped with a gripper

// Servo Angles
#define ANGLE_STEP     1        // Increment for angle adjustments
#define OPEN_BOUND     70       // Maximum open angle for the claw
#define CLOSE_BOUND    0        // Minimum closed angle for the claw
#define UP_BOUND       30       // Arm angle for position above the rear wheel
#define DOWN_BOUND     180      // Arm angle for position just above the ground
#define INIT_ANGLE     80       // Arm angle to reach before attaching claw servo

// Global Variables
String gripper_message = "[0:0]\r\n";       // Incoming command message for gripper control
int claw_angle         = CLOSE_BOUND;       // Initial claw angle: fully closed
int arm_angle          = DOWN_BOUND - 10;   // Initial arm angle: parallel to the ground
bool init_claw         = false;             // Attach claw servo only when arm is raised after setup
Servo grip_claw;
Servo grip_arm;

// Prototypes
void Gripper_Control();
void Init_Claw();

void setup() {
  grip_arm.attach(ARM_PIN);
  grip_arm.write(arm_angle);
}

void loop() {
  // Loop before claw servo attach
  if(!init_claw) {
    while(arm_angle != INIT_ANGLE) {
      gripper_message = "[+:0]\r\n";
      Gripper_Control();
      delay(DELAY*3);
    }
    Init_Claw();
    delay(1000);
  }

  // Normal loop
  else {
    while(arm_angle != UP_BOUND) {
      gripper_message = "[+:0]\r\n";
      Gripper_Control();
      delay(DELAY);
    }
    delay(1000);

    while(arm_angle != DOWN_BOUND) {
      gripper_message = "[-:0]\r\n";
      Gripper_Control();
      delay(DELAY);
    }
    delay(1000);

    while(claw_angle != OPEN_BOUND) {
      gripper_message = "[0:-]\r\n";
      Gripper_Control();
      delay(DELAY);
    }
    delay(1000);

    while(claw_angle != CLOSE_BOUND) {
      gripper_message = "[0:+]\r\n";
      Gripper_Control();
      delay(DELAY);
    }
    delay(1000);
  }
}

void Gripper_Control() {

  /* Arm control
   *   - '+' to lift (decrement the angle)
   *   - '-' to drop (increment the angle)
   */ 
  switch(gripper_message[1]) {
    case '+': if(arm_angle - ANGLE_STEP >= UP_BOUND) arm_angle -= ANGLE_STEP; break;
    case '-': if(arm_angle + ANGLE_STEP <= DOWN_BOUND) arm_angle += ANGLE_STEP; break;
    case '0': break;
    default:  return;
  }
  grip_arm.write(arm_angle); 

  /* Claw control
   *   - '+' to close (decrement the angle)
   *   - '-' to open (increment the angle)
   */ 
  switch(gripper_message[3]) {
    case '+': if(claw_angle - ANGLE_STEP >= CLOSE_BOUND) claw_angle -= ANGLE_STEP; break;
    case '-': if(claw_angle + ANGLE_STEP <= OPEN_BOUND) claw_angle += ANGLE_STEP; break;
    case '0': break;
    default:  return;
  }
  grip_claw.write(claw_angle); 
}

void Init_Claw() {
  grip_claw.attach(CLAW_PIN);
  grip_claw.write(claw_angle);
  init_claw = true;
}