#include "bitperm.h"

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
