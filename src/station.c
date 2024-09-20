// station.c
// Created on: 2024-09-17
// Author: Sebastien Cabana

/* 
 * 
 */

#include<stdio.h>
#include<string.h>

// Approximative number of seconds required to reach each station
// (these are experimental values)
enum color {RED, GREEN, BLUE, YELLOW, PURPLE, NONE};
typedef enum color Color;
enum state {INACTIVE, ACTIVE};
typedef enum state State;

typedef struct {
    Color color;        // Station color
    int passengers[5];  // R:G:B:Y:P
    State state;    
} Station;

