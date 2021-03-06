#ifndef JPG_H

#define PSOC 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "string.h"
#include <stdint.h>

#ifdef BENCH
#define BENCH_JPEG 1
#endif

/* Define if shuold encode with integer arithemtic instead of double */
//#define USE_DCT_INT_ENCODING      

/* Set to 1 if should normalize the DFT */
#define NORMALIZE_DFT 0

/* Define if should decode (IDCT) with integer arithemtic instead of double */
#define USE_DCT_INT_DECODING

/* Scale factor used to scale up rational constants in integer IDCT */
#define IDF_SF 8

#ifdef __cplusplus
#define SERVER_BUILD 1
#define restrict
#endif

#define PI 3.14159

extern uint16_t ac_lht_codes[];
extern uint16_t dc_lht_codes[];
extern uint8_t col_lin2diag[];
extern uint8_t row_lin2diag[];
extern uint8_t ac_lht_idx16[];

#if (SERVER_BUILD==1 || (PSOC != 1 && BENCH_JPEG != 1))
extern "C" 
#endif
uint8_t* compress_channel(uint32_t* outsize, uint8_t* image, uint32_t width, uint32_t height);

#if (SERVER_BUILD==1 || (PSOC != 1 && BENCH_JPEG != 1))
extern "C" 
#endif
uint32_t decompress_channel(uint8_t* dest, const uint8_t* compressed_image, uint32_t width, uint32_t height);

#endif
