#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "countdown.h"
#include "lcd.h"
#include "buzzer.h"

// Countdown variables
static uint16_t countdown_time = 0;
static uint16_t countdown_current = 0;
static countdown_state_t countdown_state = COUNTDOWN_STOPPED;

// Initialize countdown
void countdown_init(void)
{
    countdown_time = 0;
    countdown_current = 0;
    countdown_state = COUNTDOWN_STOPPED;
}

// Set countdown time in seconds
void countdown_set(uint16_t seconds)
{
    countdown_time = seconds;
    countdown_current = seconds;
}

// Start countdown
void countdown_start(void)
{
    if (countdown_state == COUNTDOWN_STOPPED && countdown_time > 0) {
        countdown_state = COUNTDOWN_RUNNING;
    }
}

// Stop countdown
void countdown_stop(void)
{
    if (countdown_state == COUNTDOWN_RUNNING) {
        countdown_state = COUNTDOWN_STOPPED;
    }
}

// Reset countdown
void countdown_reset(void)
{
    countdown_current = countdown_time;
    countdown_state = COUNTDOWN_STOPPED;
}

// Update countdown (called every second)
void countdown_update(void)
{
    if (countdown_state == COUNTDOWN_RUNNING) {
        countdown_decrement();
    }
}

// Check if countdown is running
bool countdown_is_running(void)
{
    return (countdown_state == COUNTDOWN_RUNNING);
}

// Check if countdown is finished
bool countdown_is_finished(void)
{
    return (countdown_state == COUNTDOWN_FINISHED);
}

// Get current countdown time
uint16_t countdown_get_time(void)
{
    return countdown_current;
}

// Display countdown time on LCD
void countdown_display(void)
{
    char time_str[16];
    countdown_format_time(time_str);
    lcd_print(time_str);
}

// Decrement countdown time
void countdown_decrement(void)
{
    if (countdown_current > 0) {
        countdown_current--;
        
        if (countdown_current == 0) {
            countdown_state = COUNTDOWN_FINISHED;
            // Trigger buzzer when countdown finishes
            buzzer_beep(1000);
        }
    }
}

// Format countdown time to string
void countdown_format_time(char* buffer)
{
    uint8_t minutes = countdown_get_minutes();
    uint8_t seconds = countdown_get_seconds();
    sprintf(buffer, "%02d:%02d", minutes, seconds);
}

// Get minutes from countdown time
uint8_t countdown_get_minutes(void)
{
    return (uint8_t)(countdown_current / 60);
}

// Get seconds from countdown time
uint8_t countdown_get_seconds(void)
{
    return (uint8_t)(countdown_current % 60);
} 