CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest
VPATH = lib:src:test
LIB = -lm

# Main executable construction
main: main.o station.o
	$(CC) $(CFLAGS) -o main main.o station.o $(LIB)

# Test unit executable construction
test_unit: test_unit.o
	$(CC) $(CFLAGS) -o test_unit test_unit.o $(CXXFLAGS) $(LIB)

# Object generation from source and header for MAIN
main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o main.o $(CXXFLAGS)

# Object generation from source and header for TEST_UNIT
test_unit.o: ./test/test_unit.c ./test/test_unit.h
	$(CC) $(CFLAGS) -c ./test/test_unit.c $(CXXFLAGS)	

# Object generation from source and header for STATION
station.o: ./src/station.c
	$(CC) $(CFLAGS) -c ./src/station.c -o station.o $(CXXFLAGS)

clean:
	rm -f *.o main test_unit