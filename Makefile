OBJS=main.o

TARGET=avr-usb-patlite
MCU=attiny2313

CC=avr-gcc
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy
AR=avr-ar

CFLAGS=-mmcu=$(MCU)

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@  $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS) $(TARGET).hex $(TARGET).elf
