FILENAME   = main
LIBNAME = HD44780
DEVICE     = atmega32
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)

default: compile 

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c $(LIBNAME).c -o $(LIBNAME).o
	$(COMPILE) -o $(FILENAME).elf $(LIBNAME).o $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

clean:
	rm $(FILENAME).o
	rm ${LIBNAME}.o
	rm $(FILENAME).elf
	rm $(FILENAME).hex