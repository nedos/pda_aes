#ifndef _TRIGGER_H
#define _TRIGGER_H 1

#define pda_reset_trigger() PORTB |= _BV(PB5);
#define pda_trigger() PORTB &= ~(_BV(PB5));

#endif
