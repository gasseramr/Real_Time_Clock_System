# Advanced Real-Time Clock System

## 🕒 Project Overview

A comprehensive real-time embedded system using **ATmega32 microcontroller (8 MHz)** that functions as:

- **Real-time clock (RTC)** and calendar
- **Alarm system** with buzzer notification
- **Stopwatch** with start/stop/reset functionality
- **Countdown timer** with configurable time and buzzer alert

All features are user-controlled via **push buttons** and visually displayed through a **16x2 character LCD**. Hardware modules include an **RTC module (DS1307/DS3231)** and a **buzzer** for alerts.

## 🔧 Hardware Components

- **Microcontroller**: ATmega32 (8 MHz clock)
- **RTC Module**: DS1307 or DS3231 (I2C communication)
- **Character LCD**: 16x2 HD44780-based (parallel interface)
- **Push Buttons**: 6 buttons for various functions
- **Buzzer**: For alarm and countdown notifications
- **Power Source**: Stable 5V (with optional backup battery for RTC)

## 🧠 System Architecture

### Modular Software Design

The system is built using **modular programming principles** with the following modules:

1. **Main Program Loop** (`main.c`) - System initialization and mode management
2. **RTC Interface** (`rtc.c`, `rtc.h`) - DS1307/DS3231 communication via I2C
3. **LCD Display** (`lcd.c`, `lcd.h`) - 16x2 LCD control and display functions
4. **Button Interface** (`buttons.c`, `buttons.h`) - Debounced button input handling
5. **Stopwatch** (`stopwatch.c`, `stopwatch.h`) - Timer functionality with internal timing
6. **Countdown Timer** (`countdown.c`, `countdown.h`) - Down counter with buzzer alert
7. **Alarm System** (`alarm.c`, `alarm.h`) - Time-based alarm with buzzer trigger
8. **Buzzer Control** (`buzzer.c`, `buzzer.h`) - Audio output control
9. **Time Utilities** (`time_utils.c`, `time_utils.h`) - Time conversion and formatting

## 📘 System Modes

The system operates in **5 different modes**:

### Mode 0: Clock Mode
- **Display**: Current time and date
- **Format**: `HH:MM:SS DD/MM/YYYY`
- **Function**: Real-time clock display

### Mode 1: Time Setting Mode
- **Display**: Editable time and date
- **Controls**: SET button cycles through fields (hour, minute, second, day, month, year)
- **Function**: Set RTC time and date

### Mode 2: Alarm Setting Mode
- **Display**: Alarm time setting
- **Controls**: SET button cycles through hour/minute, INC/DEC to adjust
- **Function**: Configure alarm time

### Mode 3: Stopwatch Mode
- **Display**: Running stopwatch time
- **Controls**: START/STOP button to control, RESET to clear
- **Function**: Measure elapsed time

### Mode 4: Countdown Mode
- **Display**: Countdown timer with configurable time
- **Controls**: SET button cycles through minute/second fields, INC/DEC to adjust time
- **Function**: Timer with buzzer alert when finished

## 🎮 Button Guide

| Button | Function |
|--------|----------|
| **MODE** | Cycle through system modes (M0 → M1 → M2 → M3 → M4 → M0) |
| **SET** | In Time Set: Cycle through fields (hour→minute→second→day→month→year)<br>In Alarm Set: Cycle through hour/minute<br>In Countdown: Cycle through minute/second |
| **INC** | Increment current field value |
| **DEC** | Decrement current field value |
| **START/STOP** | In Stopwatch: Start/stop timing<br>In Countdown: Start/stop countdown |
| **RESET** | In Stopwatch: Reset to 00:00:00<br>In Countdown: Reset to set time |

## 🔌 Pin Connections

### LCD (16x2 Character LCD)
- **RS**: PD0
- **RW**: PD1
- **EN**: PD2
- **D4**: PD4
- **D5**: PD5
- **D6**: PD6
- **D7**: PD7

### RTC (DS1307/DS3231)
- **SDA**: PC1
- **SCL**: PC0
- **VCC**: 5V
- **GND**: GND

### Buttons
- **MODE**: PC0
- **INC**: PC1
- **DEC**: PC2
- **SET**: PC3
- **START/STOP**: PC4
- **RESET**: PC5

### Buzzer
- **Signal**: PB0
- **VCC**: 5V
- **GND**: GND

## ⚙️ Technical Specifications

### Timing System
- **Clock Speed**: 8 MHz
- **Timer1**: CTC mode with 1-second interrupt
- **Prescaler**: 1024
- **Compare Value**: 7811 (for 1 Hz interrupt)

### RTC Communication
- **Protocol**: I2C (bit-banged)
- **Address**: 0x68 (DS1307/DS3231)
- **Data Format**: BCD
- **Backup**: Optional battery backup

### Display
- **Type**: 16x2 Character LCD
- **Interface**: 4-bit parallel
- **Controller**: HD44780 compatible
- **Update Rate**: 1 Hz

## 🚀 Building and Flashing

### Prerequisites
- AVR-GCC toolchain
- AVRDUDE
- USBasp programmer (or compatible)

### Build Commands
```bash
# Build the project
make

# Flash to microcontroller
make flash

# Clean build files
make clean

# Show size information
make size
```

### Programmer Settings
- **Programmer**: USBasp
- **Port**: COM3 (adjust in Makefile)
- **Target**: ATmega32

## 📊 Memory Usage

The compiled firmware typically uses:
- **Flash**: ~8-12 KB
- **RAM**: ~200-300 bytes
- **EEPROM**: Not used (can be added for settings storage)

## 🔧 Configuration Options

### Clock Speed
- Default: 8 MHz
- Can be modified in Makefile: `F_CPU = 8000000`

### Timer Settings
- Timer1 prescaler and compare values can be adjusted in `main.c`
- Current settings optimized for 8 MHz clock

### Pin Assignments
- All pin definitions are in respective header files
- Can be modified for different hardware configurations

## 🎯 Features

### ✅ Implemented Features
- [x] Real-time clock display
- [x] Date display and setting
- [x] Alarm system with buzzer
- [x] Stopwatch functionality
- [x] Countdown timer
- [x] Button debouncing
- [x] LCD display management
- [x] I2C RTC communication
- [x] Modular software architecture
- [x] Interrupt-based timing

### 🔮 Future Enhancements
- [ ] EEPROM storage for settings
- [ ] Temperature display (DS3231)
- [ ] Multiple alarm support
- [ ] Automatic brightness adjustment
- [ ] Melody support for buzzer
- [ ] 12/24 hour format toggle
- [ ] Day of week display

## 🐛 Troubleshooting

### Common Issues

1. **LCD not displaying**
   - Check pin connections
   - Verify power supply (5V)
   - Check contrast adjustment

2. **RTC not responding**
   - Verify I2C connections (SDA, SCL)
   - Check RTC power supply
   - Ensure proper pull-up resistors

3. **Buttons not working**
   - Check button connections
   - Verify pull-up resistors
   - Check for short circuits

4. **Buzzer not sounding**
   - Check buzzer connections
   - Verify power supply
   - Test with direct pin control

## 📝 License

This project is open source and available under the MIT License.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

**Developed for ATmega32 @ 8MHz**  
**Advanced Real-Time Clock System v1.0** 