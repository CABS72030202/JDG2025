// communication.c
// Created on: 2024-11-04
// Author: Sebastien Cabana

#include "../lib/communication.h"

int fd;
char* sent[BUFFER_SIZE];
char* received[BUFFER_SIZE];

int Start_Comm() {
    // Open the serial port
    fd = serOpen(SERIAL_PORT, BAUD_RATE, 0);
    if (fd < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return -1;
    }

    // Initialize pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Unable to initialize pigpio: %s\n", strerror(errno));
        return -1;
    }
    printf("Started serial communication successfully.\n");
    return 0;
}

int Send(char* str) {
    printf("Sending: %s", str);
    int result = serWrite(fd, str, strlen(str));
    if (result < 0) {
        fprintf(stderr, "Error sending data: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int Receive(char* str) {
    char dat;
    int index = 0;
    Empty_String(str);
    time_t start_time, current_time;
    time(&start_time);

    while (1) {
        // Check if data is available to read
        if (serDataAvailable(fd) > 0) {
            // Read a character
            dat = serReadByte(fd);

            // Append received character
            str[index++] = dat;

            // Exit loop on newline character
            if (dat == '\n') {
                str[index] = '\0';  // Null-terminate the string
                Filter_Reception();
                printf("Received: %s", str);
                return 0;
            }
        }

        // Exit loop if timeout
        time(&current_time);
        if (difftime(current_time, start_time) >= TIMEOUT) {
            break;
        }
    }

    Empty_String(str);
    strcpy(str, "Timed out.\n");
    return -1;
}

void Filter_Reception() {
    // Check if disconnected from station
    if (strcmp(received, "ERR:disconnected\n") == 0 ||
        strcmp(received, "STAT:disconnected\n") == 0 ||
        strcmp(received, "STAT:connected\n") == 0) {
        Delay(1);
        Receive(received);  // Receive next word without sending anything
    }
}

void Empty_String(char* str) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        str[i] = 0;
    }
}

void Delay(int seconds) {
    time_t start_time, current_time;
    time(&start_time);
    while (1) {
        // Exit loop if timeout
        time(&current_time);
        if (difftime(current_time, start_time) >= seconds) {
            break;
        }
    }
}