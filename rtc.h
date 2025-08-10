#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <stdbool.h>

// RTC I2C Address (DS1307/DS3231)
#define RTC_I2C_ADDRESS      0x68

// RTC Register Addresses
#define RTC_SECONDS          0x00
#define RTC_MINUTES          0x01
#define RTC_HOURS            0x02
#define RTC_DAY              0x03
#define RTC_DATE             0x04
#define RTC_MONTH            0x05
#define RTC_YEAR             0x06
#define RTC_CONTROL          0x07

// Time structure
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} time_t;

// Date structure
typedef struct {
    uint8_t day;
    uint8_t month;
    uint16_t year;
} date_t;

// Function prototypes
void rtc_init(void);
void rtc_get_time(time_t* time);
void rtc_set_time(time_t* time);
void rtc_get_date(date_t* date);
void rtc_set_date(date_t* date);
bool rtc_is_valid_time(time_t* time);
bool rtc_is_valid_date(date_t* date);

// I2C functions for RTC communication
void rtc_i2c_start(void);
void rtc_i2c_stop(void);
void rtc_i2c_write(uint8_t data);
uint8_t rtc_i2c_read(bool ack);
void rtc_write_register(uint8_t reg, uint8_t data);
uint8_t rtc_read_register(uint8_t reg);

// BCD conversion functions
uint8_t bcd_to_bin(uint8_t bcd);
uint8_t bin_to_bcd(uint8_t bin);

#endif // RTC_H 