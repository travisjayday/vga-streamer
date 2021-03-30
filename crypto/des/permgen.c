#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

/**
 * butterfly_n - Compute the butterfly output given a mask.
 * @n: number of input elements
 * @given: starting pattern
 * @mask: the mux-state mask 
 * @output: pointer to write the resulting permutation to.
 */
static inline void 
butterfly_n(int n, char* given, int mask, char* output)
{
    memcpy(output, given, n + 1); 

    int n_div2 = n / 2; 
    char tmp;
    for (int i = 0; i < n_div2; i++) {
        if (mask & (int) 1 << (n_div2-i-1)) {
            tmp = output[i];
            output[i] = output[n_div2 + i];
            output[n_div2 + i] = tmp;
        }
    }
}

/**
 *  force_mask - Bruteforce a butterfly mask 
 *  @n: number of input elements
 *  @given: starting pattern
 *  @desired: desired permutation of given
 *
 *  Given an n-bit pattern in the shape of a char array, 
 *  and a desired butterfly permutation of that array, 
 *  brute force the correct state of the n/2 muxes to 
 *  generate the n-bit mux mask that yields the desired
 *  permutation. 
 */
int
force_mask(int n, char* given, char* desired)
{
    uint64_t mask = 0; 
    uint64_t n_div2 = n / 2; 
    uint64_t overflow = 1ULL << n_div2;

    char* output = malloc(n + 1);
    do {
        butterfly_n(n, given, mask, output); 
        //if (strcmp(output, desired) == 0) 
            //return (int) ((mask << n_div2) | mask); 
        printf("%x -> %s\n", mask, output);
    } while (((++mask) & overflow) == 0ULL);

    return -1;
}

int 
main()
{
    int n = 8;
    char desired[] = "ACDB";

    char* given = malloc(n + 1); 
    int i;
    for (i = 0; i < n; i++) given[i] = 'A' + i;
    given[i] = '\0';

    char* output = malloc(n + 1);
    int mask = force_mask(n, given, desired); 

    butterfly_n(n, given, mask, output); 
    printf("found mask: %x; desired: %s; generated: %s", mask, desired, output);
}
