// mini_rpi.c
// Created on: 2024-12-18
// Author: Sebastien Cabana

#include "../lib/rpi_bluetooth.h"

int main() {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Unable to initialize pigpio: %s\n", strerror(errno));
        return 1;
    }

    // Initialize UART communication
    int uart_fd = uart_init(UART, BAUD_RATE);
    if (uart_fd < 0) {
        fprintf(stderr, "Unable to open UART device: %s\n", strerror(errno));
        gpioTerminate();
        return 1;
    }

    // Initialize Bluetooth communication as client
    int client_sock = bt_init(0, SERVER_ADR);
    if (client_sock < 0) {
        close(uart_fd);
        gpioTerminate();
        return 1;
    }

    while(1) {
        // Receive a string via Bluetooth
        char buffer[BUFFER_SIZE];
        if (bt_receive(client_sock, buffer, BUFFER_SIZE) < 0) {
            close(client_sock);
            close(uart_fd);
            gpioTerminate();
            return 1;
        }

        // Transmit message via UART
        if (uart_send(uart_fd, buffer) < 0) {
            fprintf(stderr, "Failed to send UART message: %s\n", strerror(errno));
            close(client_sock);
            close(uart_fd);
            gpioTerminate();
            return 1;
        }
    }

    // Cleanup
    close(client_sock);
    close(uart_fd);
    gpioTerminate();
    return 0;
}