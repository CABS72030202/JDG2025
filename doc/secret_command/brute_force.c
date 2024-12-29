/*
 * ----------------------------------------------------------------------------
 * File Name: brute_force.c
 *
 * Description:
 * This program attempts to brute-force find a predefined secret word by
 * generating all possible combinations of ASCII characters up to a specified
 * number of characters. The program systematically iterates through each
 * possible combination until the secret word is matched, or it exhausts
 * all potential combinations within the set length limit.
 *
 * Features:
 * - Allows customization of the maximum word length for brute-force attempts.
 * - Generates all ASCII character combinations to search for the secret word.
 * - Terminates once the secret word is found or all combinations are tested.
 * 
 * Usage:
 * 1. Build the executable file using the following parameter
 *    gcc -o brute_force brute_force.c -lwiringPi
 *
 * 2. To run the program normally (logging the date and time):
 *    ./brute_force
 *
 * 3. The program will display progress or indicate when the secret word is found.
 *
 * Note:
 * - The program may run for a prolonged period, depending on the length and 
 *   complexity of the secret word.
 * - Ensure that the predefined secret word is correctly set in the code.
 *
 * Requirements:
 * - The program must be compiled with a C compiler (e.g., GCC).
 * - Sufficient CPU time and memory may be required for large search spaces.
 * - The user must have permissions to access the serial port (/dev/tty??).
 *
 * Sources :
 * - https://www.electronicwings.com/raspberry-pi/raspberry-pi-uart-communication-using-python-and-c
 * - https://github.com/WiringPi/WiringPi/blob/master/examples/serialTest.c
 *
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <time.h>
#include <ctype.h>

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 115200
#define BUFFER_SIZE 32
#define TIMEOUT 5       // seconds
#define OUTPUT_FILE "output.txt"
#define INPUT_FILE "input.txt"

// Brute-force constants
#define DOUBLE_TEST 0
#define CAPS_TEST 0
#define MIN_LENGTH 1
#define MAX_LENGTH 2
#define START_CHAR 33
#define END_CHAR 122
const int SKIP_CHAR[] = {'"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ';', '<', '=', '>', '@', '[', '\\', ']', '^', '_', '`'};
const int SKIP_CHAR_SIZE = sizeof(SKIP_CHAR) / sizeof(SKIP_CHAR[0]);
#define START_STR "\0"
#define COMBINAISON_GEN 0

void Send(int, char*);
void Receive(int, char*);
void Empty_String(char*);
void Delay(int);
int Try_Connect(int, char*);
void Generate_Combinations(int, char*, int, int);
void Test_List(int, char*);
void Full_Caps(char*);
void Test_Word(int, char*, char*);
int Is_Skip_Char(char);
void Append_File(char*, char*);
void Save_Config();

int main() {
  int fd;
  char dat;
  char* sent = (char*)malloc(sizeof(char) * BUFFER_SIZE);
  char* received = (char*)malloc(sizeof(char) * BUFFER_SIZE);

  // Open the serial port
  if ((fd = serialOpen(SERIAL_PORT, BAUD_RATE)) < 0) {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }

  // Initialize wiringPi
  if (wiringPiSetup() == -1) {
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }

  // Send - Receive sequence after receiveing STAT:connected
  while(Try_Connect(fd, received)) {
    Save_Config();
    Test_List(fd, received);
    if(COMBINAISON_GEN)
      Generate_Combinations(fd, received, MIN_LENGTH, MAX_LENGTH);
    break;
  }

  serialClose(fd); // Close the serial port
  return 0;
}

void Send(int fd, char* str) {
  Delay(1);
  printf("Sending: %s", str);
  for (int i = 0; i < strlen(str); i++) 
    serialPutchar(fd, str[i]);
}

void Receive(int fd, char* str) {
  char dat;
  int index = 0;
  Empty_String(str);
  time_t start_time, current_time;
  time(&start_time);
  while (1) {
    // Check if data is available to read
    if (serialDataAvail(fd)) {
        
      // Read a character
      dat = serialGetchar(fd); 

      // Append received character
      str[index++] = dat; 

      // Exit loop on newline character
      if (dat == '\n')
        return;

      fflush(stdout);
    }

    // Exit loop if timeout
    time(&current_time);
    if(difftime(current_time, start_time) >= TIMEOUT)
      break;
  }
  Empty_String(str);
  strcpy(str, "Timed out.\n");
}

void Empty_String(char* str) { 
  for(int i = 0; i < BUFFER_SIZE; i++)
    str[i] = 0;
}

void Delay(int seconds) {
  time_t start_time, current_time;
  time(&start_time);
  while (1) {
    // Exit loop if timeout
    time(&current_time);
    if(difftime(current_time, start_time) >= seconds)
      break;
  }
}

int Try_Connect(int fd, char* str) {
  while(strcmp(str, "STAT:connected\n") != 97) {
    Receive(fd, str);
    //printf("Received while waiting: %s", str);
    Delay(1);
    /*if(strcmp(str, "Timed out.\n") == 0)
      return 0;*/
  }
  return 1;
}

void Generate_Combinations(int fd, char* received, int min_length, int max_length) {
  char buffer[max_length + 1];  // Buffer to hold each combination, +1 for null-terminator
  
  for (int length = min_length; length <= max_length; length++) {
    memset(buffer, START_CHAR, length);  // Initialize buffer with START_CHAR
    buffer[length] = '\n';  
    buffer[length + 1] = '\0';  // Null-terminate the string
    
    if(strcmp(START_STR, "\0") != 0) {
      Empty_String(buffer);
      int i;
      for(i = 0; i < strlen(START_STR); i++)
        buffer[i] = START_STR[i];
      length = i;
      buffer[i++] = '\n';  
      buffer[i] = '\0';  // Null-terminate the string
    }

    while (1) {
      Test_Word(fd, buffer, received);
      if(DOUBLE_TEST) {
        buffer[length] = '\n';  
        buffer[length + 1] = '\0';  // Null-terminate the string
      }

      // Move backwards through the buffer to find the first character that can be incremented
      int pos = length - 1;
      while (pos >= 0) {
        // Increment character
        buffer[pos]++;
                
        // Skip any values in SKIP_CHAR
        while (Is_Skip_Char(buffer[pos])) 
          buffer[pos]++;
                
        // Check if within range
        if (buffer[pos] <= END_CHAR) 
          break;  // Found a valid character, break out of loop
                
        // Reset this position to START_CHAR and move to the previous character
        buffer[pos] = START_CHAR;
        pos--;  
      }
      if (pos < 0) break;  // If no position was found, we are done with this length
    }
  }
}

void Test_List(int fd, char* received) {
  char buffer[32];
  FILE *input = fopen(INPUT_FILE, "r");
  if (input == NULL) {
    perror("Failed to open file");
    return;
  }
  while(fgets(buffer, 32, input)) {
    Test_Word(fd, buffer, received);
    Empty_String(buffer);
  }
  fclose(input);
}

void Full_Caps(char* str) {
  if (str == NULL) return;    // Check if the string is not NULL
  while (*str) {              // Loop through each character
    *str = toupper(*str);     // Convert character to uppercase
    str++;                    // Move to the next character
  }
}

void Test_Word(int fd, char* buffer, char* received) {
  int timeout_count = 0;
  if(CAPS_TEST)
    Full_Caps(buffer);
  A:
  //printf("%s", buffer);  // Print the current combination
  Send(fd, buffer);
  Receive(fd, received);
            
  // Check if disconnected from station
  if(strcmp(received, "ERR:disconnected\n") == 97 || strcmp(received, "STAT:disconnected\n") == 97) {
    printf("Disconnected at word: %s", buffer);
    Append_File(buffer, "#Disconnected at word: ");
              
    // Wait until reconnected
    while(!Try_Connect(fd, received));
  }

  // Print combination if response is different than unknown_command (might be valid)
  if(strcmp(received, "ERR:unknown_command\n") != 97 && strcmp(received, "ERR:command_too_long\n") != 97) {
              
    // Print combination if no response from blackbox (might be valid, like INFO:)
    if(strcmp(received, "Timed out.\n") == 0) {
      printf("Timed out after sending: %s", buffer);
      Append_File(buffer, "!Timed out after sending: ");
      timeout_count++;

      // Second timeout in a row, must be an error
      if(timeout_count == 2) {    
        timeout_count = 0;

      // Restart serial communication
      serialClose(fd);
      if ((fd = serialOpen(SERIAL_PORT, BAUD_RATE)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        Append_File(buffer, "!Could not restart serial communication after timeout at word: ");
        exit(1);
      }

      // Wait until station reset
      printf("Serial communication has been restarted. Now waiting until station resets.\n");
      while(!Try_Connect(fd, received));
      Append_File(buffer, "#Done waiting for reconnection at word: ");
      goto A; // Then try word again
      }
    }

    // Check if reconnected to station
    else if(strcmp(received, "STAT:connected\n") == 97) {
      printf("Reconnected at word: %s", buffer);
      Append_File(buffer, "#Reconnected at word: ");
    }

    // Print combination and received message if unexepected response
    else {
      printf("\nSent: %sReceived: %s\n", buffer, received);
      Append_File(buffer, received);
    }             
  }

  // Test the word a second time but with : at the end
  if(DOUBLE_TEST) {
    int i;
    for(i = 0; i < strlen(buffer); i++)
      if(buffer[i] == '\n')
        break;
    if(i > 0 && buffer[i - 1] == ':') 
      return;
    buffer[i] = ':';
    buffer[i + 1] = '\n';
    Test_Word(fd, buffer, received);
  }
}

int Is_Skip_Char(char c) {
  for (int i = 0; i < SKIP_CHAR_SIZE; i++)
    if (c == SKIP_CHAR[i])
      return 1;
  return 0;
}

void Append_File(char* sent, char* received) {
  FILE *outputFile = fopen(OUTPUT_FILE, "a");
  if (outputFile == NULL) {
    perror("Failed to open file");
    return;
  }
  fprintf(outputFile, "%s %s", received, sent);
  fclose(outputFile);
}

void Save_Config() {
  FILE *output = fopen(OUTPUT_FILE, "w");
  if (output == NULL) {
    perror("Failed to open file");
    return;
  }
  fprintf(output, "- BRUTE FORCE CONFIG -\n");
  fprintf(output, "- Words of length %d to %d\n", MIN_LENGTH, MAX_LENGTH);
  fprintf(output, "- Starting ASCII table at '%c'\n", START_CHAR);
  fprintf(output, "- Ending ASCII table at '%c'\n", END_CHAR);
  fprintf(output, "- Ignoring characters: \"");
    
  // Print the characters to be skipped
  for (int i = 0; i < SKIP_CHAR_SIZE; i++) {
    fprintf(output, "%c", SKIP_CHAR[i]);
    if (i < SKIP_CHAR_SIZE - 1) 
      fprintf(output, ", "); // Add comma between characters
  }
  fprintf(output, "\"\n");
  fprintf(output, "- Starting at word %s\n", START_STR);
  fclose(output);
}