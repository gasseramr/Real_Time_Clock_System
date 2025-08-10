#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd.h"

// LCD initialization
void lcd_init(void)
{
    // Configure LCD pins as outputs
    DDRD |= (1 << LCD_RS_PIN) | (1 << LCD_RW_PIN) | (1 << LCD_EN_PIN) |
             (1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN);
    
    // Wait for power-up
    _delay_ms(50);
    
    // Initialize LCD in 4-bit mode
    lcd_write_nibble(0x03);
    _delay_ms(5);
    lcd_write_nibble(0x03);
    _delay_ms(5);
    lcd_write_nibble(0x03);
    _delay_ms(5);
    lcd_write_nibble(0x02);
    _delay_ms(5);
    
    // Function set: 4-bit mode, 2 lines, 5x8 font
    lcd_write_command(LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE | LCD_5x8DOTS);
    _delay_ms(5);
    
    // Display control: display on, cursor off, blink off
    lcd_write_command(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
    _delay_ms(5);
    
    // Entry mode set: increment cursor, no display shift
    lcd_write_command(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DEC);
    _delay_ms(5);
    
    // Clear display
    lcd_clear();
    _delay_ms(5);
}

// Clear LCD display
void lcd_clear(void)
{
    lcd_write_command(LCD_CLEAR_DISPLAY);
    _delay_ms(2);
}

// Move cursor to specified position
void lcd_goto(uint8_t row, uint8_t col)
{
    uint8_t address;
    
    if (row == 0) {
        address = 0x00 + col;
    } else {
        address = 0x40 + col;
    }
    
    lcd_write_command(LCD_SET_DDRAM_ADDR | address);
}

// Print string to LCD
void lcd_print(const char* str)
{
    while (*str != '\0') {
        lcd_print_char(*str);
        str++;
    }
}

// Print single character to LCD
void lcd_print_char(char c)
{
    lcd_write_data(c);
}

// Display time in HH:MM:SS format
void lcd_display_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    char time_str[16];
    sprintf(time_str, "%02d:%02d:%02d", hour, minute, second);
    lcd_print(time_str);
}

// Display date in DD/MM/YYYY format
void lcd_display_date(uint8_t day, uint8_t month, uint16_t year)
{
    char date_str[16];
    sprintf(date_str, "%02d/%02d/%04d", day, month, year);
    lcd_print(date_str);
}

// Display mode name
void lcd_display_mode_name(const char* mode_name)
{
    lcd_print(mode_name);
}

// Write command to LCD
void lcd_write_command(uint8_t cmd)
{
    // Set RS low for command
    PORTD &= ~(1 << LCD_RS_PIN);
    
    // Write high nibble
    lcd_write_nibble(cmd >> 4);
    
    // Write low nibble
    lcd_write_nibble(cmd & 0x0F);
}

// Write data to LCD
void lcd_write_data(uint8_t data)
{
    // Set RS high for data
    PORTD |= (1 << LCD_RS_PIN);
    
    // Write high nibble
    lcd_write_nibble(data >> 4);
    
    // Write low nibble
    lcd_write_nibble(data & 0x0F);
}

// Write 4-bit nibble to LCD
void lcd_write_nibble(uint8_t nibble)
{
    // Clear data pins
    PORTD &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
    
    // Set data pins according to nibble
    if (nibble & 0x01) PORTD |= (1 << LCD_D4_PIN);
    if (nibble & 0x02) PORTD |= (1 << LCD_D5_PIN);
    if (nibble & 0x04) PORTD |= (1 << LCD_D6_PIN);
    if (nibble & 0x08) PORTD |= (1 << LCD_D7_PIN);
    
    // Pulse enable
    lcd_pulse_enable();
}

// Pulse enable pin
void lcd_pulse_enable(void)
{
    PORTD |= (1 << LCD_EN_PIN);
    _delay_us(1);
    PORTD &= ~(1 << LCD_EN_PIN);
    _delay_us(1);
} 