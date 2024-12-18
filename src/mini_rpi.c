// mini_rpi.c
// Created on: 2024-12-18
// Author: Sebastien Cabana

#include "../../lib/rpi_bluetooth.h"

int main() {
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
    }

    close(client_sock);
    return 0;
}