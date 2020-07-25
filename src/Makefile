#==============================================================================

OUTPUT = avrmon

#==============================================================================

MCU = atmega328p
F_CPU = 16000000UL

#==============================================================================

C_SRC = main.c \
        uart.c \

OBJ = $(C_SRC:%.c=%.o) $(A_SRC:%.S=%.o) 

#==============================================================================
# tools

X_CC = avr-gcc
X_LD = avr-ld
X_OBJCOPY = avr-objcopy
X_OBJDUMP = avr-objdump

#==============================================================================

CFLAGS += -mmcu=$(MCU)
CFLAGS += -DF_CPU=$(F_CPU)

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wstrict-prototypes
CFLAGS += -Os

%.o : %.c
	$(X_CC) $(CFLAGS) -c $< -o $@ 

#==============================================================================
# avrdude programmer

AVRDUDE = /usr/bin/avrdude
AVRDUDE_PROGRAMMER = usbtiny
AVRDUDE_PORT = usb

#==============================================================================

.PHONY: all
all: $(OUTPUT).hex

.PHONY: clean
clean:
	-rm -f $(OBJ)
	-rm -f $(OUTPUT).elf
	-rm -f $(OUTPUT).hex

.PHONY: format
format:
	uncrustify -c ../fmt.cfg --replace --no-backup *.h *.c

.PHONY: program
program:
	$(AVRDUDE) -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -v -B 1 -U flash:w:$(OUTPUT).hex

$(OUTPUT).elf: $(OBJ)
	$(X_CC) $(CFLAGS) $(OBJ) --output $@

$(OUTPUT).hex: $(OUTPUT).elf
	$(X_OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

#==============================================================================
