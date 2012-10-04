include Makefile.inc

all: gf256mul.o

%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o
	$(RM) *.~
