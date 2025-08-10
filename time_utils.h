#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"
#include "stopwatch.h"

// Time increment/decrement functions
uint8_t increment_hour(uint8_t hour);
uint8_t decrement_hour(uint8_t hour);
uint8_t increment_minute(uint8_t minute);
uint8_t decrement_minute(uint8_t minute);
uint8_t increment_second(uint8_t second);
uint8_t decrement_second(uint8_t second);
uint8_t increment_day(uint8_t day, uint8_t month, uint16_t year);
uint8_t decrement_day(uint8_t day, uint8_t month, uint16_t year);
uint8_t increment_month(uint8_t month);
uint8_t decrement_month(uint8_t month);
uint16_t increment_year(uint16_t year);
uint16_t decrement_year(uint16_t year);

// Time formatting functions
void format_time_to_string(time_t* time, char* buffer);
void format_date_to_string(date_t* date, char* buffer);
void format_date_short(date_t* date, char* buffer);
void format_date_compact(date_t* date, char* buffer);
void format_year_only(date_t* date, char* buffer);
void format_stopwatch_to_string(stopwatch_time_t* time, char* buffer);
void format_countdown_to_string(uint16_t seconds, char* buffer);

// Time validation functions
bool is_valid_hour(uint8_t hour);
bool is_valid_minute(uint8_t minute);
bool is_valid_second(uint8_t second);
bool is_valid_day(uint8_t day, uint8_t month, uint16_t year);
bool is_valid_month(uint8_t month);
bool is_valid_year(uint16_t year);
bool is_leap_year(uint16_t year);
uint8_t days_in_month(uint8_t month, uint16_t year);

// Time conversion functions
uint16_t time_to_seconds(uint8_t hour, uint8_t minute, uint8_t second);
void seconds_to_time(uint16_t total_seconds, uint8_t* hour, uint8_t* minute, uint8_t* second);

#endif // TIME_UTILS_H 