PRGRMR = usbasp
TARGET = t861
CFLAGS=-Wall -g -Os -mmcu=attiny861
SRC = RGB_PWM_test

build: $(SRC).c
	avr-gcc $(CFLAGS) -o $(SRC).bin $(SRC).c
	avr-objcopy -j .text -j .data -O ihex $(SRC).bin $(SRC).hex
    
flash: build
	avrdude -c $(PRGRMR) -p $(TARGET) -P /dev/ttyUSB0 -U flash:w:$(SRC).hex:i

read_fuse:
	avrdude -c $(PRGRMR) -p $(TARGET) -P /dev/ttyUSB0 -U efuse:r:efuse.hex:h -U lfuse:r:lfuse.hex:h -U hfuse:r:hfuse.hex:h

# set clock ot internal 8Mhz
write_fuse:
	avrdude -c $(PRGRMR) -p $(TARGET) -P /dev/ttyUSB0 -U lfuse:w:0xe2:m
clean:
	rm *.o

