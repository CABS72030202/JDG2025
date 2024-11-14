// communication.c
// Created on: 2024-11-04
// Author: Sebastien Cabana

#include "../lib/communication.h"

int fd;
char* sent[BUFFER_SIZE];
char* received[BUFFER_SIZE];

int Start_Comm() {
  // Open the serial port
  if ((fd = serialOpen(SERIAL_PORT, BAUD_RATE)) < 0) {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return ERROR;
  }

  // Initialize wiringPi
  if (wiringPiSetup() == -1) {
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return ERROR;
  }
  printf("Started serial communication successfully.\n");
  return OK;
}

int Send(char* str) {
  Delay(1);
  printf("Sending: %s", str);
  for (int i = 0; i < strlen(str); i++) 
    serialPutchar(fd, str[i]);
  return OK;
}

int Receive(char* str) {
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
      if (dat == '\n') {
        Filter_Reception();
        printf("Received: %s", str);
        Delay(1);
        return OK;
      }

      fflush(stdout);
    }

    // Exit loop if timeout
    time(&current_time);
    if(difftime(current_time, start_time) >= TIMEOUT)
      break;
  }
  Empty_String(str);
  strcpy(str, "Timed out.\n");
  return ERROR;
}

void Filter_Reception() {
  // Check if disconnected from station
  if(strcmp(received, "ERR:disconnected\n") == 97 || strcmp(received, "STAT:disconnected\n") == 97 || strcmp(received, "STAT:connected\n") == 97) {
    Delay(1);
    Receive(received);  // Receive next word without sending anything
  }
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

int Try_Connect() {
  const int MAX_TRIES = 10;
  for(int i = 0; i < MAX_TRIES; i++) {
    printf("Waiting for connection. ");
    //Receive(received);
    //if(strcmp(received, "STAT:connected\n") == 97) {
    Color temp = Info_Color();
    if(temp != NONE) {
      // Set active station
      curr_station = Color_To_Station(temp);
      curr_station->state = ACTIVE;
      printf("Successfully connected to station %s.\n", Color_To_String(curr_station->color));
      return OK;
    }
    Delay(2);
  }
  printf("Could not connect to the station.\n");
  return ERROR;
}

/*void Test_Word(int fd, char* buffer, char* received) {
  int timeout_count = 0;
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
  if(strcmp(received, "ERR:unknown_command\n") != 97) {
              
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
}*/