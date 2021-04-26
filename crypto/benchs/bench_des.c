#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <stdint.h>
#include <linux/byteorder/big_endian.h>
#include <des.h>

 
uint64_t 
openssl_encrypt_des_blk(uint64_t block, uint64_t key)
{
    uint64_t out = 0;

    DES_key_schedule keysched;
    DES_set_key((uint8_t (*)[8]) &key, &keysched);
    DES_ecb_encrypt((uint8_t (*)[8]) &block, 
            (uint8_t (*)[8]) &out, &keysched, DES_ENCRYPT);
 
    return __cpu_to_le64(out);
}


/**
 * bench_des_openssl - benchmark openssl DES given a list of keys and values.
 * @keys: An array of 64-bit keys.
 * @keys_n: Length of keys.
 * @blocks: An array of 64-bit data blocks.
 * @blocks_n: Length of blocks.
 * @samples_per_key: Number of blocks to be encrypted by key. 
 */
double
bench_des_openssl(
        uint64_t* blocks, int blocks_n, 
        uint64_t* keys, int keys_n, 
        int samples_per_key)
{
    /* Start timer */
    uint64_t start_time = __rdtsc();

    /* Do computations */
    int i, j;
    uint64_t x;
    for (i = 0; i < keys_n; i++, keys++)
        for (j = 0; j < samples_per_key; j++, blocks++)
            x = openssl_encrypt_des_blk(*blocks, *keys);

    /* Print Results */
    uint64_t elapsed = (__rdtsc() - start_time);
    double seconds = (elapsed / 3600.033e6);
    double seconds_per_call = seconds / (keys_n * samples_per_key);

    printf("OpenSSL DES Results:\n");
    printf("-----------------\n");
    printf("Ticks elapsed: \t%ld\n", elapsed);
    printf("Total seconds: \t%.17g\nSeconds / Call: %.17g\n", seconds, seconds_per_call);
    printf("Sanity Check: \t0x%lx\n\n", x);

    return seconds_per_call;
}

/**
 * bench_des - benchmark my DES given a list of keys and values.
 * @keys: An array of 64-bit keys.
 * @keys_n: Length of keys.
 * @blocks: An array of 64-bit data blocks.
 * @blocks_n: Length of blocks.
 * @samples_per_key: Number of blocks to be encrypted by key. 
 */
double
bench_des(uint64_t* blocks, int blocks_n,
          uint64_t* keys, int keys_n, 
          int samples_per_key)
{
    /* Start timer */
    uint64_t start_time = __rdtsc();

    /* Do computations */
    int i, j;
    uint64_t x;
    for (i = 0; i < keys_n; i++, keys++)
        for (j = 0; j < samples_per_key; j++, blocks++)
            x = encrypt_des_blk(*blocks, *keys);

    /* Print Results */
    uint64_t elapsed = (__rdtsc() - start_time);
    double seconds = (elapsed / 3600.033e6);
    double seconds_per_call = seconds / (keys_n * samples_per_key);

    printf("My DES Results:\n");
    printf("-----------------\n");
    printf("Ticks elapsed: \t%ld\n", elapsed);
    printf("Total seconds: \t%.17g\nSeconds / Call: %.17g\n", seconds, seconds_per_call);
    printf("Sanity Check: \t0x%lx\n\n", x);

    return seconds_per_call;
}
 
int main() 
{
    printf("encrypted 0: %lx", openssl_encrypt_des_blk(0, 0));
    return 0;
}
