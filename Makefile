include Makefile.inc

#ROUNDS = 0
DIRS = aes gf256mul
PRG  = pda_aes
OBJS = pda_aes.o uart.o version.o
LIBS = gf256mul/gf256mul.o aes/aes128_enc.o aes/aes_enc.o aes/aes_invsbox.o aes/aes_keyschedule.o aes/aes_sbox.o
PROG = usbtiny
#PORT = -P usb

all: $(PRG).elf

# Full-Debug printf
debug: CFLAGS += -DDEBUG
debug: MFLAGS += debug
debug: all

# Basic printf
verbose: CFLAGS += -DVERBOSE
verbose: MFLAGS += verbose
verbose: all

# Photon
photon: CFLAGS += -DPHOTON
photon: MFLAGS += photon
photon: all

# Basic printf
photon-verbose: CFLAGS += -DPHOTON
photon-verbose: CFLAGS += -DVERBOSE
photon-verbose: MFLAGS += photon-verbose
photon-verbose: all

$(PRG).elf: $(DIRS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

aes: force_look
	cd aes; $(MAKE) $(MFLAGS)

gf256mul: force_look
	cd gf256mul; $(MAKE)

version.o:
	-./version.sh
	$(CC) $(CFLAGS) -c -o version.o version.c

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

program: $(PRG).hex
	avrdude -p m328p -c $(PROG) $(PORT) -U flash:w:$(PRG).hex

erase: $(PRG).hex
	avrdude -p m328p -c $(PROG) $(PORT) -e

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	$(RM) *.o $(PRG).elf *.map *.hex
	$(RM) version.c
	$(RM) *~
	$(RM) *.lst
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

asm:
	$(OBJDUMP) $(OBJDUMPFLAGS) $(PRG).elf > $(PRG).lst

1mhz:
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x3f:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lfuse:w:0xd2:m -U hfuse:w:0xda:m -U efuse:w:0x05:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x0f:m

8mhz:
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x3f:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lfuse:w:0xe2:m -U hfuse:w:0xda:m -U efuse:w:0x05:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x0f:m

16mhz:
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x3f:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0x05:m
	avrdude -p m328p -c $(PROG) $(PORT) -e -U lock:w:0x0f:m

force_look:
	true
