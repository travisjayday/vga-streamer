#include "bitperm.h"

uint64_t
perm_bitstring_dumb(uint64_t x, uint8_t n, uint8_t* src, uint8_t* dst)
{
    uint64_t res = 0; 
    uint8_t* bits = malloc(n * sizeof(uint8_t));
    uint8_t* permbits = malloc(n * sizeof(uint8_t));
    uint8_t bit_i = n - 1;  
    uint64_t i = 1;
    uint8_t j; 
    uint64_t max = n == 64? 0 : 1ULL << n; 

    /* Set the bit in the array if its set */
    for (i = 1; i != max; i <<= 1, bit_i--) 
        bits[bit_i] = (i & x)? '1' : '0';               

     /* For each source bit, find the corresponding destination bit
      * and move the src's value into the desintations' index. 
      */ 
    for (i = 0; i < n; i++) 
       for (j = 0; j < n; j++) 
            if (dst[j] == src[i]) 
                permbits[j] = bits[i];              

    /* Set the bit if it's set in the array */
    bit_i = n - 1;
    for (i = 1; i != max; i <<= 1, bit_i--) 
        if (permbits[bit_i] == '1') res = res | i;     

    return res; 
}

// See http://programming.sirrida.de/bit_perm.html#benes for butterfly reference
static inline uint64_t 
do_delta_swap(uint64_t x, uint8_t n, uint64_t mask)
{
    uint64_t n_div2 = (uint64_t) (n / 2);
    uint64_t y = (x ^ (x >> n_div2)) & mask;
    x = x ^ y ^ (y << n_div2); 
    return x; 
}

uint64_t 
perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask) 
{
    uint8_t layer; 
    for (layer = n; layer >= 4; layer >>= 1, mask++) x = do_delta_swap(x, layer, *mask);
    for (layer = 2; layer <= n; layer <<= 1, mask++) x = do_delta_swap(x, layer, *mask); 
    return x;
}
