#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize the LCD, set the I2C address (0x27) and LCD size (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();               // Initialize the LCD
  lcd.backlight();          // Turn on the backlight
  lcd.setCursor(0, 0);      // Set cursor to the first column of the first row
  Serial.begin(115200);     // Initialize UART communication at 9600 baud rate
}

void loop() {
  // Check if data is available to read from the serial buffer
  if (Serial.available() > 0) {
    String message = "";    // Variable to store the received message

    // Read all available characters
    while (Serial.available() > 0) {
      char c = Serial.read();  // Read a character from the serial buffer
      message += c;            // Append it to the message string
      delay(10);               // Small delay to ensure proper data reading
    }

    // Display the message on the LCD
    lcd.clear();               // Clear the previous display
    lcd.setCursor(0, 0);       // Reset cursor to the first column of the first row
    lcd.print(message);        // Print the message on the LCD
  }
}
