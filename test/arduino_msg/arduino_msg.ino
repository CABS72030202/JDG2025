#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Initialize the LCD, set the I2C address (0x27) and LCD size (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile bool resetLCDFlag = false;
#define RESET_TIMER 10     // in ms

#define ROBOT_ID 0
String message = "0:00:00:0\r\n"; // Format is <Robot>:<Left wheel speed>:<Right wheel speed>:<Arm control>
int robot = 0;                  // RED is 0, GREEN is 1, BLUE is 2, YELLOW is 3, PURPLE is 4, CONE is 5, BOAT is 6
int l_speed = 0;                // Analog left wheel speed value
int r_speed = 0;                // Analog right wheel speed value
int arm_state = 0;              // 0 is DOWN, 1 is UP

void Print_LCD();
void Clear();
void Get_Message();
void Decode_Message();

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(115200);

  // Configure Timer1 for 1kHz interrupts
  noInterrupts();            // Disable all interrupts
  TCCR1A = 0;                // Set entire TCCR1A register to 0
  TCCR1B = 0;                // Same for TCCR1B
  TCNT1 = 0;                 // Initialize counter value to 0
  OCR1A = 15999;             // Set compare match register for 1kHz
                             // (16MHz / (1kHz * 8) - 1)
  TCCR1B |= (1 << WGM12);    // Turn on CTC mode (Clear Timer on Compare)
  TCCR1B |= (1 << CS11);     // Set prescaler to 8
  TIMSK1 |= (1 << OCIE1A);   // Enable timer compare interrupt
  interrupts();              // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  static int count = 0;
  count++;
  if (count >= RESET_TIMER) {
    resetLCDFlag = true;
    count = 0;
  }
}

void loop() {
  Get_Message();
  Decode_Message();
  Print_LCD();
}

void Print_LCD() {
  char buffer[8];
  if (resetLCDFlag)
    Clear();

  // Print robot
  sprintf(buffer, "ID: %i", robot);            
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  // Print arm
  sprintf(buffer, "ARM: %i", arm_state);
  lcd.setCursor(8, 0);
  lcd.print(buffer);

  // Print left
  sprintf(buffer, "L:  %i", l_speed);
  lcd.setCursor(0, 1);
  lcd.print(buffer);

  // Print right
  sprintf(buffer, "R:   %i", r_speed);
  lcd.setCursor(8, 1);
  lcd.print(buffer);
}

void Clear() {
  resetLCDFlag = false;
  lcd.setCursor(4, 0);
  lcd.print("  ");
  lcd.setCursor(13, 0);
  lcd.print("  ");
  lcd.setCursor(4, 1);
  lcd.print("  ");
  lcd.setCursor(13, 1);
  lcd.print("  ");
}

void Get_Message() {
  message = "";
  while (Serial.available() > 0) {
    char received_char = Serial.read();
    message += received_char;
    if(received_char == '\n')
      break;
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