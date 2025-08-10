#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// Buzzer pin definition (built-in buzzer on IMT School Kit)
#define BUZZER_PIN    PA0  // Most common connection, adjust if needed

// Function prototypes
void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_beep(uint16_t duration_ms);
void buzzer_tone(uint16_t frequency, uint16_t duration_ms);

#endif // BUZZER_H 