#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "buzzer.h"

void buzzer_init(void)
{
    // Configure buzzer pin as output
    DDRA |= (1 << BUZZER_PIN);
    
    // Ensure buzzer is off initially
    buzzer_off();
}

void buzzer_on(void)
{
    // Set buzzer pin high
    PORTA |= (1 << BUZZER_PIN);
}

void buzzer_off(void)
{
    // Set buzzer pin low
    PORTA &= ~(1 << BUZZER_PIN);
}

void buzzer_beep(uint16_t duration_ms)
{
    buzzer_on();
    _delay_ms(duration_ms);
    buzzer_off();
}

void buzzer_tone(uint16_t frequency, uint16_t duration_ms)
{
    uint16_t cycles = (frequency * duration_ms) / 1000;
    
    for (uint16_t i = 0; i < cycles; i++) {
        buzzer_on();
        _delay_us(500000 / frequency);  // Half period
        buzzer_off();
        _delay_us(500000 / frequency);  // Half period
    }
} 