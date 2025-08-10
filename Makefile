# Makefile for Advanced Real-Time Clock System
# ATmega32 @ 8MHz

# Target microcontroller
MCU = atmega32
F_CPU = 8000000

# Programmer settings
PROGRAMMER = usbasp
PORT = COM3

# Compiler and flags
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude

# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
LDFLAGS = -mmcu=$(MCU)

# Source files
SOURCES = main.c lcd.c rtc.c buttons.c stopwatch.c countdown.c alarm.c buzzer.c time_utils.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = rtc_system

# Default target
all: $(TARGET).hex

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

# Create hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flash to microcontroller
flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$<:i

# Clean build files
clean:
	rm -f *.o *.elf *.hex

# Show size information
size: $(TARGET).elf
	$(SIZE) $<

# Help
help:
	@echo "Available targets:"
	@echo "  all     - Build the project (default)"
	@echo "  flash   - Flash to microcontroller"
	@echo "  clean   - Remove build files"
	@echo "  size    - Show size information"
	@echo "  help    - Show this help"

.PHONY: all flash clean size help 