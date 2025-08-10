#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include <stdbool.h>

// Button matrix pins (4-button version to avoid I2C conflicts)
#define ROW1_PIN    PB0  // R1
#define ROW2_PIN    PB1  // R2

#define COL1_PIN    PB4  // C1
#define COL2_PIN    PB5  // C2

// Button definitions (4 buttons only)
#define BTN_MODE        0  // R1C1 - MODE button
#define BTN_SET         1  // R1C2 - SET button
#define BTN_START       2  // R2C1 - START button
#define BTN_STOP        3  // R2C2 - STOP button

// Button states
#define BTN_PRESSED         0
#define BTN_RELEASED        1

// Debounce time in milliseconds
#define BTN_DEBOUNCE_TIME   50

// Function prototypes
void buttons_init(void);
void buttons_read_input(void);
bool button_is_pressed(uint8_t button);
bool button_was_pressed(uint8_t button);
void button_clear_press(uint8_t button);
uint8_t get_pressed_button(void);

#endif // BUTTONS_H 