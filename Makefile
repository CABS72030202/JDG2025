# Define build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Compiler settings
CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest
VPATH = lib:src:test
LIB = -lm -lpigpio -lbluetooth

# Create build directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BUILD_DIR))

# Default construction
all: blackbox controller mini_rpi

# Blackbox executable construction
blackbox: $(OBJ_DIR)/blackbox.o $(OBJ_DIR)/station.o $(OBJ_DIR)/communication.o $(OBJ_DIR)/gpio.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/blackbox $(OBJ_DIR)/blackbox.o $(OBJ_DIR)/station.o $(OBJ_DIR)/communication.o $(OBJ_DIR)/gpio.o $(LIB)

# Controller executable construction
controller: $(OBJ_DIR)/controller.o $(OBJ_DIR)/gpio.o $(OBJ_DIR)/rpi_bluetooth.o $(OBJ_DIR)/boat_control.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/controller $(OBJ_DIR)/controller.o $(OBJ_DIR)/gpio.o $(OBJ_DIR)/rpi_bluetooth.o $(OBJ_DIR)/boat_control.o $(LIB)

# Bluetooth receiver executable construction
mini_rpi: $(OBJ_DIR)/mini_rpi.o $(OBJ_DIR)/rpi_bluetooth.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/mini_rpi $(OBJ_DIR)/mini_rpi.o $(OBJ_DIR)/rpi_bluetooth.o $(LIB)

# Object generation from source and header for BLACKBOX
$(OBJ_DIR)/blackbox.o: ./src/blackbox.c ./lib/station.h ./src/station.c ./lib/communication.h ./src/communication.c ./lib/gpio.h ./src/gpio.c
	$(CC) $(CFLAGS) -c ./src/blackbox.c -o $(OBJ_DIR)/blackbox.o $(CXXFLAGS)

# Object generation from source and header for CONTROLLER
$(OBJ_DIR)/controller.o: ./src/controller.c ./lib/controller.h ./lib/gpio.h ./src/gpio.c ./lib/rpi_bluetooth.h ./src/rpi_bluetooth.c
	$(CC) $(CFLAGS) -c ./src/controller.c -o $(OBJ_DIR)/controller.o $(CXXFLAGS)

# Object generation from source and header for MINI_RPI
$(OBJ_DIR)/mini_rpi.o: ./src/mini_rpi.c ./lib/rpi_bluetooth.h ./src/rpi_bluetooth.c
	$(CC) $(CFLAGS) -c ./src/mini_rpi.c -o $(OBJ_DIR)/mini_rpi.o $(CXXFLAGS)

# Object generation from source and header for RPI_BLUETOOTH
$(OBJ_DIR)/rpi_bluetooth.o: ./src/rpi_bluetooth.c ./lib/rpi_bluetooth.h
	$(CC) $(CFLAGS) -c ./src/rpi_bluetooth.c -o $(OBJ_DIR)/rpi_bluetooth.o $(CXXFLAGS)

# Object generation from source and header for STATION
$(OBJ_DIR)/station.o: ./lib/station.h ./src/station.c ./lib/communication.h ./src/communication.c ./lib/gpio.h ./src/gpio.c
	$(CC) $(CFLAGS) -c ./src/station.c -o $(OBJ_DIR)/station.o $(CXXFLAGS)

# Object generation from source and header for COMMUNICATION
$(OBJ_DIR)/communication.o: ./lib/communication.h ./src/communication.c ./lib/station.h ./src/station.c
	$(CC) $(CFLAGS) -c ./src/communication.c -o $(OBJ_DIR)/communication.o $(CXXFLAGS)

# Object generation from source and header for GPIO
$(OBJ_DIR)/gpio.o: ./lib/gpio.h ./src/gpio.c
	$(CC) $(CFLAGS) -c ./src/gpio.c -o $(OBJ_DIR)/gpio.o $(CXXFLAGS)

# Object generation from source and header for BOAT_CONTROL
$(OBJ_DIR)/boat_control.o: ./lib/boat_control.h ./src/boat_control.c
	$(CC) $(CFLAGS) -c ./src/boat_control.c -o $(OBJ_DIR)/boat_control.o $(CXXFLAGS)

# Clean up the build directories
clean:
	rm -rf $(BUILD_DIR)/*