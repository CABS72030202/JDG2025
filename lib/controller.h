// controller.h
// Created on: 2024-11-14
// Author: Sebastien Cabana

/* 
 * 
 */

#include <stdio.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#define GAMEPAD_PATH "/dev/input/js0"