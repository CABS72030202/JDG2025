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
#include "gripper.h"

/* INCLUDE CORRESPONDING HEADER FILE */
#include "blue.h"

// Global Variables
Servo arm;                            // Servo motor object for arm control
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD object for display
String message = "0:00:00:0\r\n";     // Incoming command message format for robot control
int robot = 0;                        // Current robot ID
int l_speed = 0;                      // Left wheel speed
int r_speed = 0;                      // Right wheel speed
int arm_state = 0;                    // Arm state: 0 = down, 1 = up

// Prototypes
void Get_Message();                   // Receive and construct the command message via UART
void Decode_Message();                // Decode the command message and extract robot control values
void Move();                          // Control the movement of the robot's wheels
void Arm();                           // Control the robotic arm
void Print_LCD();                     // Display the robot's status on the LCD
void LED_Control();                   // Control the LEDs based on robot and arm status

void setup() {
  // DC motors
  pinMode(DC_FL_PIN, OUTPUT);
  pinMode(DC_FR_PIN, OUTPUT);
  pinMode(DC_BL_PIN, OUTPUT);
  pinMode(DC_BR_PIN, OUTPUT);

  // LEDs
  pinMode(ARM_LED_PIN, OUTPUT);
  pinMode(ROBOT_LED_PIN, OUTPUT);

  // Servo motors
  if(ROBOT_ID != CONE_ROBOT_ID) {
    arm.attach(SERVO_PIN);
    arm.write(ARM_ANGLE_UP);
  }
  else {
    grip_arm.attach(ARM_PIN);
    grip_arm.write(arm_angle);
  }
    
  // UART
  Serial.begin(115200);

  // LCD screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void loop() {

  // Receive message over UART
  Get_Message();

  // Convert message to command values
  Decode_Message();

  // Test for gripper format and convert message to command values
  if(gripper_message != "[0:0]\r\n")
    Decode_Gripper();

  // Test if gripper claw needs to be initialized
  if(!init_claw && arm_angle <= INIT_ANGLE) 
    Init_Claw();

  // Manage LEDs from command values
  LED_Control();

  // Print command values on LCD screen
  Print_LCD();

  // Execute commands 
  Move();
  Arm();
}

void Init_Claw() {
  grip_claw.attach(CLAW_PIN);
  grip_claw.write(claw_angle);
  init_claw = true;
  delay(300);
}

void Get_Message() {
  message = "";
  while (Serial.available() > 0) {
    char received_char = Serial.read();
    message += received_char;
    if (received_char == '\n') break;
  }
}

void Decode_Message() {

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

  // Test for gripper message format
  if(message[0] == '[') {
    gripper_message = message;
    message = "";
    return;
  }

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

  message = "";
}

void Decode_Gripper() {

/*
 * Message format: a 5-character string structured as follows:
 * [<arm>:<claw>]
 *
 * Possible values for <arm> and <claw> are as follows :
 *   - '+' to increment the angle
 *   - '-' to decrement the angle
 *   - '0' to keep current angle
 */

  // Exit if this robot is not the cone robot
  if (ROBOT_ID != CONE_ROBOT_ID) return;

  // Execute command
  Gripper_Control();
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
  if(!init_claw)
    return;
  switch(gripper_message[3]) {
    case '+': if(claw_angle - ANGLE_STEP >= CLOSE_BOUND) claw_angle -= ANGLE_STEP; break;
    case '-': if(claw_angle + ANGLE_STEP <= OPEN_BOUND) claw_angle += ANGLE_STEP; break;
    case '0': break;
    default:  return;
  }
  grip_claw.write(claw_angle); 
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
  arm.write(arm_state ? ARM_ANGLE_UP : ARM_ANGLE_DOWN);
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