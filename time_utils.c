#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "time_utils.h"
#include "rtc.h"

// Time increment functions
uint8_t increment_hour(uint8_t hour)
{
    hour++;
    if (hour >= 24) hour = 0;
    return hour;
}

uint8_t decrement_hour(uint8_t hour)
{
    if (hour == 0) hour = 23;
    else hour--;
    return hour;
}

uint8_t increment_minute(uint8_t minute)
{
    minute++;
    if (minute >= 60) minute = 0;
    return minute;
}

uint8_t decrement_minute(uint8_t minute)
{
    if (minute == 0) minute = 59;
    else minute--;
    return minute;
}

uint8_t increment_second(uint8_t second)
{
    second++;
    if (second >= 60) second = 0;
    return second;
}

uint8_t decrement_second(uint8_t second)
{
    if (second == 0) second = 59;
    else second--;
    return second;
}

uint8_t increment_day(uint8_t day, uint8_t month, uint16_t year)
{
    uint8_t max_days = days_in_month(month, year);
    day++;
    if (day > max_days) day = 1;
    return day;
}

uint8_t decrement_day(uint8_t day, uint8_t month, uint16_t year)
{
    if (day == 1) {
        day = days_in_month(month, year);
    } else {
        day--;
    }
    return day;
}

uint8_t increment_month(uint8_t month)
{
    month++;
    if (month > 12) month = 1;
    return month;
}

uint8_t decrement_month(uint8_t month)
{
    if (month == 1) month = 12;
    else month--;
    return month;
}

uint16_t increment_year(uint16_t year)
{
    year++;
    if (year > 2099) year = 2000;
    return year;
}

uint16_t decrement_year(uint16_t year)
{
    if (year == 2000) year = 2099;
    else year--;
    return year;
}

// Time formatting functions
void format_time_to_string(time_t* time, char* buffer)
{
    sprintf(buffer, "%02d:%02d:%02d", time->hour, time->minute, time->second);
}

void format_date_to_string(date_t* date, char* buffer)
{
    sprintf(buffer, "%02d/%02d/%04d", date->day, date->month, date->year);
}

// Shorter date format for LCD display (DD/MM/YY)
void format_date_short(date_t* date, char* buffer)
{
    sprintf(buffer, "%02d/%02d/%02d", date->day, date->month, date->year % 100);
}

// Compact date format for LCD display (DD/MM/YYYY)
void format_date_compact(date_t* date, char* buffer)
{
    sprintf(buffer, "%02d/%02d/%04d", date->day, date->month, date->year);
}

// Year-only format for LCD display
void format_year_only(date_t* date, char* buffer)
{
    sprintf(buffer, "%d", date->year);
}

void format_stopwatch_to_string(stopwatch_time_t* time, char* buffer)
{
    sprintf(buffer, "%02d:%02d:%02d", time->hours, time->minutes, time->seconds);
}

void format_countdown_to_string(uint16_t seconds, char* buffer)
{
    uint8_t minutes = seconds / 60;
    uint8_t secs = seconds % 60;
    sprintf(buffer, "%02d:%02d", minutes, secs);
}

// Time validation functions
bool is_valid_hour(uint8_t hour)
{
    return (hour < 24);
}

bool is_valid_minute(uint8_t minute)
{
    return (minute < 60);
}

bool is_valid_second(uint8_t second)
{
    return (second < 60);
}

bool is_valid_day(uint8_t day, uint8_t month, uint16_t year)
{
    return (day >= 1 && day <= days_in_month(month, year));
}

bool is_valid_month(uint8_t month)
{
    return (month >= 1 && month <= 12);
}

bool is_valid_year(uint16_t year)
{
    return (year >= 2000 && year <= 2099);
}

bool is_leap_year(uint16_t year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

uint8_t days_in_month(uint8_t month, uint16_t year)
{
    const uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) return 31;
    
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    
    return days[month - 1];
}

// Time conversion functions
uint16_t time_to_seconds(uint8_t hour, uint8_t minute, uint8_t second)
{
    return (uint16_t)hour * 3600 + (uint16_t)minute * 60 + second;
}

void seconds_to_time(uint16_t total_seconds, uint8_t* hour, uint8_t* minute, uint8_t* second)
{
    *hour = total_seconds / 3600;
    *minute = (total_seconds % 3600) / 60;
    *second = total_seconds % 60;
} 