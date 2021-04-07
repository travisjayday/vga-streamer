#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <time.h>

#include "des_data.c"

uint64_t perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask);

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

uint64_t key_scheduler(uint8_t iter, uint64_t* cd);
uint32_t apply_f(uint32_t r, uint64_t k);

uint64_t 
encrypt_des(uint64_t block, uint64_t key) 
{
    block = perm_bitstring(block, 64, perm_mask_ip); 
    uint32_t l_i = (0xffffffff00000000 & block) >> 32;
    uint32_t r_i = (0x00000000ffffffff & block);
    uint64_t cd_i = perm_bitstring(key, 64, perm_mask_pc1) & 0x00ffffffffffffff; 
    uint32_t tmp; 

    key = 0;
    for (int i = 0; i < 16; i++) {
        key = key_scheduler(i + 1, &cd_i); 
        tmp = r_i;
        r_i = l_i ^ apply_f(r_i, key); 
        l_i = tmp; 
    }

    block = (((uint64_t) r_i) << 32) | ((uint64_t) l_i); 
    return perm_bitstring(block, 64, perm_mask_ipi);
}


/** key_scheduler - Given the current iteration CD blocks, update CD 
 *                  and  return the next key. 
 */
inline uint64_t 
key_scheduler(uint8_t iter, uint64_t* cd)
{
    uint64_t out = *cd;
    uint64_t smask1  = 0x0100000010000000; // set bit D[n+1] (C[0]) and bit C[n+1]
    uint64_t smask1i = 0x00ffffffefffffff; // discard C[0] and irrelevant bits

    uint64_t smask2  = 0x0300000030000000; // set D[n+2:n+1] (C[1:0) and C[n+2:n+1] 
    uint64_t smask2i = 0x00ffffffcfffffff; // discard C[1:0] and irrelevant bits
 
    if (iter == 1 || iter == 2 || iter == 9 || iter == 16) {
        // rotate C and D left by 1 
        out = (out << 1); 
        out = (out & smask1i) | ((out & smask1) >> 28); 
    }
    else {
        // rotate left by 2
        out = (out << 2); 
        out = (out & smask2i) | ((out & smask2) >> 28); 
    }

    // update C and D blocks
    *cd = out; 
    
    // return the 48 bit 
    return perm_bitstring(out, 64, perm_mask_pc2) & 0x0000ffffffffffff; 
}

//#define TESTING


/** apply_f - Apply the f(x), where f is defined as in DES Spec.
 */
inline uint32_t 
apply_f(uint32_t r, uint64_t k)
{
    int8_t   si;      
    uint8_t  cmd, val;       
    uint32_t out = 0; 
    uint8_t  idx; 


#ifdef TESTING
    k = 0x1b02effc7072;
    r = 0xf0aaf0aa;
    uint64_t __test1 = 0x7a15557a1555;
#endif

    // MSB of r is 1st position. 32 position is LSB.
    uint64_t t = (uint64_t) r;
    uint64_t re = (t & 0xf8000000) << 15 |
                  (t & 0x1f800000) << 13 |
                  (t & 0x01f80000) << 11 |
                  (t & 0x001f8000) << 9  |
                  (t & 0x0001f800) << 7  |
                  (t & 0x00001f80) << 5  |
                  (t & 0x000001f8) << 3  |
                  (t & 0x0000001f) << 1  | 
                  (t & 0x00000001) << 47 |
                  (t >> 31);

#ifdef TESTING
    if (re == __test1) printf("E-Selection Table Passed!\n"); 
    else printf("Failed E-Selection Table Test!! r=%x perm: %lx\n", r, re);
#endif

    uint64_t x = (uint64_t) re ^ k; 

    /**
     *  Do the sblock calculations. Convert 48 bit --> 32 bit output. 
     */ 
    for (si = 7; si >= 0; si--, x >>= 6) {
        /* Get 6 bits as cmd word. B8, ..., B1 */
        cmd = x & 0x3f;                             
        
        /* Compute the S-Block byte-level index into the table */
        idx = ((cmd >> 2) | (cmd << 4)) & 0x1f;

        /* Find S8, ..., S1 and add byte-level idx, then fetch*/
        val = sblocks[(si << 5) + idx];

        /* If cmd uses upper nibble, shift it */
        if (cmd & 2) val >>= 4; 

        /* Shift and or value into out */
        out >>= 4; 
        out |= val << 0x1c;
    }

#ifdef TESTING
    uint32_t __test1_out = 0x5c82b597;
    if (out == __test1_out) printf("S-Block Test Passed!\n");
    else printf("Failed S-Block Test!! out=%x != %x", out, __test1_out);
#endif
    
    out = perm_bitstring(out, 32, perm_mask_p); 

#ifdef TESTING
    __test1_out = 0x234aa9bb;
    if (out == __test1_out) printf("P Perm Test Passed!\n");
    else {
        printf("Failed P Perm Test!! out=%x != %x\n", out, __test1_out);
        print_bits(out);
    }
#endif

    return out; 
}



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
    /*
    uint64_t x = 0b110000100000000001010000110000010000110001011101;
    printf("x = %d", x);
    uint32_t res = apply_f(x);
    printf("f(%ld) = 0x%x\n", x, res);
    if (res != 0xf00dbabe) 
        printf("Failed f() function!!\n");
    else
        printf("Passed f() function test!!\n");
    */
    printf("Encrypt: %lx", encrypt_des(0x0, 0x1), 0x0);
    //apply_f(0, 0);

    return 0;
    uint8_t src[] = { 1, 0, 2, 3 };
    uint8_t dst[] = { 1, 2, 0, 3 };
    perm_bitstring_dumb(0b1000, 64, src, dst);
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
