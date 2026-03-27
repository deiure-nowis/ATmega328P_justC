# Konfigurace projektu
MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyUSB1
BAUD = 115200
PROGRAMMER = arduino

# Složky projektu
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Nástroje
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude  # Odstraněno sudo!

# Kompilátor
CFLAGS  = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -I$(INC_DIR)
LDFLAGS =

# Soubory
TARGET = $(BIN_DIR)/main
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Vygeneruje seznam .o souborů ve složce build/ na základě .c souborů v src/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Pravidla

all: directories $(TARGET).hex

# Pravidlo pro vytvoření složek, pokud neexistují
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Kompilace jednotlivých .c souborů do .o souborů
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).hex
	$(AVRDUDE) -F -V -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all flash clean directories
