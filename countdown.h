#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <stdint.h>
#include <stdbool.h>

// Countdown states
typedef enum {
    COUNTDOWN_STOPPED = 0,
    COUNTDOWN_RUNNING,
    COUNTDOWN_FINISHED
} countdown_state_t;

// Function prototypes
void countdown_init(void);
void countdown_set(uint16_t seconds);
void countdown_start(void);
void countdown_stop(void);
void countdown_reset(void);
void countdown_update(void);
bool countdown_is_running(void);
bool countdown_is_finished(void);
uint16_t countdown_get_time(void);
void countdown_display(void);

// Internal functions
void countdown_decrement(void);
void countdown_format_time(char* buffer);
uint8_t countdown_get_minutes(void);
uint8_t countdown_get_seconds(void);

#endif // COUNTDOWN_H 