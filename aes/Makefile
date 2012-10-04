include Makefile.inc

INCLUDE = ../gf256mul
OBJS = aes128_enc.o aes_enc.o aes_invsbox.o aes_keyschedule.o aes_sbox.o

debug: CFLAGS += -DDEBUG
debug: all
verbose: CFLAGS += -DVERBOSE
verbose: all
photon: CFLAGS += -DPHOTON
photon: all
photon-verbose: CFLAGS += -DPHOTON
photon-verbose: CFLAGS += -DVERBOSE
photon-verbose: all

all: $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	$(RM) *.o
	$(RM) *.s
	$(RM) *~
