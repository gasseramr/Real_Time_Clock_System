#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "buttons.h"

// Button matrix state
static uint8_t button_states[4] = {0};
static uint8_t last_button_states[4] = {0};
static uint8_t button_press_flags[4] = {0};

void buttons_init(void)
{
    // Configure row pins as outputs
    DDRB |= (1 << ROW1_PIN) | (1 << ROW2_PIN);
    
    // Configure column pins as inputs with pull-up
    DDRB &= ~((1 << COL1_PIN) | (1 << COL2_PIN));
    
    // Enable pull-up resistors for columns
    PORTB |= (1 << COL1_PIN) | (1 << COL2_PIN);
    
    // Initialize button states
    for (uint8_t i = 0; i < 4; i++) {
        button_states[i] = 0;
        last_button_states[i] = 0;
        button_press_flags[i] = 0;
    }
}

void buttons_read_input(void)
{
    uint8_t row, col;
    uint8_t button_index = 0;
    
    // Save last button states
    for (uint8_t i = 0; i < 4; i++) {
        last_button_states[i] = button_states[i];
        button_states[i] = 0;
    }
    
    // Scan 2 rows (R1 and R2)
    for (row = 0; row < 2; row++) {
        // Set all rows high first
        PORTB |= (1 << ROW1_PIN) | (1 << ROW2_PIN);
        
        // Set current row low
        PORTB &= ~(1 << (ROW1_PIN + row));
        
        _delay_us(10); // Small delay for settling
        
        // Read 2 columns (C1 and C2)
        for (col = 0; col < 2; col++) {
            bool pressed = !(PINB & (1 << (COL1_PIN + col)));
            
            if (pressed && button_index < 4) {
                button_states[button_index] = 1;
                if (last_button_states[button_index] == 0) {
                    button_press_flags[button_index] = 1;
                }
            }
            button_index++;
        }
    }
}

bool button_is_pressed(uint8_t button)
{
    if (button < 4) {
        return (button_states[button] == 1 && last_button_states[button] == 0);
    }
    return false;
}

bool button_was_pressed(uint8_t button)
{
    if (button < 4) {
        return button_press_flags[button];
    }
    return false;
}

void button_clear_press(uint8_t button)
{
    if (button < 4) {
        button_press_flags[button] = 0;
    }
}

uint8_t get_pressed_button(void)
{
    for (uint8_t i = 0; i < 4; i++) {
        if (button_is_pressed(i)) {
            return i;
        }
    }
    return 0xFF; // No button pressed
}

uint8_t button_get_state(uint8_t button)
{
    if (button < 4) {
        return button_states[button];
    }
    return 0;
} 