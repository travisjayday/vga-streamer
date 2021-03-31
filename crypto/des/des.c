#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <time.h>

void print_bits(uintmax_t n)
{
    char c;
    for (size_t i = 8 * sizeof (int); i-- != 0;)
    {
        if ((n & (1UL << i)) != 0) c = '1';
        else c = '0';
        printf ("%c", c);
    }
    putchar('\n');
}

uint64_t
perm_bitstring_dumb(uint64_t x, uint8_t n, uint8_t* src, uint8_t* dst)
{
    uint64_t res = 0; 
    uint8_t* bits = malloc(n * sizeof(uint8_t));
    uint8_t* permbits = malloc(n * sizeof(uint8_t));
    uint8_t bit_i = n - 1;  
    uint64_t i;
    uint8_t j; 

    if (n != 64) {
        for (i = 1; i < (1 << n); i <<= 1, bit_i--) {
            bits[bit_i] = (i & x)? '1' : '0';               // set the bit in the array if its set
        }
    }
    else {
        for (i = 1; i != 0; i <<= 1, bit_i--) {
            bits[bit_i] = (i & x)? '1' : '0';               // set the bit in the array if its set
        }
    }
 
    for (i = 0; i < n; i++) {
        uint8_t current_bit = bits[i];             // get the value of the bit at current index
        for (j = 0; j < n; j++) {
            if (dst[j] == src[i]) 
                permbits[j] = current_bit;                 // set the value at destination index to current bit
        }
    }

    bit_i = n - 1;
    if (n != 64) {
        for (i = 1; i < 1 << n; i <<= 1, bit_i--) 
            if (permbits[bit_i] == '1') res = res | i;      // set the bit if it's set in the array
    }
    else {
        for (i = 1; i != 0; i <<= 1, bit_i--) 
            if (permbits[bit_i] == '1') res = res | i;      // set the bit if it's set in the array

    }

    return res; 
}

/**
 * bench_perm_bitstring_dumb - benchmark dumb bitstring permutations given src and destination masks.
 * @n: The number of bits which will be permuatated
 */
double
bench_perm_bitstring_dumb(uint8_t n, uint8_t** srcs, uint8_t** dsts, int perms_n,
        uint64_t* samples, int samples_per_perm)
{
    /* Start timer */
    uint64_t start_time = __rdtsc();

    /* Do computations */
    int i, j;
    uint64_t x;
    for (i = 0; i < perms_n; i++, srcs++, dsts++) 
        for (j = 0; j < samples_per_perm; j++, samples++) 
            x = perm_bitstring_dumb(*samples, n, *srcs, *dsts);

    /* Print Results */
    uint64_t elapsed = (__rdtsc() - start_time);
    double freq = 3600.033e6;
    double seconds = (elapsed / freq) ;
    double seconds_per_call = seconds / (perms_n* samples_per_perm);

    printf("Dumb C Results:\n");
    printf("-----------------\n");
    printf("Ticks elapsed: \t%ld\n", elapsed);
    printf("Total seconds: \t%.17g\nSeconds / Call: %.17g\n", seconds, seconds_per_call);
    printf("Sanity Check: \t0x%lx\n\n", x); 

    return seconds_per_call;
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

/**
 * bench_perm_bitstring - benchmark bitstring permutations given a list of masks.
 * @n: The number of bits which will be permuatated
 * @mask: A list of lists of masks 
 * @mask_n: Length of mask list
 * @samples: The number of randomly generated samples which will be run for each mask. 
 */
double
bench_perm_bitstring(uint8_t n, uint64_t** masks, int masks_n, 
        uint64_t* samples, int samples_per_mask)
{
    /* Start timer */
    uint64_t start_time = __rdtsc();

    /* Do computations */
    int i, j;
    uint64_t x;
    for (i = 0; i < masks_n; i++, masks++) 
        for (j = 0; j < samples_per_mask; j++, samples++) 
            x = perm_bitstring(*samples, n, *masks);

    /* Print Results */
    uint64_t elapsed = (__rdtsc() - start_time);
    double freq = 3600.033e6;
    double seconds = (elapsed / freq) ;
    double seconds_per_call = seconds / (masks_n * samples_per_mask);

    printf("Fast C Results:\n");
    printf("-----------------\n");
    printf("Ticks elapsed: \t%ld\n", elapsed);
    printf("Total seconds: \t%.17g\nSeconds / Call: %.17g\n", seconds, seconds_per_call);
    printf("Sanity Check: \t0x%lx\n\n", x); 

    return seconds_per_call;
}

int 
main() 
{

    uint8_t src[] = { 1, 0, 2, 3 };
    uint8_t dst[] = { 1, 2, 0, 3 };
    perm_bitstring_dumb(0b1000, 4, src, dst);
    return 0; 

    uint64_t masks0[] = {
        0, 0, 85, 51, 15
    };
    uint64_t x0 = perm_bitstring(0b11101000, 8, masks0);
    printf("result: %ld", x0);


    uint64_t masks1[] = {
        0, 0, 0, 21845, 13107, 3855, 255
    };
    uint64_t x1 = perm_bitstring(0b1111111010000000, 16, masks1);
    printf("result: %ld", x1);

    uint64_t masks[] = {
        0, 0, 0, 0, 1431655765, 858993459, 252645135, 65535, 65535
    };

    return 0;
}
