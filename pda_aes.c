#ifdef DEBUG
#define VERBOSE
#endif

#ifdef VERBOSE
#endif

#include <stdio.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include <util/delay.h>
#include "aes/aes.h"
#include "aes/aes_sbox.h"
#include "uart.h"
#include "version.h"
#include "keys.h"
#include "trigger.h"

uint8_t init[16];

#if ROUNDS == 0
uint16_t write_cycles EEMEM;
uint8_t input_byte EEMEM;
#endif

void print_data(uint8_t* data, unsigned len) {
    int i;
#ifdef DEBUG
    printf("    ");
    for(i=0; i < len; i++) {
        if(i != 0) {
            if(i % 4 == 0) {
                printf("\n");
                printf("    ");
            } else {
                printf(" ");
            }
        }
        printf("0x%.2X", data[i]);
    }
    printf("\n");
#else
    for(i=0; i < len; i++) printf("%.2X", data[i]);
    printf("\n");
#endif
}

void print_128(uint8_t* data) {
    print_data(data, 16);
}

void io_init(void) {
    uart_init();
    DDRB = 0xff;
#if ROUNDS == 0
    uint16_t writes = eeprom_read_word(&write_cycles);
    writes++;
    eeprom_write_word(&write_cycles, writes);
    uint8_t byte = eeprom_read_byte(&input_byte);
    byte++;
    eeprom_write_byte(&input_byte, byte);
#endif
    pda_reset_trigger();
}

void reset_input(uint8_t* input, uint8_t val) {
    int i;
    for (i = 0; i < 16; i++)
        input[i] = val;
}

int main(void) {
    io_init();
    uint8_t input[16];
    aes128_ctx_t ks, iks;


    printf("Build Date: %s\n", build_date);
    printf("Git:        %s\n\n", build_git_sha);
    printf("  Photon DA AES\n");
    printf("-----------------\n");
    printf("ROUNDS  = %lu\n", (unsigned long) ROUNDS);
    printf("DELAY   = %lu\n", (unsigned long) DELAY);
    printf("STARTUP = %lu\n", (unsigned long) STARTUP);
    printf("-----------------\n");
    printf("SBOX:  0x%.2X\n", (unsigned int) aes_sbox);
    printf("Input: 0x%.2X\n", (unsigned int) input);
    printf("Key:   0x%.2X\n", (unsigned int) key);
    printf("-----------------\n");
#if ROUNDS == 0
    uint16_t writes = eeprom_read_word(&write_cycles);
    uint8_t byte = eeprom_read_byte(&input_byte);
    printf("write_cycles = %u\n", writes);
    printf("input_byte = 0x%.2X\n", byte);
#endif
#ifdef PHOTON
    printf("Computing S-Box only!\n");
#endif
    printf("\n");

#if STARTUP > 0
    _delay_ms(STARTUP * 1000);
#endif

#ifdef VERBOSE
    printf("Printing AES Key:\n");
    print_128(key);
    printf("\n");
#endif

    // aes128_init(const void* key, aes128_ctx_t* ctx);
    aes128_init(key, &ks);

#ifdef VERBOSE
    printf("Printing AES SBOX:\n");
    uint16_t s = 0;
    for(s=0; s < 256; s++) {
        printf("%.2X", aes_sbox[s]);
        if((s + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
#endif

#if ROUNDS > 0
    unsigned long round = 1;
    uint8_t i=0;
    unsigned long total_rounds = 256 * ROUNDS;
    while(round <= total_rounds) {
        reset_input(input, i);
#else
    reset_input(input, byte);
    memcpy(&init,&input,sizeof(uint8_t)*16);
    printf("Input:\n");
    print_128(input);
    printf("\n");
    while(1) {
        memcpy(&input,&init,sizeof(uint8_t)*16);
#endif
        memcpy(&iks,&ks,sizeof(aes128_ctx_t));

#ifdef VERBOSE
#if ROUNDS > 0
        printf("[%6ld]: Input:\n", round);
#else
        printf("Input:\n");
#endif
        print_128(input);
        printf("\n");
#endif

        // void aes128_enc(void* buffer, aes128_ctx_t* ctx);
        aes128_enc(input, &ks);

#ifdef VERBOSE
#if ROUNDS  > 0
        printf("[%6ld]: Result:\n", round);
#else
        printf("Result:\n");
#endif

        //Result gets written back to the input
        print_128(input);
        printf("\n");
#endif

#if ROUNDS > 0
        round++;
        if ( (round - 1) % ROUNDS == 0)
            i++;
#endif
#if DELAY > 0
        _delay_ms(DELAY);
#endif
    }
#if ROUNDS  > 0
    printf("Exiting... after round %li\n", round - 1);
#endif

    return(0);
}
