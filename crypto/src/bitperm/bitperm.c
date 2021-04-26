#include "bitperm.h"

void _print_bits(uintmax_t n, uint32_t bits) {
    char c;
    for (size_t i = bits; i-- != 0;)
    {
        if ((n & (1UL << i)) != 0) c = '1';
        else c = '0';
        printf ("%c", c);
    }
    putchar('\n');
}

void print_bits(uintmax_t n)
{
    _print_bits(n, 8 * sizeof(int));
}

// See http://programming.sirrida.de/bit_perm.html#benes for butterfly reference
uint64_t 
do_delta_swap(uint64_t x, uint8_t n, uint64_t mask)
{
    uint64_t n_div2 = (uint64_t) (n / 2);
    uint64_t y = (x ^ (x >> n_div2)) & mask;
    x = x ^ y ^ (y << n_div2); 
    return x; 
}

#define __do_delta_swap                 \
    n_div2 = layer >> 1;                \
    y = (x ^ (x >> n_div2)) & *mask;         \
    x = x ^ y ^ (y << n_div2); 

#define ___do_delta_swap(n_div2)             \
    y = (x ^ (x >> n_div2)) & *mask;         \
    x = x ^ y ^ (y << n_div2);          \
    mask++;
 
uint64_t 
_perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask) 
{
    uint8_t layer; 
    uint64_t n_div2;
    uint64_t y; 

    for (layer = n; layer >= 4; layer >>= 1, mask++) {
        __do_delta_swap
        //x = do_delta_swap(x, layer, *mask);
    }
    for (layer = 2; layer <= n; layer <<= 1, mask++)  {
        __do_delta_swap
        //x = do_delta_swap(x, layer, *mask);
    }
    return x;
}

uint64_t 
perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask) 
{
    uint64_t y; 

    if (n == 32) {
        ___do_delta_swap(16); 
        ___do_delta_swap(8); 
        ___do_delta_swap(4); 
        ___do_delta_swap(2); 
        ___do_delta_swap(1); 
        ___do_delta_swap(2); 
        ___do_delta_swap(4); 
        ___do_delta_swap(8); 
        ___do_delta_swap(16); 
    }
    else if (n == 64) {
        ___do_delta_swap(32); 
        ___do_delta_swap(16); 
        ___do_delta_swap(8); 
        ___do_delta_swap(4); 
        ___do_delta_swap(2); 
        ___do_delta_swap(1); 
        ___do_delta_swap(2); 
        ___do_delta_swap(4); 
        ___do_delta_swap(8); 
        ___do_delta_swap(16); 
        ___do_delta_swap(32); 
    }
    return x;
}
