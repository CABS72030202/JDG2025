// mini_rpi.c
// Created on: 2024-12-18
// Author: Sebastien Cabana

#include "../lib/rpi_bluetooth.h"

int main() {
     // Open the serial port
    int uart_fd = serialOpen(UART, BAUD_RATE);
    if (uart_fd < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    // Initialize Bluetooth communication as client
    int client_sock = bt_init(0, SERVER_ADR);
    if (client_sock < 0) return 1;

    while(1) {
        // Receive a string
        char buffer[BUFFER_SIZE];
        if (bt_receive(client_sock, buffer, BUFFER_SIZE) < 0) {
            close(client_sock);
            return 1;
        }

        // Transmit message via UART
        for (int i = 0; i < strlen(buffer); i++) 
            serialPutchar(uart_fd, buffer[i]);
    }

    close(client_sock);
    return 0;
}