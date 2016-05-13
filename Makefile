MCU_GCC=atmega16
MCU_AVRDUDE=m16
F_CPU=16000000UL

PROGRAMMER=usbasp
BITCLOCK=1 # bigger is slower

FLAGS=-Wall -DF_CPU=$(F_CPU) -Wl,-u,vfprintf -lprintf_flt -lm -Os

all:
	avr-gcc -mmcu=$(MCU_GCC) $(FLAGS) -o out.elf *.c 
	avr-objcopy -j .text -j .data -O ihex out.elf out.hex
	sudo avrdude  -p $(MCU_AVRDUDE) -c $(PROGRAMMER) -P usb -B $(BITCLOCK) -U flash:w:out.hex

clean:
	rm *.out *.hex
