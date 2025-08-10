#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <stdint.h>
#include <stdbool.h>

// Stopwatch time structure
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} stopwatch_time_t;

// Stopwatch states
typedef enum {
    STOPWATCH_STOPPED = 0,
    STOPWATCH_RUNNING,
    STOPWATCH_PAUSED
} stopwatch_state_t;

// Function prototypes
void stopwatch_init(void);
void stopwatch_start(void);
void stopwatch_stop(void);
void stopwatch_reset(void);
void stopwatch_update(void);
bool stopwatch_is_running(void);
stopwatch_time_t stopwatch_get_time(void);
void stopwatch_display(void);
void stopwatch_set_time(stopwatch_time_t time);

// Internal functions
void stopwatch_increment(void);
void stopwatch_format_time(char* buffer);

#endif // STOPWATCH_H 