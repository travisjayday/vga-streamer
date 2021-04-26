#ifndef BITPERM_H

#define BITPERM_H
#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <time.h>

/**
 * perm_bitstring - Permute a bitsting according to a bit-permutation mask.
 * @x: The bitstring to permutate.
 * @n: The power of two size of the bitstring (8, 16, 32, 64 suppported)
 * @mask: A pointer to an array of masks.
 *
 * To generate a permutation, use src/tools/permgen.py's get_masks function. 
 */
uint64_t perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask);

#ifdef PSOC_RELEASE
/**
 * perm_bitstring_dumb - Permutes a bitstring according to src and dest arrays.  
 * @x: The bitstring to permutate.
 * @n: The size of the bitstring.
 * @src: An array of source symbols. E.g. {1, 2, 3, 4};
 * @dst: An arrya of desitination symbols. E.g. {3, 2, 1, 4};
 *
 * Uses simple swaps to solve a given permutation. 
 */
uint64_t perm_bitstring_dumb(uint64_t x, uint8_t n, uint8_t* src, uint8_t* dst);
#endif

/** 
 * _print_bits - Print the bits of an integer.
 * @n: The integer
 * @bits: The width of the integer in bits
 */
void _print_bits(uintmax_t n, uint32_t bits);

/**
 * print_bits - Prints the bits of an integer.
 * @n: The integer (fixed sizeof(int) width)
 */
void print_bits(uintmax_t n);


#endif
