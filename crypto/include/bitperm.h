#ifndef BITPERM_H

#define BITPERM_H
#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <time.h>

uint64_t perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask);
uint64_t perm_bitstring_dumb(uint64_t x, uint8_t n, uint8_t* src, uint8_t* dst);

#endif
