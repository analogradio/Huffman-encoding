#include <assert.h>
#include <stdbool.h>
#include "bit_writer.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

BitWriter open_bit_writer(const char* path) {
	return (BitWriter) { .file = fopen(path, "wb"), .current_byte = 0x00, .num_bits_left = 8 };
}

// void write_bits(BitWriter* a_writer, uint8_t bits, uint8_t num_bits_to_write) { 
// 	assert(num_bits_to_write >= 0 && num_bits_to_write <= 8);;
// 	assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);

// 	// if(num_bits_to_write > a_writer -> num_bits_left) { 
// 	// 	// uint8_t final_num_bits = num_bits_to_write - a_writer -> num_bits_left;
// 	// 	//getting seg fault when i test this loop 
// 	// }
// 	// else {
// 	// 	uint8_t bits_left = bits << (8 - num_bits_to_write);
// 	// 	uint8_t proper = bits_left >> (8 - a_writer -> num_bits_left);	
// 	// 	a_writer -> num_bits_left -= num_bits_to_write;
// 	// 	a_writer -> current_byte = proper | a_writer -> current_byte;

// 	// 	if(a_writer -> num_bits_left == 0) {
// 	// 		flush_bit_writer(a_writer);
// 	// 	}
// 	// }
// 	assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);
// }

void write_bits(BitWriter* a_writer, uint8_t bits, uint8_t num_bits_to_write) {
    assert(num_bits_to_write >= 0 && num_bits_to_write <= 8);
    assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);

    if(num_bits_to_write <= a_writer -> num_bits_left) { //  
        uint8_t num_left = bits << (8 - num_bits_to_write);
        uint8_t pog = num_left >> (8 - a_writer -> num_bits_left);
        a_writer -> num_bits_left -= num_bits_to_write;
        a_writer -> current_byte = pog | a_writer -> current_byte;

        if(a_writer -> num_bits_left == 0) {
            flush_bit_writer(a_writer);
        }
    }
    else {
        uint8_t num_bits_2 = num_bits_to_write - a_writer -> num_bits_left;
        write_bits(a_writer,bits >> num_bits_2, a_writer -> num_bits_left);
        write_bits(a_writer, bits, num_bits_2);
    }

    assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);
}

void flush_bit_writer(BitWriter* a_writer) {
	if (a_writer -> num_bits_left != 8) {
		fputc(a_writer -> current_byte, a_writer -> file);
		a_writer -> current_byte = 0x00;
		a_writer -> num_bits_left = 8;
	}
}
void close_bit_writer(BitWriter* a_writer) {
	flush_bit_writer(a_writer);
	fclose(a_writer -> file);
	a_writer -> file = NULL;
}

#define __BIT_WRITER_C_V1__

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab colorcolumn=96: */
