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

/**
 * bench_perm_bitstring_dumb - benchmark dumb bitstring permutations 
 *
 * @n: The number of bits which will be permuatated.
 * @srcs: Nested array of source permutation. E.g. [[1, 2, 3], [2, 3, 1], ...]]
 * @dsts: Nested array of dest permutation.   E.g. [[3, 2, 1], [2, 1, 3], ...]]
 * @perms_n: Length of srcs and dsts arrays.
 * @samples: Pointer to an array of values to permute.
 * @samples_per_perm: Number of samples to run for each permutation mask
 *
 * Note: samples needs to point to a block of uint64_t's of size 
 *       samples_per_perm * perms_n = total number of samples
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
