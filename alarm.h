#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include <stdbool.h>

// Alarm structure
typedef struct {
    uint8_t hour;
    uint8_t minute;
    bool enabled;
} alarm_t;

// Function prototypes
void alarm_init(void);
void alarm_set(uint8_t hour, uint8_t minute);
void alarm_enable(void);
void alarm_disable(void);
void alarm_stop(void);
bool alarm_check_trigger(void);
bool alarm_is_enabled(void);
alarm_t alarm_get_time(void);
void alarm_display(void);

// Internal functions
bool alarm_time_matches(uint8_t current_hour, uint8_t current_minute);
void alarm_format_time(char* buffer);

#endif // ALARM_H 