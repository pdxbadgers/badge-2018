CFLAGS=-Wall -g -Os -mmcu=attiny861
SRC = main

build: $(SRC).c
	avr-gcc $(CFLAGS) -o $(SRC).bin $(SRC).c
	avr-objcopy -j .text -j .data -O ihex $(SRC).bin $(SRC).hex
    
flash:
	avrdude -c arduino -p t861 -b 19200  -P /dev/ttyUSB0 -U flash:w:$(SRC).hex:i

clean:
	rm *.o

