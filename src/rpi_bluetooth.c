// rpi_bluetooth.c
// Created on: 2024-12-18
// Author: Sébastien Cabana

#include "../lib/rpi_bluetooth.h"

/**
 * Initialize the Bluetooth RFCOMM server or client socket.
 * @param server: A boolean flag (1 for server mode, 0 for client mode).
 * @param dest: Bluetooth address of the remote device (only required in client mode).
 * @return: A socket file descriptor (positive integer) on success, -1 on failure.
 */
int bt_init(int server, const char *dest) {
    struct sockaddr_rc addr = { 0 };
    int sock;

    // Create RFCOMM socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    if (server) {
        // Server setup
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_bdaddr = *BDADDR_ANY;
        addr.rc_channel = CHANNEL;

        // Bind the socket
        if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("Failed to bind socket");
            close(sock);
            return -1;
        }

        // Listen for incoming connections
        if (listen(sock, 1) < 0) {
            perror("Failed to listen on socket");
            close(sock);
            return -1;
        }

        printf("Bluetooth server listening on channel %d...\n", CHANNEL);
    } else {
        // Client setup
        addr.rc_family = AF_BLUETOOTH;
        str2ba(dest, &addr.rc_bdaddr); // Convert address string to bdaddr_t
        addr.rc_channel = CHANNEL;

        printf("Connecting to Bluetooth server at %s...\n", dest);

        // Connect to server
        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("Failed to connect to server");
            close(sock);
            return -1;
        }

        printf("Connected to server at %s\n", dest);
    }

    return sock;
}

/**
 * Send a string over the specified Bluetooth socket.
 * @param sock: Socket file descriptor.
 * @param message: String to send.
 * @return: 0 on success, -1 on failure.
 */
int bt_send(int sock, const char *message) {
    int len = send(sock, message, strlen(message), 0);
    if (len < 0) {
        perror("Failed to send message");
        return -1;
    }
    return 0;
}

/**
 * Receive a string from the Bluetooth socket.
 * @param sock: Socket file descriptor.
 * @param buffer: Buffer to store the received string.
 * @param buffer_size: Size of the buffer.
 * @return: 0 on success, -1 on failure.
 */
int bt_receive(int sock, char *buffer, int buffer_size) {
    memset(buffer, 0, buffer_size);
    int len = recv(sock, buffer, buffer_size - 1, 0);
    if (len <= 0) {
        perror("Failed to receive message");
        return -1;
    }
    printf("Received: %s", buffer);
    return 0;
}

/**
 * Initialize UART communication.
 * @param device: Path to the UART device (e.g., "/dev/serial0").
 * @param baud_rate: Baud rate for communication.
 * @return: File descriptor for the UART device, or -1 on failure.
 */
int uart_init(const char *device, int baud_rate) {
    int uart_fd = serOpen(device, baud_rate, 0);
    if (uart_fd < 0) {
        fprintf(stderr, "Failed to initialize UART: %s\n", strerror(errno));
        return -1;
    }
    return uart_fd;
}

/**
 * Send data over UART.
 * @param uart_fd: UART file descriptor.
 * @param message: String to send.
 * @return: 0 on success, -1 on failure.
 */
int uart_send(int uart_fd, const char *message) {
    if (serWrite(uart_fd, message, strlen(message)) < 0) {
        fprintf(stderr, "Failed to send UART message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/**
 * Receive data over UART.
 * @param uart_fd: UART file descriptor.
 * @param buffer: Buffer to store the received data.
 * @param buffer_size: Size of the buffer.
 * @return: Number of bytes received, or -1 on failure.
 */
int uart_receive(int uart_fd, char *buffer, int buffer_size) {
    int bytes_read = serRead(uart_fd, buffer, buffer_size);
    if (bytes_read < 0) {
        fprintf(stderr, "Failed to receive UART message: %s\n", strerror(errno));
        return -1;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the received string
    return bytes_read;
}
