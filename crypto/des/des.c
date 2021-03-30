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

// See http://programming.sirrida.de/bit_perm.html#benes for butterfly reference

/*
 * Equivalent to bit rotate by 4. 
 * Swaps lower half with upper half.
 */
static inline uint8_t 
subflip8(uint8_t n)
{
    return (n << 4 | n >> 4); 
}

/*
 * Equivalent to bit rotate by 2. 
 * Swaps lower half with upper half. 
 * This function does it on the two 4bit subwords. 
 */
static inline uint8_t 
subflip4(uint8_t n)
{
    return (((n << 2) & 0xcc) | ((n >> 2) & 0x33)); 
}

/*
 * Equivalent to bit rotate by 1. 
 * Swaps lower half with upper half. 
 * This function does it on the four 2bit subwords. 
 */
static inline uint8_t 
subflip2(uint8_t n)
{
    return (((n << 1) & 0xaa) | ((n >> 1) & 0x55)); 
}


static inline uint8_t 
butterfly2(uint8_t input, uint8_t mask) 
{
    return ((~mask) & input) | subflip2(mask & input); 
}

static inline uint8_t 
butterfly4(uint8_t input, uint8_t mask) 
{
    return ((~mask) & input) | subflip4(mask & input); 
}

static inline uint8_t 
butterfly8(uint8_t input, uint8_t mask) 
{
    return ((~mask) & input) | subflip8(mask & input); 
}

/*
 *  masks = 00 [ 2 bits ] [ 2 bits ] [ 2 bits ] 
 *               layer 3    layer 2    layer 1  mask
 */
static inline uint8_t
perm4(uint8_t input, uint8_t masks)
{
    print_bits(masks);
    uint8_t layer1_mask = ((masks & 0x3) << 2) | (masks & 0x3);
    uint8_t layer2_mask = (masks & 0xc) >> 2;
    uint8_t layer3_mask = ((masks & 0x30) >> 2) | ((masks & 0x30) >> 4);
    print_bits(layer1_mask);
    print_bits(layer2_mask);
    print_bits(layer3_mask);
    input = butterfly4(input, layer1_mask);     // layer 1
    input = butterfly2(input, layer2_mask);     // layer 2
    input = butterfly4(input, layer3_mask);     // layer 3
    return input; 
}

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
    for (layer = n; layer >= 4; layer >>= 1, mask++)
        x = do_delta_swap(x, layer, *mask);
    for (layer = 2; layer <= n; layer <<= 1, mask++) 
        x = do_delta_swap(x, layer, *mask);
    return x;
}

uint64_t 
bench_perm_bitstring(uint64_t x, uint8_t n, uint64_t* mask)
{
    double freq = 3600.033e6;
    uint64_t start_time = __rdtsc();

    perm_bitstring(x, n, mask);

    uint64_t elapsed = (__rdtsc() - start_time);
    double seconds = (elapsed * freq) ;
    printf("Done in %ld clock_cylcles --> %.17g seconds \n",  elapsed, 1.0 / seconds);
}

int 
main() 
{
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
    uint64_t x = bench_perm_bitstring(0b11111111111111101000000000000000, 32, masks);
    printf("result: %ld", x);

    uint64_t masks2[] = {
        716730551, 120972048890684, 13511314278318134, 360292381121380866, 1224980267144314896, 4706262990382519364, 2522582113671189296, 576743339878189828, 36873423817474067, 63836098968526, 2861590325

    };
    for (int i = 0; i < 10000; i++) {
    uint64_t x2 = bench_perm_bitstring(0b0010110100000111001100001011110010000100001100001110111001110001, 64, masks2);
    printf("result: %ld", x2);
    }

    return 0;
 
    /*
    uint64_t masks[] = {
        0b00000011,
        0b00010001,        
        0b00010100,
        0b00000010,
        0b00001101
    };
    uint64_t x = perm_bitstring(0b10011100, 8, masks);
    printf("result: %ld", x);
   
    printf("layer 8\n");
    print_bits(do_delta_swap(0b10011100, 8, 0b00000011));
    printf("layer 4\n");
    print_bits(do_delta_swap(0b00100011010, 4, 0b00010001));
    printf("layer 2\n");
    print_bits(do_delta_swap(0b01100110, 2, 0b00010100));
    printf("layer 4\n");
    print_bits(do_delta_swap(0b01100110, 4, 0b00000010));
    print_bits(do_delta_swap(0b01101100, 8, 0b00001101));*/
    return 0;
    uint8_t res = 0;

    // test 4 input 2 bit butterfly
    res = butterfly2(0x82, 0xcc);
    assert(res == 0x42);

    // test 2 input 4 bit buttefly 
    res = butterfly4(0x9b, 0xa5);
    assert(res == 0x3e);

    // test 1 input 8 bit buttefly
    res = butterfly8(0xca, 0x77);
    assert(res == 0xac);

    // test 4 bit perm 
    res = perm4(0b1011, 0b00000011);       // abcd -> cdab 
    assert(res == 0b1110);
    res = perm4(0b1011, 0b00011101);    // abcd -> acbd
    assert(res == 0b1101);
    res = perm4(0b1101, 0b00011101);   // acbd -> abcd
    assert(res == 0b1011);

    return 0;
}
