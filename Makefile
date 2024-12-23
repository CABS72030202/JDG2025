CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest
VPATH = lib:src:test
LIB = -lm -lwiringPi -lbluetooth

# Blackbox executable construction
blackbox: blackbox.o station.o communication.o gpio.o
	$(CC) $(CFLAGS) -o blackbox blackbox.o station.o communication.o gpio.o $(LIB)

# Controller executable construction
controller: controller.o gpio.o rpi_bluetooth.o boat_control.o
	$(CC) $(CFLAGS) -o controller controller.o gpio.o rpi_bluetooth.o boat_control.o $(LIB)

# Bluetooth receiver executable construction
mini_rpi: mini_rpi.o rpi_bluetooth.o
	$(CC) $(CFLAGS) -o mini_rpi mini_rpi.o rpi_bluetooth.o $(LIB)

# Object generation from source and header for BLACKBOX
blackbox.o: ./src/blackbox.c ./lib/station.h ./src/station.c ./lib/communication.h ./src/communication.c ./lib/gpio.h ./src/gpio.c
	$(CC) $(CFLAGS) -c ./src/blackbox.c -o blackbox.o $(CXXFLAGS)

# Object generation from source and header for BLACKBOX
controller.o: ./src/controller.c ./lib/controller.h ./lib/gpio.h ./src/gpio.c ./lib/rpi_bluetooth.h ./src/rpi_bluetooth.c
	$(CC) $(CFLAGS) -c ./src/controller.c -o controller.o $(CXXFLAGS)

# Object generation from source and header for MINI_RPI
mini_rpi.o: ./src/mini_rpi.c ./lib/rpi_bluetooth.h ./src/rpi_bluetooth.c
	$(CC) $(CFLAGS) -c ./src/mini_rpi.c -o mini_rpi.o $(CXXFLAGS)

# Object generation from source and header for RPI_BLUETOOTH
rpi_bluetooth.o: ./src/rpi_bluetooth.c ./lib/rpi_bluetooth.h
	$(CC) $(CFLAGS) -c ./src/rpi_bluetooth.c -o rpi_bluetooth.o $(CXXFLAGS)

# Object generation from source and header for STATION
station.o: ./lib/station.h ./src/station.c ./lib/communication.h ./src/communication.c ./lib/gpio.h ./src/gpio.c
	$(CC) $(CFLAGS) -c ./src/station.c -o station.o $(CXXFLAGS)

# Object generation from source and header for COMMUNICATION
communication.o: ./lib/communication.h ./src/communication.c ./lib/station.h ./src/station.c
	$(CC) $(CFLAGS) -c ./src/communication.c -o communication.o $(CXXFLAGS)	

# Object generation from source and header for GPIO
gpio.o: ./lib/gpio.h ./src/gpio.c 
	$(CC) $(CFLAGS) -c ./src/gpio.c -o gpio.o $(CXXFLAGS)	

# Object generation from source and header for BOAT_CONTROL
boat_control.o: ./lib/boat_control.h ./src/boat_control.c 
	$(CC) $(CFLAGS) -c ./src/boat_control.c -o boat_control.o $(CXXFLAGS)	

clean:
	rm -f *.o blackbox controller mini_rpi