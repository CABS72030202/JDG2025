CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest -lwiringPi
VPATH = lib:src:test
LIB = -lm

# Main executable construction
main: main.o station.o communication.o
	$(CC) $(CFLAGS) -o main main.o station.o communication.o $(LIB)

# Test unit executable construction
test_unit: test_unit.o
	$(CC) $(CFLAGS) -o test_unit test_unit.o $(CXXFLAGS) $(LIB)

# Object generation from source and header for MAIN
main.o: ./src/main.c ./lib/station.h ./lib/communication.h
	$(CC) $(CFLAGS) -c ./src/main.c -o main.o $(CXXFLAGS)

# Object generation from source and header for TEST_UNIT
test_unit.o: ./test/test_unit.h ./test/test_unit.c
	$(CC) $(CFLAGS) -c ./test/test_unit.c -o test_unit.o $(CXXFLAGS)	

# Object generation from source and header for STATION
station.o: ./lib/station.h ./src/station.c ./lib/communication.h ./src/communication.c
	$(CC) $(CFLAGS) -c ./src/station.c -o station.o $(CXXFLAGS)

# Object generation from source and header for COMMUNICATION
communication.o: ./lib/communication.h ./src/communication.c ./lib/station.h ./src/station.c
	$(CC) $(CFLAGS) -c ./src/communication.c -o communication.o $(CXXFLAGS)	

clean:
	rm -f *.o main test_unit