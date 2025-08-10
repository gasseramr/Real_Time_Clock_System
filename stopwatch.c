#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stopwatch.h"
#include "lcd.h"

// Stopwatch variables
static stopwatch_time_t stopwatch_time = {0, 0, 0};
static stopwatch_state_t stopwatch_state = STOPWATCH_STOPPED;

// Initialize stopwatch
void stopwatch_init(void)
{
    stopwatch_time.hours = 0;
    stopwatch_time.minutes = 0;
    stopwatch_time.seconds = 0;
    stopwatch_state = STOPWATCH_STOPPED;
}

// Start stopwatch
void stopwatch_start(void)
{
    if (stopwatch_state == STOPWATCH_STOPPED) {
        stopwatch_state = STOPWATCH_RUNNING;
    }
}

// Stop stopwatch
void stopwatch_stop(void)
{
    if (stopwatch_state == STOPWATCH_RUNNING) {
        stopwatch_state = STOPWATCH_STOPPED;
    }
}

// Reset stopwatch
void stopwatch_reset(void)
{
    stopwatch_time.hours = 0;
    stopwatch_time.minutes = 0;
    stopwatch_time.seconds = 0;
    stopwatch_state = STOPWATCH_STOPPED;
}

// Update stopwatch (called every second)
void stopwatch_update(void)
{
    if (stopwatch_state == STOPWATCH_RUNNING) {
        stopwatch_increment();
    }
}

// Check if stopwatch is running
bool stopwatch_is_running(void)
{
    return (stopwatch_state == STOPWATCH_RUNNING);
}

// Get current stopwatch time
stopwatch_time_t stopwatch_get_time(void)
{
    return stopwatch_time;
}

// Display stopwatch time on LCD
void stopwatch_display(void)
{
    char time_str[16];
    stopwatch_format_time(time_str);
    lcd_print(time_str);
}

// Set stopwatch time
void stopwatch_set_time(stopwatch_time_t time)
{
    stopwatch_time = time;
}

// Increment stopwatch time
void stopwatch_increment(void)
{
    stopwatch_time.seconds++;
    
    if (stopwatch_time.seconds >= 60) {
        stopwatch_time.seconds = 0;
        stopwatch_time.minutes++;
        
        if (stopwatch_time.minutes >= 60) {
            stopwatch_time.minutes = 0;
            stopwatch_time.hours++;
            
            // Limit to 99:59:59
            if (stopwatch_time.hours >= 100) {
                stopwatch_time.hours = 99;
                stopwatch_time.minutes = 59;
                stopwatch_time.seconds = 59;
                stopwatch_state = STOPWATCH_STOPPED;
            }
        }
    }
}

// Format stopwatch time to string
void stopwatch_format_time(char* buffer)
{
    sprintf(buffer, "%02d:%02d:%02d", 
            stopwatch_time.hours, 
            stopwatch_time.minutes, 
            stopwatch_time.seconds);
} 