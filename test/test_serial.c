#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/serial0"
#define BAUD_RATE 115200
#define BUFFER_SIZE 5
#define ITERATIONS 10
#define SEND "TEST\n"

int main() {
    int fd;

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

    // Main loop
    for(int j = 0; j < ITERATIONS; j++) {

        //Send
        for (int i = 0; i < strlen(SEND); i++) 
            serialPutchar(fd, SEND[i]);

        // Delay
        printf("Sleep for 2 sec\n");
        sleep(2);
    }

    serialClose(fd);
    printf("\nExit with 0\n");
    return 0;
}