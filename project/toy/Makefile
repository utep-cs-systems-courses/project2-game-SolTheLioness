# Makefile configuration
COMMON_OBJECTS = main.o buzzer.o state4.o
CPU            = msp430g2553
CFLAGS         = -mmcu=${CPU} -I../h
LDFLAGS        = -L/opt/ti/msp430_gcc/include

# Switch the compiler (for internal make rules)
CC             = msp430-elf-gcc
AS             = msp430-elf-as

all: toy.elf

# Additional rules for files
toy.elf: ${COMMON_OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a

load: toy.elf
	msp430loader.sh toy.elf

clean:
	rm -f *.o *.elf

main.o: led.h buzzer.h

buzzer.o: buzzer.h

state4.o: state4.s

wdInterruptHandler.o: led.h
