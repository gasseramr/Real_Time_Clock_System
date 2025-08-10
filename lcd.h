#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD Commands
#define LCD_CLEAR_DISPLAY    0x01
#define LCD_RETURN_HOME      0x02
#define LCD_ENTRY_MODE_SET   0x04
#define LCD_DISPLAY_CONTROL  0x08
#define LCD_CURSOR_SHIFT     0x10
#define LCD_FUNCTION_SET     0x20
#define LCD_SET_CGRAM_ADDR   0x40
#define LCD_SET_DDRAM_ADDR   0x80

// LCD Display Control
#define LCD_DISPLAY_ON       0x04
#define LCD_DISPLAY_OFF      0x00
#define LCD_CURSOR_ON        0x02
#define LCD_CURSOR_OFF       0x00
#define LCD_BLINK_ON         0x01
#define LCD_BLINK_OFF        0x00

// LCD Entry Mode
#define LCD_ENTRY_RIGHT      0x00
#define LCD_ENTRY_LEFT       0x02
#define LCD_ENTRY_SHIFT_INC  0x01
#define LCD_ENTRY_SHIFT_DEC  0x00

// LCD Function Set
#define LCD_4BIT_MODE        0x00
#define LCD_2LINE            0x08
#define LCD_5x8DOTS          0x00

// LCD Pin Definitions (for ATmega32)
#define LCD_RS_PIN           PD0
#define LCD_RW_PIN           PD1
#define LCD_EN_PIN           PD2
#define LCD_D4_PIN           PD4
#define LCD_D5_PIN           PD5
#define LCD_D6_PIN           PD6
#define LCD_D7_PIN           PD7

// Function prototypes
void lcd_init(void);
void lcd_clear(void);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_print(const char* str);
void lcd_print_char(char c);
void lcd_display_time(uint8_t hour, uint8_t minute, uint8_t second);
void lcd_display_date(uint8_t day, uint8_t month, uint16_t year);
void lcd_display_mode_name(const char* mode_name);

// Internal functions
void lcd_write_command(uint8_t cmd);
void lcd_write_data(uint8_t data);
void lcd_write_nibble(uint8_t nibble);
void lcd_pulse_enable(void);

#endif // LCD_H 