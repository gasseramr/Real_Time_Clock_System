#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lcd.h"
#include "rtc.h"
#include "buttons.h"
#include "stopwatch.h"
#include "countdown.h"
#include "alarm.h"
#include "buzzer.h"
#include "time_utils.h"

/*
 * BUTTON ASSIGNMENTS BY MODE:
 * 
 * CLOCK MODE (M0):
 * - MODE: Switch to next mode
 * - SET: No function
 * - START: No function  
 * - STOP: No function
 * 
 * TIME SET MODE (M1):
 * - MODE: Switch to next mode
 * - SET: Cycle through fields (Hour→Minute→Second→Day→Month→Year)
 * - START: Increment selected field
 * - STOP: Decrement selected field
 * 
 * ALARM SET MODE (M2):
 * - MODE: Switch to next mode
 * - SET: Cycle through fields (Hour→Minute) OR Long press to toggle alarm ON/OFF
 * - START: Increment selected field (auto-enables alarm)
 * - STOP: Decrement selected field (auto-enables alarm)
 * 
 * STOPWATCH MODE (M3):
 * - MODE: Switch to next mode
 * - SET: No function
 * - START: Start/Stop stopwatch
 * - STOP: Reset stopwatch
 * 
 * COUNTDOWN MODE (M4):
 * - MODE: Switch to next mode
 * - SET: Cycle through fields (Minute→Second)
 * - START: Start/Stop countdown
 * - STOP: Increment time (minute or second based on field)
 * 
 * SETUP MODE (M5):
 * - MODE: Switch to next mode
 * - SET: Cycle through fields (Hour→Minute→Second→Day→Month→Year)
 * - START: Increment selected field
 * - STOP: Decrement selected field
 */

// System modes
typedef enum {
    MODE_CLOCK = 0,
    MODE_TIME_SET = 1,
    MODE_ALARM_SET = 2,
    MODE_STOPWATCH = 3,
    MODE_COUNTDOWN = 4,
    MODE_SETUP = 5,      // New setup mode
    MODE_MAX = 6
} system_mode_t;

// Global variables
static system_mode_t current_mode = MODE_CLOCK;
static bool mode_changed = true;
static uint8_t seconds_tick = 0;

// Setup mode variables (shared between setup handler and display)
static time_t setup_time = {12, 0, 0};
static date_t setup_date = {1, 1, 2024};

// Time set mode variables (shared between time set handler and display)
static time_t time_set_time = {12, 0, 0};
static date_t time_set_date = {1, 1, 2024};
static bool time_set_initialized = false;

// Function prototypes
void system_init(void);
void set_initial_time_date(void);
void handle_mode_clock(void);
void handle_mode_time_set(void);
void handle_mode_alarm_set(void);
void handle_mode_stopwatch(void);
void handle_mode_countdown(void);
void handle_mode_setup(void);
void update_display(void);
void check_alarm_trigger(void);
void debug_buttons(void);

int main(void)
{
    // Initialize all modules
    system_init();
    
    // Enable global interrupts
    sei();
    
    // Main program loop
    while(1) {
        // Poll for button inputs
        buttons_read_input();
        
        // Check for mode change
        if (button_is_pressed(BTN_MODE)) {
            current_mode = (current_mode + 1) % MODE_MAX;
            mode_changed = true;
            lcd_clear();
            _delay_ms(200); // Debounce delay
        }
        
        // Handle current mode
        switch(current_mode) {
            case MODE_CLOCK:
                handle_mode_clock();
                break;
            case MODE_TIME_SET:
                handle_mode_time_set();
                break;
            case MODE_ALARM_SET:
                handle_mode_alarm_set();
                break;
            case MODE_STOPWATCH:
                handle_mode_stopwatch();
                break;
            case MODE_COUNTDOWN:
                handle_mode_countdown();
                break;
            case MODE_SETUP:
                handle_mode_setup();
                break;
            default:
                current_mode = MODE_CLOCK;
                break;
        }
        
        // Check for alarm trigger
        check_alarm_trigger();
        
        // Debug buttons (uncomment to test)
        debug_buttons();
        
        // Update display if mode changed or every second
        if (mode_changed || seconds_tick) {
            update_display();
            mode_changed = false;
            seconds_tick = 0;
        }
        
        // Small delay for button polling
        _delay_ms(50);
    }
    
    return 0;
}

void system_init(void)
{
    // Initialize LCD
    lcd_init();
    
    // Initialize RTC
    rtc_init();
    
    // Initialize buttons
    buttons_init();
    
    // Initialize stopwatch
    stopwatch_init();
    
    // Initialize countdown
    countdown_init();
    
    // Initialize alarm
    alarm_init();
    
    // Initialize buzzer
    buzzer_init();
    
    // Set initial time and date (uncomment and modify as needed)
    // set_initial_time_date();
    
    // Initialize Timer1 for 1-second ticks
    // Set Timer1 to CTC mode with prescaler 1024
    // For 8MHz: 8000000/1024 = 7812.5 Hz
    // To get 1 second: 7812.5/7812 = 1 Hz
    TCCR1A = 0x00;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, prescaler 1024
    OCR1A = 7811; // Compare value for 1 second
    TIMSK = (1 << OCIE1A); // Enable Timer1 compare interrupt
    
    // Display welcome message
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("RTC System v1.0");
    lcd_goto(1, 0);
    lcd_print("Initializing...");
    _delay_ms(2000);
    lcd_clear();
}

// Function to set initial time and date (uncomment and modify as needed)
void set_initial_time_date(void)
{
    // ===== SET YOUR DESIRED TIME HERE =====
    time_t initial_time = {
        .hour = 12,    // Set hour (0-23)
        .minute = 30,  // Set minute (0-59)
        .second = 0    // Set second (0-59)
    };
    
    // ===== SET YOUR DESIRED DATE HERE =====
    date_t initial_date = {
        .day = 15,     // Set day (1-31)
        .month = 1,    // Set month (1-12)
        .year = 2024   // Set year (2000-2099)
    };
    
    // Set the time and date in RTC
    rtc_set_time(&initial_time);
    rtc_set_date(&initial_date);
}

void handle_mode_clock(void)
{
    // Clock mode - just display current time
    // No special handling needed, display is updated in update_display()
}

void handle_mode_time_set(void)
{
    static uint8_t set_field = 0; // 0=hour, 1=minute, 2=second, 3=day, 4=month, 5=year
    
    // Initialize with current RTC values if not done yet
    if (!time_set_initialized) {
        rtc_get_time(&time_set_time);
        rtc_get_date(&time_set_date);
        time_set_initialized = true;
    }
    
    // Handle SET button to cycle through fields
    if (button_is_pressed(BTN_SET)) {
        set_field = (set_field + 1) % 6;
        _delay_ms(200);
    }
    
    // Handle START button for increment (since we don't have INC button)
    if (button_is_pressed(BTN_START)) {
        switch(set_field) {
            case 0: // Hour
                time_set_time.hour = increment_hour(time_set_time.hour);
                break;
            case 1: // Minute
                time_set_time.minute = increment_minute(time_set_time.minute);
                break;
            case 2: // Second
                time_set_time.second = increment_second(time_set_time.second);
                break;
            case 3: // Day
                time_set_date.day = increment_day(time_set_date.day, time_set_date.month, time_set_date.year);
                break;
            case 4: // Month
                time_set_date.month = increment_month(time_set_date.month);
                break;
            case 5: // Year
                time_set_date.year = increment_year(time_set_date.year);
                break;
        }
        _delay_ms(200);
    }
    
    // Handle STOP button for decrement (since we don't have DEC button)
    if (button_is_pressed(BTN_STOP)) {
        switch(set_field) {
            case 0: // Hour
                time_set_time.hour = decrement_hour(time_set_time.hour);
                break;
            case 1: // Minute
                time_set_time.minute = decrement_minute(time_set_time.minute);
                break;
            case 2: // Second
                time_set_time.second = decrement_second(time_set_time.second);
                break;
            case 3: // Day
                time_set_date.day = decrement_day(time_set_date.day, time_set_date.month, time_set_date.year);
                break;
            case 4: // Month
                time_set_date.month = decrement_month(time_set_date.month);
                break;
            case 5: // Year
                time_set_date.year = decrement_year(time_set_date.year);
                break;
        }
        _delay_ms(200);
    }
    
    // Update RTC if any changes were made
    rtc_set_time(&time_set_time);
    rtc_set_date(&time_set_date);
}

void handle_mode_alarm_set(void)
{
    static uint8_t alarm_field = 0; // 0=hour, 1=minute
    static time_t alarm_time = {6, 30, 0}; // Default 06:30
    static bool alarm_enabled = false;
    static uint8_t set_press_count = 0;
    
    // Handle SET button to cycle through fields or toggle alarm
    if (button_is_pressed(BTN_SET)) {
        set_press_count++;
        if (set_press_count == 1) {
            // Short press - cycle through fields
            alarm_field = (alarm_field + 1) % 2;
        } else if (set_press_count >= 3) {
            // Long press - toggle alarm on/off
            if (alarm_is_enabled()) {
                alarm_disable();
                alarm_enabled = false;
            } else {
                alarm_enable();
                alarm_enabled = true;
            }
            set_press_count = 0;
        }
        _delay_ms(200);
    } else {
        set_press_count = 0;
    }
    
    // Handle START button for increment
    if (button_is_pressed(BTN_START)) {
        if (alarm_field == 0) {
            alarm_time.hour = increment_hour(alarm_time.hour);
        } else {
            alarm_time.minute = increment_minute(alarm_time.minute);
        }
        alarm_set(alarm_time.hour, alarm_time.minute);
        alarm_enable(); // Automatically enable alarm when time is set
        alarm_enabled = true;
        _delay_ms(200);
    }
    
    // Handle STOP button for decrement
    if (button_is_pressed(BTN_STOP)) {
        if (alarm_field == 0) {
            alarm_time.hour = decrement_hour(alarm_time.hour);
        } else {
            alarm_time.minute = decrement_minute(alarm_time.minute);
        }
        alarm_set(alarm_time.hour, alarm_time.minute);
        alarm_enable(); // Automatically enable alarm when time is set
        alarm_enabled = true;
        _delay_ms(200);
    }
}

void handle_mode_stopwatch(void)
{
    // Handle START button for start/stop
    if (button_is_pressed(BTN_START)) {
        if (stopwatch_is_running()) {
            stopwatch_stop();
        } else {
            stopwatch_start();
        }
        _delay_ms(200);
    }
    
    // Handle STOP button for reset
    if (button_is_pressed(BTN_STOP)) {
        stopwatch_reset();
        _delay_ms(200);
    }
    
    // Update stopwatch every second
    if (seconds_tick) {
        stopwatch_update();
    }
}

void handle_mode_countdown(void)
{
    static uint8_t countdown_field = 0; // 0=minute, 1=second
    static uint16_t countdown_time = 120; // Default 2 minutes
    
    // Handle SET button to cycle through fields
    if (button_is_pressed(BTN_SET)) {
        countdown_field = (countdown_field + 1) % 2;
        _delay_ms(200);
    }
    
    // Handle START button for start/stop
    if (button_is_pressed(BTN_START)) {
        if (countdown_is_running()) {
            countdown_stop();
        } else {
            countdown_start();
        }
        _delay_ms(200);
    }
    
    // Handle STOP button for increment/decrement based on field
    if (button_is_pressed(BTN_STOP)) {
        if (countdown_field == 0) {
            // Minute field - increment
            countdown_time += 60;
        } else {
            // Second field - increment
            countdown_time += 1;
        }
        countdown_set(countdown_time);
        _delay_ms(200);
    }
    
    // Update countdown every second
    if (seconds_tick) {
        countdown_update();
    }
}

void handle_mode_setup(void)
{
    static uint8_t setup_field = 0; // 0=hour, 1=minute, 2=second, 3=day, 4=month, 5=year
    
    // Handle SET button to cycle through fields
    if (button_is_pressed(BTN_SET)) {
        setup_field = (setup_field + 1) % 6;
        _delay_ms(200);
    }
    
    // Handle START button for increment
    if (button_is_pressed(BTN_START)) {
        switch(setup_field) {
            case 0: // Hour
                setup_time.hour = increment_hour(setup_time.hour);
                break;
            case 1: // Minute
                setup_time.minute = increment_minute(setup_time.minute);
                break;
            case 2: // Second
                setup_time.second = increment_second(setup_time.second);
                break;
            case 3: // Day
                setup_date.day = increment_day(setup_date.day, setup_date.month, setup_date.year);
                break;
            case 4: // Month
                setup_date.month = increment_month(setup_date.month);
                break;
            case 5: // Year
                setup_date.year = increment_year(setup_date.year);
                break;
        }
        _delay_ms(200);
    }
    
    // Handle STOP button for decrement
    if (button_is_pressed(BTN_STOP)) {
        switch(setup_field) {
            case 0: // Hour
                setup_time.hour = decrement_hour(setup_time.hour);
                break;
            case 1: // Minute
                setup_time.minute = decrement_minute(setup_time.minute);
                break;
            case 2: // Second
                setup_time.second = decrement_second(setup_time.second);
                break;
            case 3: // Day
                setup_date.day = decrement_day(setup_date.day, setup_date.month, setup_date.year);
                break;
            case 4: // Month
                setup_date.month = decrement_month(setup_date.month);
                break;
            case 5: // Year
                setup_date.year = decrement_year(setup_date.year);
                break;
        }
        _delay_ms(200);
    }
    
    // Update RTC with setup values
    rtc_set_time(&setup_time);
    rtc_set_date(&setup_date);
}

void update_display(void)
{
    time_t current_time;
    date_t current_date;
    char time_str[16];
    char date_str[16];
    char date_short[16];
    char date_full[16];
    
    // Get current time and date from RTC
    rtc_get_time(&current_time);
    rtc_get_date(&current_date);
    
    // Display based on current mode
    switch(current_mode) {
        case MODE_CLOCK:
            // Use RTC values
            format_time_to_string(&current_time, time_str);
            format_date_to_string(&current_date, date_str);
            format_date_short(&current_date, date_short);
            
            lcd_goto(0, 0);
            lcd_print("Clock Mode");
            lcd_goto(0, 11);
            lcd_print("M0");
            lcd_goto(1, 0);
            lcd_print(time_str);
            lcd_goto(1, 9);
            lcd_print(date_short);
            break;
            
        case MODE_TIME_SET:
            // Use the values being modified in handle_mode_time_set
            format_time_to_string(&time_set_time, time_str);
            format_date_to_string(&time_set_date, date_str);
            format_date_short(&time_set_date, date_short);
            
            lcd_goto(0, 0);
            lcd_print("Set Time");
            lcd_goto(0, 11);
            lcd_print("M1");
            lcd_goto(1, 0);
            lcd_print(time_str);
            lcd_goto(1, 9);
            lcd_print(date_short);
            break;
            
        case MODE_ALARM_SET:
            lcd_goto(0, 0);
            lcd_print("Set Alarm");
            lcd_goto(0, 11);
            lcd_print("M2");
            lcd_goto(1, 0);
            lcd_print("Alarm: ");
            alarm_display();
            // Show alarm status
            if (alarm_is_enabled()) {
                lcd_goto(1, 14);
                lcd_print("ON");
            } else {
                lcd_goto(1, 14);
                lcd_print("OFF");
            }
            break;
            
        case MODE_STOPWATCH:
            lcd_goto(0, 0);
            lcd_print("Stopwatch");
            lcd_goto(0, 11);
            lcd_print("M3");
            lcd_goto(1, 0);
            lcd_print("Time: ");
            stopwatch_display();
            break;
            
        case MODE_COUNTDOWN:
            lcd_goto(0, 0);
            lcd_print("Countdown");
            lcd_goto(0, 11);
            lcd_print("M4");
            lcd_goto(1, 0);
            lcd_print("Time: ");
            countdown_display();
            break;
            
        case MODE_SETUP:
            // Use static setup values from handle_mode_setup
            format_time_to_string(&setup_time, time_str);
            format_date_to_string(&setup_date, date_str);
            format_date_short(&setup_date, date_short);
            format_date_compact(&setup_date, date_full);
            
            lcd_goto(0, 0);
            lcd_print("Setup Mode");
            lcd_goto(0, 11);
            lcd_print("M5");
            lcd_goto(1, 0);
            lcd_print(time_str);
            // Show date in format DD/MM YYYY to fit better
            char date_part[8];
            sprintf(date_part, "%02d/%02d", setup_date.day, setup_date.month);
            lcd_goto(1, 9);
            lcd_print(date_part);
            lcd_goto(1, 15);
            char year_str[5];
            sprintf(year_str, "%d", setup_date.year);
            lcd_print(year_str);
            break;
            
        default:
            // MODE_MAX case - should not happen in normal operation
            lcd_goto(0, 0);
            lcd_print("Error Mode");
            lcd_goto(1, 0);
            lcd_print("Invalid Mode");
            break;
    }
}

void check_alarm_trigger(void)
{
    if (alarm_check_trigger()) {
        buzzer_on();
        _delay_ms(1000);
        buzzer_off();
        alarm_stop();
    }
}

// Timer1 Compare Match ISR - called every second
ISR(TIMER1_COMPA_vect)
{
    seconds_tick = 1;
} 

void debug_buttons(void)
{
    uint8_t pressed_button = get_pressed_button();
    
    switch(pressed_button) {
        case BTN_MODE:
            lcd_clear();
            lcd_print("MODE Pressed");
            _delay_ms(1000);
            break;
        case BTN_SET:
            lcd_clear();
            lcd_print("SET Pressed");
            _delay_ms(1000);
            break;
        case BTN_START:
            lcd_clear();
            lcd_print("START Pressed");
            _delay_ms(1000);
            break;
        case BTN_STOP:
            lcd_clear();
            lcd_print("STOP Pressed");
            _delay_ms(1000);
            break;
    }
}

// Debug function to test year display
void debug_year_display(void)
{
    date_t test_date = {15, 1, 2024};
    char date_full[16];
    char date_short[16];
    
    format_date_to_string(&test_date, date_full);
    format_date_short(&test_date, date_short);
    
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Full: ");
    lcd_print(date_full);
    lcd_goto(1, 0);
    lcd_print("Short: ");
    lcd_print(date_short);
    _delay_ms(3000);
    
    // Also test current setup values
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Setup Year: ");
    char year_str[8];
    sprintf(year_str, "%d", setup_date.year);
    lcd_print(year_str);
    lcd_goto(1, 0);
    lcd_print("Short: ");
    format_date_short(&setup_date, date_short);
    lcd_print(date_short);
    _delay_ms(3000);
} 

// Debug function to test alarm functionality
void debug_alarm_test(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Alarm Test");
    lcd_goto(1, 0);
    lcd_print("Set to 1 min ahead");
    _delay_ms(2000);
    
    // Get current time
    time_t current_time;
    rtc_get_time(&current_time);
    
    // Set alarm to 1 minute from now
    uint8_t alarm_hour = current_time.hour;
    uint8_t alarm_minute = current_time.minute + 1;
    if (alarm_minute >= 60) {
        alarm_minute = 0;
        alarm_hour = increment_hour(alarm_hour);
    }
    
    alarm_set(alarm_hour, alarm_minute);
    alarm_enable();
    
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Alarm set for:");
    lcd_goto(1, 0);
    char time_str[16];
    sprintf(time_str, "%02d:%02d", alarm_hour, alarm_minute);
    lcd_print(time_str);
    _delay_ms(3000);
} 

 