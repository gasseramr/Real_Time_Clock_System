#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"

// I2C pin definitions (for ATmega32)
#define I2C_SDA_PIN        PC1
#define I2C_SCL_PIN        PC0

// I2C functions
void rtc_i2c_start(void)
{
    // Set SDA and SCL high
    PORTC |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);
    DDRC |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);
    
    // Start condition: SDA goes low while SCL is high
    PORTC &= ~(1 << I2C_SDA_PIN);
    _delay_us(5);
    PORTC &= ~(1 << I2C_SCL_PIN);
}

void rtc_i2c_stop(void)
{
    // Set SDA low
    PORTC &= ~(1 << I2C_SDA_PIN);
    DDRC |= (1 << I2C_SDA_PIN);
    
    // Stop condition: SDA goes high while SCL is high
    PORTC |= (1 << I2C_SCL_PIN);
    _delay_us(5);
    PORTC |= (1 << I2C_SDA_PIN);
    _delay_us(5);
}

void rtc_i2c_write(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++) {
        // Set SCL low
        PORTC &= ~(1 << I2C_SCL_PIN);
        
        // Set SDA according to bit
        if (data & 0x80) {
            PORTC |= (1 << I2C_SDA_PIN);
        } else {
            PORTC &= ~(1 << I2C_SDA_PIN);
        }
        
        // Set SCL high
        PORTC |= (1 << I2C_SCL_PIN);
        _delay_us(5);
        
        // Set SCL low
        PORTC &= ~(1 << I2C_SCL_PIN);
        
        data <<= 1;
    }
    
    // Read ACK
    PORTC &= ~(1 << I2C_SCL_PIN);
    DDRC &= ~(1 << I2C_SDA_PIN); // Set SDA as input
    PORTC |= (1 << I2C_SCL_PIN);
    _delay_us(5);
    PORTC &= ~(1 << I2C_SCL_PIN);
    DDRC |= (1 << I2C_SDA_PIN); // Set SDA as output
}

uint8_t rtc_i2c_read(bool ack)
{
    uint8_t data = 0;
    uint8_t i;
    
    DDRC &= ~(1 << I2C_SDA_PIN); // Set SDA as input
    
    for (i = 0; i < 8; i++) {
        // Set SCL low
        PORTC &= ~(1 << I2C_SCL_PIN);
        _delay_us(5);
        
        // Set SCL high
        PORTC |= (1 << I2C_SCL_PIN);
        _delay_us(5);
        
        // Read bit
        data <<= 1;
        if (PINC & (1 << I2C_SDA_PIN)) {
            data |= 1;
        }
        
        // Set SCL low
        PORTC &= ~(1 << I2C_SCL_PIN);
    }
    
    // Send ACK/NACK
    DDRC |= (1 << I2C_SDA_PIN); // Set SDA as output
    if (ack) {
        PORTC &= ~(1 << I2C_SDA_PIN); // ACK
    } else {
        PORTC |= (1 << I2C_SDA_PIN); // NACK
    }
    
    PORTC |= (1 << I2C_SCL_PIN);
    _delay_us(5);
    PORTC &= ~(1 << I2C_SCL_PIN);
    
    return data;
}

// RTC initialization
void rtc_init(void)
{
    // Configure I2C pins
    DDRC |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);
    PORTC |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);
    
    _delay_ms(100); // Wait for RTC to stabilize
}

// Write to RTC register
void rtc_write_register(uint8_t reg, uint8_t data)
{
    rtc_i2c_start();
    rtc_i2c_write(RTC_I2C_ADDRESS << 1); // Write address
    rtc_i2c_write(reg); // Register address
    rtc_i2c_write(data); // Data
    rtc_i2c_stop();
}

// Read from RTC register
uint8_t rtc_read_register(uint8_t reg)
{
    uint8_t data;
    
    rtc_i2c_start();
    rtc_i2c_write(RTC_I2C_ADDRESS << 1); // Write address
    rtc_i2c_write(reg); // Register address
    rtc_i2c_start();
    rtc_i2c_write((RTC_I2C_ADDRESS << 1) | 1); // Read address
    data = rtc_i2c_read(false); // Read data, no ACK
    rtc_i2c_stop();
    
    return data;
}

// Get current time from RTC
void rtc_get_time(time_t* time)
{
    time->second = bcd_to_bin(rtc_read_register(RTC_SECONDS) & 0x7F);
    time->minute = bcd_to_bin(rtc_read_register(RTC_MINUTES) & 0x7F);
    time->hour = bcd_to_bin(rtc_read_register(RTC_HOURS) & 0x3F);
}

// Set time in RTC
void rtc_set_time(time_t* time)
{
    rtc_write_register(RTC_SECONDS, bin_to_bcd(time->second));
    rtc_write_register(RTC_MINUTES, bin_to_bcd(time->minute));
    rtc_write_register(RTC_HOURS, bin_to_bcd(time->hour));
}

// Get current date from RTC
void rtc_get_date(date_t* date)
{
    date->day = bcd_to_bin(rtc_read_register(RTC_DATE) & 0x3F);
    date->month = bcd_to_bin(rtc_read_register(RTC_MONTH) & 0x1F);
    date->year = 2000 + bcd_to_bin(rtc_read_register(RTC_YEAR));
}

// Set date in RTC
void rtc_set_date(date_t* date)
{
    rtc_write_register(RTC_DATE, bin_to_bcd(date->day));
    rtc_write_register(RTC_MONTH, bin_to_bcd(date->month));
    rtc_write_register(RTC_YEAR, bin_to_bcd(date->year - 2000));
}

// Validate time
bool rtc_is_valid_time(time_t* time)
{
    return (time->hour < 24 && time->minute < 60 && time->second < 60);
}

// Validate date
bool rtc_is_valid_date(date_t* date)
{
    return (date->day >= 1 && date->day <= 31 &&
            date->month >= 1 && date->month <= 12 &&
            date->year >= 2000 && date->year <= 2099);
}

// BCD to binary conversion
uint8_t bcd_to_bin(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Binary to BCD conversion
uint8_t bin_to_bcd(uint8_t bin)
{
    return ((bin / 10) << 4) | (bin % 10);
} 