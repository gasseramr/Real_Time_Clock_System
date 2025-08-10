#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "alarm.h"
#include "lcd.h"
#include "rtc.h"

// Alarm variables
static alarm_t alarm_time = {6, 30, false};
static bool alarm_triggered = false;

// Initialize alarm
void alarm_init(void)
{
    alarm_time.hour = 6;
    alarm_time.minute = 30;
    alarm_time.enabled = false;
    alarm_triggered = false;
}

// Set alarm time
void alarm_set(uint8_t hour, uint8_t minute)
{
    alarm_time.hour = hour;
    alarm_time.minute = minute;
}

// Enable alarm
void alarm_enable(void)
{
    alarm_time.enabled = true;
    alarm_triggered = false;
}

// Disable alarm
void alarm_disable(void)
{
    alarm_time.enabled = false;
    alarm_triggered = false;
}

// Stop alarm
void alarm_stop(void)
{
    alarm_triggered = false;
}

// Check if alarm should trigger
bool alarm_check_trigger(void)
{
    if (!alarm_time.enabled || alarm_triggered) {
        return false;
    }
    
    time_t current_time;
    rtc_get_time(&current_time);
    
    if (alarm_time_matches(current_time.hour, current_time.minute)) {
        alarm_triggered = true;
        return true;
    }
    
    return false;
}

// Check if alarm is enabled
bool alarm_is_enabled(void)
{
    return alarm_time.enabled;
}

// Get alarm time
alarm_t alarm_get_time(void)
{
    return alarm_time;
}

// Display alarm time on LCD
void alarm_display(void)
{
    char time_str[16];
    alarm_format_time(time_str);
    lcd_print(time_str);
}

// Check if current time matches alarm time
bool alarm_time_matches(uint8_t current_hour, uint8_t current_minute)
{
    return (current_hour == alarm_time.hour && current_minute == alarm_time.minute);
}

// Format alarm time to string
void alarm_format_time(char* buffer)
{
    sprintf(buffer, "%02d:%02d", alarm_time.hour, alarm_time.minute);
} 