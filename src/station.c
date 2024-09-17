// station.c
// Created on: 2024-09-17
// Author: Sebastien Cabana

/* 
 * 
 */

#include<stdio.h>
#include<string.h>

enum color {RED, GREEN, BLUE, YELLOW, PURPLE, NONE};
typedef enum color Color;

typedef struct {
    Color color;        // Station color
    int passengers[5];  // R:G:B:Y:P
    int time_limit;     // Seconds left before penalty
} Station;

