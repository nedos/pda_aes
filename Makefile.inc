# put common definitions in here
CC          = avr-gcc
OBJCOPY     = avr-objcopy
OBJDUMP     = avr-objdump
RM          = rm -vf
ECHO        = echo
ROUNDS      = 0
DELAY       = 0
STARTUP     = 0

# AVR Settings
MCU_TARGET  = atmega328p
BAUD_RATE   = 9600
F_CPU       = 16000000

# FLAGS
OPTIMIZE    = -O2
CFLAGS      = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(F_CPU)UL -DBAUD_RATE=$(BAUD_RATE)UL -Werror -DROUNDS=$(ROUNDS) -DDELAY=$(DELAY) -DSTARTUP=$(STARTUP)
LDFLAGS     = -Wl,-Map,$(PRG).map
OBJDUMPFLAGS = -h -S
