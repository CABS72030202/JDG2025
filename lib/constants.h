/*
 * constants.h
 * Created on: 2025-01-02
 * Author: Sebastien Cabana
 *
 * Description:
 * This header file consolidates all constants utilized across the source code files. 
 * By centralizing these definitions, it simplifies debugging and ensures consistent 
 * reference throughout the project. The file serves as a single source of truth for 
 * constant values, promoting maintainability and reducing the risk of discrepancies.
 */

// Librairies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdbool.h>

/************************************************************************************
********************************** CONTROLLER ***************************************
************************************************************************************/

    // ----- //
    // DEBUG //
    // ----- //
#define SKIP_BLUETOOTH          1               // Set to 1 to disable Bluetooth initialization
#define USER_CAN_CONTROL_ARM    1               // Set to 1 to enable manual arm control

    // ---------------- //
    // GLOBAL CONSTANTS //
    // ---------------- //

#define GAMEPAD_PATH       "/dev/input/js0"      // Path to the gamepad device
#define BLOCKING_MODE      0                     // Set to 1 to wait for controller updates
const int robot_order[] = {6, 2, 1, 3, 0, 4, 5}; // Order of robot selection

    // ------------- //
    // ANALOG RANGES //
    // ------------- //

#define ANAL_SHIFT_VAL     32767                 // Shift value for analog inputs
#define ANAL_MAX_VAL       65534                 // Maximum value for analog inputs
#define AXIS_RANGE_COUNT   4                     // Number of axis ranges (includes 0)
#define JOYSTICK_THRESHOLD 10000                 // Threshold to detect joystick movement

/************************************************************************************
************************************** BOAT *****************************************
************************************************************************************/

    // ------- //
    // CONTROL //
    // ------- //

#define BOAT_ID                     6               // Unique ID for the BOAT robot
#define MAX_SPEED                   3               // Maximum speed multiplier for the robot
const int BOAT_SPEEDS[MAX_SPEED] =  {60, 70, 80};   // Preset duty cycle values for brushless motors speed

    // ---------------- //
    // BRUSHLESS MOTORS //
    // ---------------- //

// Pin Definitions
#define LEFT_MOTOR_PWM_PIN    23      // GPIO Pin for the left motor PWM
#define LEFT_POWER_GPIO_PIN   24      // GPIO Pin to provide 5V power to the left motor
#define RIGHT_MOTOR_PWM_PIN   26      // GPIO Pin for the right motor PWM
#define RIGHT_POWER_GPIO_PIN  22      // GPIO Pin to provide 5V power to the right motor
// PWM Constants
#define PWM_RANGE           1024         // Range for PWM (0-1023)
#define PWM_CLOCK_DIV       384          // Clock divisor for 50 Hz frequency (19.2 MHz / (384 * 1024) = 50 Hz)
#define DUTY_CYCLE_STOP     51           // 5% duty cycle (5% of 1024)
#define DUTY_CYCLE_MAX      102          // 10% duty cycle (10% of 1024)
#define DUTY_CYCLE_LIMIT    93           // Maximum duty cycle accounting for load
#define INIT_DELAY          1500         // Time delay for brushless motor initialization
#define INCREASE_DELAY      1            // Time delay when increasing speed (in ms)

    // ----------- //
    // SERVOMOTORS //
    // ----------- //

// Pin Definitions
#define RAMP_SERVO_PIN      7
#define SWITCH_SERVO_PIN    28
// Global Constants
#define SERVO_MIN_PULSE_WIDTH   0       // Minimum pulse width for the servo (µs)
#define SERVO_MAX_PULSE_WIDTH   100     // Maximum pulse width for the servo (µs)
#define TURN_TIME_PER_DEGREE    10      // Typical time in milliseconds to turn 1 degree
// Angle values
#define RAMP_SERVO_OPEN         10      // Pulse width for open position
#define RAMP_SERVO_CLOSE        20      // Pulse width for close position
#define SWITCH_SERVO_OPEN       0       // Pulse width for open position
#define SWITCH_SERVO_CLOSE      365     // Pulse width for close position

    // ---- //
    // LEDS //
    // ---- //

#define LED_COUNT       5        // Number of LEDs
#define GPIO_PIN        21       // GPIO pin number (BCM mode)
#define LED_FREQ_HZ     800000   // Frequency of WS2811 LEDs
#define LED_DMA         10       // DMA channel
#define LED_BRIGHTNESS  255      // Maximum brightness
#define LED_CHANNEL     0        // Channel for controlling LEDs

/************************************************************************************
************************************ STATION ****************************************
************************************************************************************/

    // ----- //
    // DEBUG //
    // ----- //

#define DEBUG_MODE 		0
#define ACTIVE_LOOP 	1
#define MAX_ITERATIONS	1000
#define USER_INPUT	    0

    // ---------------- //
    // GLOBAL CONSTANTS //
    // ---------------- //

#define ERROR           1
#define OK              0
#define ONBOARD_TIMEOUT 2		// Delay to show passengers onboard

/************************************************************************************
********************************* COMMUNICATION *************************************
************************************************************************************/

    // ---------------- //
    // GLOBAL CONSTANTS //
    // ---------------- //
#define BUFFER_SIZE     32                      // Size of communication buffer
#define TIMEOUT         5                       // Communication timeout in seconds

    // ---- //
    // UART //
    // ---- //
#define BAUD_RATE       115200                  // Baud rate for serial communication
#define SERIAL_PORT     "/dev/ttyUSB0"          // Serial port for communication with the blackbox
#define UART            "/dev/serial0"          // UART device path

    // --------- //
    // BLUETOOTH //
    // --------- //
#define CHANNEL         1                       // RFCOMM communication channel
#define SERVER_ADR      "B8:27:EB:A9:8C:AB"     // Default server Bluetooth address

/************************************************************************************
************************************** GPIO *****************************************
************************************************************************************/

    // ------------ //
    // COMMAND PINS //
    // ------------ //
    
#define ARM_OUT_PIN_0   0    // GPIO 0 - PIN 11
#define ARM_OUT_PIN_1   3    // GPIO 3 - PIN 15
#define ARM_OUT_PIN_2   5    // GPIO 5 - PIN 18
#define ARM_OUT_PIN_3   6    // GPIO 6 - PIN 22
#define ARM_IN_PIN_0    2    // GPIO 2 - PIN 13
#define ARM_IN_PIN_1    4    // GPIO 4 - PIN 16
#define ARM_IN_PIN_2    25   // GPIO 25 - PIN 37
#define ARM_IN_PIN_3    27   // GPIO 27 - PIN 36

    // ---------- //
    // STATE PINS //
    // ---------- //

#define R_QUEUE_PIN     12   // GPIO 12 - PIN 19
#define G_QUEUE_PIN     13   // GPIO 13 - PIN 21
#define B_QUEUE_PIN     14   // GPIO 14 - PIN 23
#define Y_QUEUE_PIN     28   // GPIO 28 - PIN 38
#define P_QUEUE_PIN     29   // GPIO 29 - PIN 40

    // ----------- //
    //  STATE IDS //
    // ----------- //

#define DOWN_ALL        0
#define RED_UP          1
#define RED_DOWN        2
#define GREEN_UP        3
#define GREEN_DOWN      4
#define BLUE_UP         5
#define BLUE_DOWN       6
#define YELLOW_UP       7
#define YELLOW_DOWN     8
#define PURPLE_UP       9
#define PURPLE_DOWN     10
#define RED_READY       11
#define GREEN_READY     12
#define BLUE_READY      13
#define YELLOW_READY    14
#define PURPLE_READY    15