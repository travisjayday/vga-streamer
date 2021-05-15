#include "jpg.h"

#ifdef USE_DCT_INT_DECODING
#define pix_t int32_t
#else
#define pix_t double
#endif

/* For testing TGA files */
#if BENCH == 1
#include "targa.h"
#define DONTFAIL(x) do { tga_result res;  if ((res=x) != TGA_NOERR) { \
	printf("Targa error: %s\n", tga_error(res)); \
	exit(EXIT_FAILURE); } } while(0)
void 
load_tga(tga_image *tga, const char *fn)
{
	DONTFAIL( tga_read(tga, fn) );

	printf("Loaded %dx%dx%dbpp targa (\"%s\").\n",
		tga->width, tga->height, tga->pixel_depth, fn);

	if (!tga_is_mono(tga)) DONTFAIL( tga_desaturate_rec_601_1(tga) );
	if (!tga_is_top_to_bottom(tga)) DONTFAIL( tga_flip_vert(tga) );
	if (tga_is_right_to_left(tga)) DONTFAIL( tga_flip_horiz(tga) );

	if ((tga->width % 8 != 0) || (tga->height % 8 != 0))
	{
		printf("Width and height must be multiples of 8\n");
		exit(EXIT_FAILURE);
	}
}
#endif

uint8_t huff(int32_t prev_dcval, int32_t b[8][8], uint8_t* buf8);
static uint32_t dehuff(int32_t prev_dcval, const uint8_t* restrict buf8, int32_t outb[8][8]);

/* 
 * w, c: width of block. For columnwise DFT, use w=8, c=colum
 *       For row-wise DCT, use w=0, c=0. 
 */
void dct8(double b[8], double out[8], uint8_t w, uint8_t c) {
    double g[8] = {};
    double f[8] = {};
    double f_8; 

    // Stage 1
    g[0] =  b[0*w+c] + b[7*w+c];
    g[1] =  b[1*w+c] + b[6*w+c]; 
    g[2] =  b[2*w+c] + b[5*w+c];
    g[3] =  b[3*w+c] + b[4*w+c]; 
    g[4] = -b[4*w+c] + b[3*w+c]; 
    g[5] = -b[5*w+c] + b[2*w+c]; 
    g[6] = -b[6*w+c] + b[1*w+c];
    g[7] = -b[7*w+c] + b[0*w+c]; 

    // Stage 2
    f[0] =  g[0] + g[3];  
    f[1] =  g[1] + g[2]; 
    f[2] = -g[2] + g[1]; 
    f[3] = -g[3] + g[0]; 
    f[4] =  g[4] + g[5]; 
    f[5] =  g[5] + g[6];
    f[6] =  g[6] + g[7];
    f[7] =  g[7]; 

    // Stage 3
    g[0] =  f[0] + f[1];
    g[1] = -f[1] + f[0]; 
    g[2] =  f[2] + f[3];
    g[3] =  f[3]; 
    g[4] =  f[4]; 
    g[5] =  f[5]; 
    g[6] =  f[6];
    g[7] =  f[7]; 

    // Stage 4
    f_8  = -g[6] + g[4]; 
#ifndef USE_DCT_INT_ENCODING
    f_8  =   0.382 * f_8; 

    f[0] =  1  *g[0];          // *8
    f[1] =  1  *g[1];          // *8
    f[2] =  0.707  *g[2];           // *6
    f[3] =  1  *g[3];          // *8
    f[4] =  0.541  *g[4] + f_8;    // *4
    f[5] =  0.707  *g[5];               // *6
    f[6] =  1.306 *g[6] + f_8;    // *10
    f[7] =  1  *g[7] ;          // *8*/
#else
    f_8  =  3 * f_8; 

    f[0] =  8  *g[0];          // *8
    f[1] =  8  *g[1];          // *8
    f[2] =  6  *g[2];           // *6
    f[3] =  8  *g[3];          // *8
    f[4] =  4  *g[4] + f_8;    // *4
    f[5] =  6  *g[5];               // *6
    f[6] =  10 *g[6] + f_8;    // *10
    f[7] =  8  *g[7] ;          // *8*/
#endif

    // Stage 5
    g[0] =  f[0];    
    g[1] =  f[1]; 
    g[2] =  f[2] + f[3]; 
    g[3] =  f[3] - f[2]; 
    g[4] =  f[4];
    g[5] =  f[5] + f[7]; 
    g[6] =  f[6]; 
    g[7] =  f[7] - f[5]; 

    // Stage 6
    f[0] =  g[0];
    f[1] =  g[1]; 
    f[2] =  g[2];
    f[3] =  g[3]; 
    f[4] =  g[4] + g[7]; 
    f[5] =  g[5] + g[6]; 
    f[6] = -g[6] + g[5]; 
    f[7] =  g[7] - g[4]; 

    // Stage 7
    g[0] = f[0] / 4;
    g[1] = f[5] / 8; 
    g[2] = f[2] / 8;
    g[3] = f[7] / 8; 
    g[4] = f[1] / 8;
    g[5] = f[4] / 8; 
    g[6] = f[3] / 8; 
    g[7] = f[6] / 8; 

#if NORMALIZE_DFT
    s[0] = sqrt(2);
    s[1] = 2.0 / (cos(1.0 * PI / 16.0));        
    s[2] = 2.0 / (cos(2.0 * PI / 16.0));
    s[3] = 2.0 / (cos(3.0 * PI / 16.0));
    s[4] = 2.0 / (cos(4.0 * PI / 16.0));
    s[5] = 2.0 / (cos(5.0 * PI / 16.0));
    s[6] = 2.0 / (cos(6.0 * PI / 16.0));
    s[7] = 2.0 / (cos(7.0 * PI / 16.0));

    // Stage 7
    for (int i = 0; i < 8; i++) 
       out[i] = g[i] * s[i];
#else
    for (int i = 0; i < 8; i++) 
       out[i] = g[i];
#endif

}


void dct_encode(uint8_t* imdata, uint32_t width, double data[8][8],
	const int xpos, const int ypos)
{
    // block = input block. Do not modify.
    double block[8][8] = {};
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            block[i][j] = imdata[(ypos + i) * width + j + xpos];

    double out[8] = {};

    // This calculates the rowise (left/right) transforms
    uint8_t r = 0; 
    uint8_t i = 0;
    for (r = 0; r < 8; r++) {
        dct8((double*) block + r * 8, out, 1, 0); 
        memcpy(&data[r][0], out, 8 * sizeof(double));
    }

    // This calculates the columwise (up/down) transform
    uint8_t c = 0; 
    for (c = 0; c < 8; c++) {
        dct8((double*) data, out, 8, c); 
        for (i = 0; i < 8; i++) 
            data[i][c] = out[i];
    }

#ifdef USE_DCT_INT_ENCODING
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            data[i][j] /= 32.0;
#endif
}

#define __check_rng(F) \
    for (int i = 8; i < 8; i++) {               \
        if (F[i] > 0x8000 || F[i] < -0x8000) {  \
            printf("OUTSIDE OF 16-BIT RANGE: %d", F[i]);  \
        }                                       \
    }                                           \

static inline void
idct8(const pix_t in[8], pix_t out[8], uint8_t w, uint8_t c) {
    pix_t g[8] = {};
    pix_t f[8] = {};

    for (int i = 0; i < 8; i++) 
       g[i] = in[i*w+c];

#if NORMALIZE_DFT
    pix_t s[8] = {};
    s[0] = sqrt(2);
    s[1] = 2.0 / (cos(1.0 * PI / 16.0));        
    s[2] = 2.0 / (cos(2.0 * PI / 16.0));
    s[3] = 2.0 / (cos(3.0 * PI / 16.0));
    s[4] = 2.0 / (cos(4.0 * PI / 16.0));
    s[5] = 2.0 / (cos(5.0 * PI / 16.0));
    s[6] = 2.0 / (cos(6.0 * PI / 16.0));
    s[7] = 2.0 / (cos(7.0 * PI / 16.0));

    for (int i = 0; i < 8; i++) 
       g[i] = g[i] / s[i] * 10;

    // Stage 7
    f[0] = g[0] * 4;
    f[5] = g[1] * 4; 
    f[2] = g[2] * 8;
    f[7] = g[3] * 4; 
    f[1] = g[4] * 8;
    f[4] = g[5] * 4;    // f4 = g4 + g7
    f[3] = g[6] * 8; 
    f[6] = g[7] * 4; 

#else
    // Stage 7
    f[0] = g[0] * IDF_SF * 4;
    f[5] = g[1] * IDF_SF * 4; 
    f[2] = g[2] * IDF_SF * 8;
    f[7] = g[3] * IDF_SF * 4; 
    f[1] = g[4] * IDF_SF * 8;
    f[4] = g[5] * IDF_SF * 4;    // f4 = g4 + g7
    f[3] = g[6] * IDF_SF * 8; 
    f[6] = g[7] * IDF_SF * 4; 
#endif

    // Stage 6
    g[0] =  f[0];
    g[1] =  f[1]; 
    g[2] =  f[2];
    g[3] =  f[3]; 
    g[4] =  (f[4] - f[7]); 
    g[5] =  (f[5] + f[6]); 
    g[6] =  (f[5] - f[6]); 
    g[7] =  (f[4] + f[7]); 

#ifndef USE_DCT_INT_DECODING
    // Stage 5
    f[0] =  g[0];    
    f[1] =  g[1]; 
    f[2] =  (g[2] - g[3]) / 2; 
    f[3] =  (g[2] + g[3]) / 2; 
    f[4] =  g[4];
    f[5] =  (g[5] - g[7]) / 2; 
    f[6] =  g[6]; 
    f[7] =  (g[5] + g[7]) / 2;

    // Stage 4
    g[0] = f[0] / 1;          
    g[1] = f[1] / 1;          
    g[2] = f[2] / 0.707;
    g[3] = f[3] / 1;
    g[5] = f[5] / 0.707; 
    g[6] = ((0.541 + 0.382) * f[6] - 0.382 * f[4]);
    //g[4] = (f[4] + 0.382 * g[6]) / (0.541 + 0.382); 
    g[4] = (1 * f[4] +  0.353 * f[6] - 0.146 * f[4]) / (0.923); 
    g[7] = f[7] / 1;
#else
#if IDF_SF == 8
    // Stage 5
    f[0] =  g[0];    
    f[1] =  g[1]; 
    f[2] =  (g[2] - g[3]); 
    f[3] =  (g[2] + g[3]); 
    f[4] =  g[4];
    f[5] =  (g[5] - g[7]); 
    f[6] =  g[6]; 
    f[7] =  (g[5] + g[7]);

    g[0] = f[0] / 8;          
    g[1] = f[1] / 8;        
    g[3] = f[3] / 16;
    g[7] = f[7] / 16;
    g[2] = f[2] / 12; 
    g[5] = f[5] / 12; 
    g[6] = (7 * f[6] - 3 * f[4]) / 64; 
    g[4] = (8 * f[4] + 3 * f[6] - 1 * f[4]) / 55; 

#elif IDF_SF == 10
    // Stage 5
    f[0] =  g[0];    
    f[1] =  g[1]; 
    f[2] =  (g[2] - g[3]) / 2; 
    f[3] =  (g[2] + g[3]) / 2; 
    f[4] =  g[4];
    f[5] =  (g[5] - g[7]) / 2; 
    f[6] =  g[6]; 
    f[7] =  (g[5] + g[7]) / 2;

    g[0] = f[0] / 10;          
    g[1] = f[1] / 10;        
    g[2] = f[2] / 7;
    g[3] = f[3] / 10;
    g[5] = f[5] / 7;
    g[6] = (9 * f[6] - 4 * f[4]) / 100;
    g[4] = (10 * f[4] + 4 * f[6] - 1 * f[4]) / 90; 
    g[7] = f[7] / 10;
#endif 

#endif

    // Stage 3
    f[0] =  (g[0] + g[1]) / 2; 
    f[1] =  (g[0] - g[1]) / 2; 
    f[2] =  g[2] - g[3]; 
    f[3] =  g[3]; 
    f[4] =  g[4]; 
    f[5] =  g[5]; 
    f[6] =  g[6];
    f[7] =  g[7]; 

    // Stage 2
    g[0] =  (f[0] + f[3]) / 2; 
    g[1] =  (f[1] + f[2]) / 2;
    g[2] =  (f[1] - f[2]) / 2; 
    g[3] =  (f[0] - f[3]) / 2; 
    g[6] =  f[6] - f[7];
    g[5] =  f[5] - g[6]; 
    g[4] =  f[4] - g[5]; 
    g[7] =  f[7]; 

    // Stage 1
    out[0*w+c] =  (g[0] + g[7]) / 2;
    out[1*w+c] =  (g[1] + g[6]) / 2; 
    out[2*w+c] =  (g[2] + g[5]) / 2;
    out[3*w+c] =  (g[3] + g[4]) / 2; 
    out[4*w+c] =  (g[3] - g[4]) / 2; 
    out[5*w+c] =  (g[2] - g[5]) / 2; 
    out[6*w+c] =  (g[1] - g[6]) / 2; 
    out[7*w+c] =  (g[0] - g[7]) / 2; 
}

static void
dct_decode(uint8_t* restrict buf, uint16_t width,
		pix_t data[8][8], int xpos, int ypos)
{
    // This calculates the columwise (up/down) transform
    uint8_t c = 0; 
    for (c = 0; c < 8; c++) 
        idct8((int32_t*) data, (int32_t*) data, 8, c); 

    // This calculates the rowise (left/right) transforms
    uint8_t r = 0; 
    for (r = 0; r < 8; r++) 
        idct8((pix_t*) data + r * 8, &data[r][0], 1, 0); 

    // Copy result to output buffer 
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) {
            int32_t d = data[i][j];
            buf[(ypos + i) * width + j + xpos] = d < 0? 0 : d > 255? 255 : d;
        }
}


/* play with this bit */
void quantize(double dct_buf[8][8])
{
	int x,y;
    return; 
	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	if (x > 7 || y > 7) dct_buf[y][x] = 0.0;
}

uint8_t* compress_channel(uint32_t* outsize, uint8_t* image, uint32_t width, uint32_t height) {
	double dct_buf[8][8];
	pix_t idct_buf[8][8];
    int c, r;
	
    int32_t prev_dcval = 0;
    uint8_t* encoded_data = (uint8_t*) calloc(1, width * height * sizeof(uint16_t));
    uint32_t encoded_data_n = 0;
 
    // compress the image
	for (r=0; r<height/8; r++)
	for (c=0; c<width/8; c++)
	{
        dct_encode(image, width, dct_buf, c*8, r*8); 
    	quantize(dct_buf);
        for (int l = 0; l < 8; l++) {
            for (int m = 0; m < 8; m++) {
                idct_buf[l][m] = (pix_t) dct_buf[l][m];
            }
        }
        encoded_data_n += huff(prev_dcval, idct_buf, encoded_data + encoded_data_n); 
        prev_dcval = idct_buf[0][0];
	}

    *outsize = encoded_data_n;
    return encoded_data; 
}

uint32_t
decompress_channel(uint8_t* restrict dest,
		const uint8_t* restrict compressed_image,
		uint32_t width, uint32_t height)
{
	pix_t idct_buf[8][8];
    uint32_t prev_dcval = 0;
    uint32_t encoded_data_i = 0;
    int r, c; 
    for (r = 0; r < height / 8; r++) {
        for (c = 0; c < width / 8; c++) {
            encoded_data_i += dehuff(prev_dcval, compressed_image + encoded_data_i, idct_buf);
            prev_dcval = idct_buf[0][0]; 
            dct_decode(dest, width, idct_buf, c*8, r*8);
        }
    }
    return encoded_data_i; 
}

#ifdef TEST_JPEG
int main()
{
    // load image from disk 
	tga_image tga;
	load_tga(&tga, "test.tga");

    // compress iamge 
    uint32_t encoded_data_n = 0;
    uint8_t* compressed_image = compress_channel(&encoded_data_n, tga.image_data, tga.width, tga.height); 
   
    // send encoded_data down the wire...
    printf("Total compressed size: %d bytes\n", encoded_data_n); 
    printf("Raw Image Size: %d\n", (tga.width * tga.height));
    printf("Compression Factor: %0.2f\n", (float) encoded_data_n / (tga.width * tga.height));

    // decompress image
    uint8_t* buf = (uint8_t*) calloc(1, tga.width * tga.height * sizeof(uint8_t)); 
    decompress_channel(buf, compressed_image, tga.width, tga.height);

    // decoded the image 
	DONTFAIL( tga_write_mono("out.tga", buf, tga.width, tga.height) );

    // free heap 
	tga_free_buffers(&tga);
    free(buf);
    free(compressed_image);
    
    // exit 
	return 0;
}
#endif 

#define VLI_DC 12
#define VLI_AC 11
#define HUFF_START_PARSE_SYMS 0xffff

/*
 * Given a DC size category, return it's codelength
 * in bits.
 */
static inline uint8_t
get_dc_code_len(uint8_t category) {
    switch (category) {
        case 0:   return 2;
        case 6:   return 4;
        case 7:   return 5;
        case 8:   return 6;
        case 9:   return 7;
        case 10:  return 8;
        case 11:  return 9;
        default:  return 3;
    }
}

/*
 * Given an AC size category, return its codelength in
 * bits
 */
static inline uint8_t
get_ac_code_len(uint8_t run_size) {
    switch (run_size) {
        case 0:   return 4;
        case 1:   return 2;
        case 2:   return 2;
        case 3:   return 3;
        case 4:   return 4;
        case 5:   return 5;
        case 6:   return 7;
        case 7:   return 8;
        case 8:   return 10;
        case 11:  return 4;
        case 12:  return 5;
        case 13:  return 7;
        case 14:  return 9;
        case 15:  return 11;
        case 21:  return 5;
        case 22:  return 8;
        case 23:  return 10;
        case 24:  return 12;
        case 31:  return 6;
        case 32:  return 9;
        case 33:  return 12;
        /* Next Table */
        case 41:  return 6;
        case 42:  return 10;
        case 51:  return 7;
        case 52:  return 11;
        case 61:  return 7;
        case 62:  return 12;
        case 71:  return 8;
        case 72:  return 12;
        case 81:  return 9;
        case 82:  return 15;
        /* Next Table */
        case 91:  return 9;
        case 101: return 9;
        case 111: return 10;
        case 121: return 10;
        /* Next Table*/
        case 131: return 11;
        case 151: return 11;
        default:  return 16;
    }
}

/*
 * Given an AC Luminance codeword, return its index
 * into the luminance table and the length of the
 * given codeword.
 */
static inline int
ac_lht_idx_from_codeword(uint16_t b, int* restrict len) {
    switch(b&0b1100000000000000){
        case 0b0000000000000000: *len = 2; return 1;
        case 0b0100000000000000: *len = 2; return 2;  }
    if    ((b&0b1110000000000000)
            ==0b1000000000000000){*len= 3; return 3;  }
    switch(b&0b1111000000000000){
        case 0b1010000000000000: *len = 4; return 0;
        case 0b1011000000000000: *len = 4; return 4;
        case 0b1100000000000000: *len = 4; return 11; }
    switch(b&0b1111100000000000){
        case 0b1101000000000000: *len = 5; return 5;
        case 0b1101100000000000: *len = 5; return 12;
        case 0b1110000000000000: *len = 5; return 21; }
    switch(b&0b1111110000000000){
        case 0b1110100000000000: *len = 6; return 31;
        case 0b1110110000000000: *len = 6; return 41; }
    switch(b&0b1111111000000000){
        case 0b1111000000000000: *len = 7; return 6;
        case 0b1111001000000000: *len = 7; return 13;
        case 0b1111010000000000: *len = 7; return 51;
        case 0b1111011000000000: *len = 7; return 61; }
    switch(b&0b1111111100000000){
        case 0b1111100000000000: *len = 8; return 7;
        case 0b1111100100000000: *len = 8; return 22;
        case 0b1111101000000000: *len = 8; return 71; }
    switch(b&0b1111111110000000){
        case 0b1111101100000000: *len = 9; return 14;
        case 0b1111101110000000: *len = 9; return 32;
        case 0b1111110000000000: *len = 9; return 81;
        case 0b1111110010000000: *len = 9; return 91;
        case 0b1111110100000000: *len = 9; return 101;}
    switch(b&0b1111111111000000){
        case 0b1111110110000000: *len = 10;return 8;
        case 0b1111110111000000: *len = 10;return 23;
        case 0b1111111000000000: *len = 10;return 42;
        case 0b1111111001000000: *len = 10;return 111;
        case 0b1111111010000000: *len = 10;return 121;}
    switch(b&0b1111111111100000){
        case 0b1111111011000000: *len = 11;return 15;
        case 0b1111111011100000: *len = 11;return 52;
        case 0b1111111100000000: *len = 11;return 131;
        case 0b1111111100100000: *len = 11;return 151;}
    switch(b&0b1111111111110000){
        case 0b1111111101000000: *len = 12;return 24;
        case 0b1111111101010000: *len = 12;return 33;
        case 0b1111111101100000: *len = 12;return 62;
        case 0b1111111101110000: *len = 12;return 72; }
    if    ((b&0b1111111111111110)
            ==0b1111111110000000){*len= 15; return 82; }
    // if we got this far, we're dealing with a 16 bit code
    b &= 0b0000000001111111;    // discard preceding 1s
    *len = 16;
    return ac_lht_idx16[b];
}

/*
 * Given a DC codeword, return it's size / category.
 * Out pointer len will return the length of the codeword.
 */
static inline int
dc_size_from_codeword(uint16_t b, int* restrict len) {
    if    ((b&0b1100000000000000)
            ==0b0000000000000000){*len= 2; return 0;  }
    switch(b&0b1110000000000000){
        case 0b0100000000000000: *len = 3; return 1;
        case 0b0110000000000000: *len = 3; return 2;
        case 0b1000000000000000: *len = 3; return 3;
        case 0b1010000000000000: *len = 3; return 4;
        case 0b1100000000000000: *len = 3; return 5;  }
    if    ((b&0b1111000000000000)
            ==0b1110000000000000){*len= 4; return 6;  }
    if    ((b&0b1111100000000000)
            ==0b1111000000000000){*len= 5; return 7;  }
    if    ((b&0b1111110000000000)
            ==0b1111100000000000){*len= 6; return 8;  }
    if    ((b&0b1111111000000000)
            ==0b1111110000000000){*len= 7; return 9;  }
    if    ((b&0b1111111100000000)
            ==0b1111111000000000){*len= 8; return 10; }
    if    ((b&0b1111111110000000)
            ==0b1111111100000000){*len= 9; return 11; }
    return -1;
}

/*
 * Converts an amplitude into a VLI coded number.
 * @amp: The amplitude
 * @amp_size: Output of the size category for this amplitude.
 * @dc: 12 if this is a DC amplitude, if it's AC, 11. Use #defined
 *      VLI_DC or VLI_AC
 */
static inline int32_t
amp_to_vli(int32_t amp, uint8_t* restrict amp_size, uint8_t category)
{
    if (amp == 0) {
        *amp_size = 0;
        return 0;
    }
    for (*amp_size = 1; (*amp_size) < category; (*amp_size)++) {
        int bound = 1 << (*amp_size);
        if (abs(amp) < bound) {
            if (amp < 0) amp = (bound - 1) + amp;
            return amp;
        }
    }
    // Encountered error!
    exit(-1);
    return -1;
}

/*
 * Converts a VLI coded number into an amplitude.
 */
static inline int32_t
vli_to_amp(uint16_t vli, uint8_t vli_size)
{
    int32_t amp = vli >> (16 - vli_size);

    int sign = (vli & 0x8000)? 1 : -1;
    if (sign < 0) {
        int bound = 1 << vli_size;
        amp = -(bound - 1) + (int32_t) amp;
    }
    return amp;
}


/*
 * Parses an 8x8 DCT block by consecutievely feeding in amplitudes
 * in a zigzag. Outputs intermediate codes and amplitudes to the
 * provided buffers.
 *
 * @val: Set to HUFF_START_PARSE_SYMS to start parsing. Otherwise,
 *       set it to the current amplitude of the DCT block.
 * @codes: Pointer to a parallel buffer of encoded runlengths and sizes.
 * @amps: Pointer to a paralell buffer of VLI encoded amplitudes.
 */
void parse_syms(int32_t val, uint8_t** codes, uint16_t** amps) {
    static uint8_t runlen;
    static uint8_t zeroblocks;

    if (val == HUFF_START_PARSE_SYMS) {
        runlen = 0;
        zeroblocks = 0;
        return;
    }

    if (val == 0) {
        if (++runlen > 15) {
            // too many zeros! Keep track of how many zero blocks
            // we will add once we've reached a nonzero value.
            zeroblocks++;
            runlen = 0;
        }
    }
    else {
        while (zeroblocks > 0) {
            // Add as many zeroblocks as we've encountered.
            **codes = 151;
            (*codes)++;
            zeroblocks--;
        }

        /* Compute size and magnitude representation */
        uint8_t size = 0;
        val = amp_to_vli(val, &size, VLI_AC);

        /* Add the code and amplitude to buffers */
        uint16_t c = (runlen * 10) + size;
        **codes = c;
        (*codes)++;
        **amps = val | (size << 12);
        (*amps)++;
        runlen = 0;
    }
}


/*
 * Given the amount of bits left in a buffer of 16bit words, add a group
 * of len bits specified by word into the buffer.
 *
 * @bits_left: Number of bits free in the current word pointed to by buf.
 * @buf: The buffer in which bits will be inserted.
 * @word: The bits to insert. Will insert MSB first.
 * @len: Number of bits to insert.
 *
 * Returns the number of bits left in the current word after insertion.
 */
uint16_t
add_bits2buf(uint16_t bits_left, uint16_t** buf, uint16_t word, uint8_t len)
{
    if (bits_left >= len) {
        // We can fit the entire word into the current buffer word.
        **buf |= (word << (bits_left - len));
        bits_left -= len;
        if (bits_left == 0) {
            (*buf)++;
            bits_left = 16;
        }
    }
    else {
        // We have to split up the word, fill up the current buffer word,
        // increment the buffer, and start filling up the next buffer word.
        uint16_t upper = word >> (len - bits_left);
        **buf |= upper;
        (*buf)++;
        bits_left = 16 - (len - bits_left);
        **buf |= (word << bits_left);
    }
    return bits_left;
}

/*
 * Given a reference to a pointer buffer and a pointer to a word,
 * read the next 16 bit word at bitoffset from this buffer.
 *
 * @p: Reference to 16bit pointed buffer
 * @word: Reference to word which will be read into
 * @bitoffset: The bitoffset we should read the word from in the
 *              @p pointer
 *
 * Returns the new bitoffset after advancing bitoffset bits.
 */
static inline int
get_word_from_buf(const uint16_t** restrict p, uint16_t* restrict word, int bitoffset)
{
    // If bitoffset too big, go to next word in buffer
    while (bitoffset >= 16) {
        bitoffset -= 16;
        (*p)++;
    }

    // if bitoffset is zero, just return the 16bit word
    if (bitoffset == 0) *word = **p;
    // else, split it up and put it back together
    else  *word = ((**p) << bitoffset) | ((*((*p)+1) >> (16 - bitoffset)));

    return bitoffset;
}


/*
 * Huffman encode an 8x8 DCT block of data with JPEG tables.
 * @prev_dcval: DC value of previous 8x8 block.
 * @b: 8x8 quantized block.
 * @buf: output buffer where huffman encoded data will be written to.
 *
 * Returns size of output buffer in bytes.
 */
uint8_t
huff(int32_t prev_dcval, int32_t b[8][8], uint8_t* buf8)
{
    uint16_t* buf = (uint16_t*) buf8;
    uint16_t* buf_start = buf;
    uint8_t* codes = (uint8_t*) calloc(128, 1);
    uint16_t* amps = (uint16_t*) calloc(128, 1);
    uint8_t* code_i = codes;
    uint16_t* amp_i = amps;

    uint16_t codeword;
    uint8_t codelen;
    int r, c, i;
    int32_t dc_amp;
    uint8_t size;
    uint16_t amp;
    uint8_t code;
    uint8_t bits_left = 16;

    /* Parse the buffer into intermediate Runlen/Size + VLI codes */
    parse_syms(HUFF_START_PARSE_SYMS, NULL, NULL);
    for (i = 1; i < 64; i++) {
        // Loop through the 8x8 block zigzag and parse
        r = row_lin2diag[i];
        c = col_lin2diag[i];
        parse_syms(b[r][c], &code_i, &amp_i);
    }

    /* The following converts intermediate codes to huffman codewords */

    /* Find DC amplitude and category (size) */
    dc_amp = b[0][0] - prev_dcval;
    dc_amp = amp_to_vli((uint32_t) dc_amp, &size, VLI_DC);

    /* Add DC huffman code (VLC) for size, then VLI for amplitude */
    codelen = get_dc_code_len(size);
    codeword = dc_lht_codes[size];
    bits_left = add_bits2buf(bits_left, &buf, codeword, codelen);
    bits_left = add_bits2buf(bits_left, &buf, dc_amp, size);

    uint8_t* p = codes - 1;     // point to start of codes
    uint16_t* a = amps - 1;     // point to start of amps
    while (p != code_i) {       // loop until we've processed all codes

        /* Add AC huffman code (VLC) for size, then VLI for amplitude */
        code = *(++p); // index into table
        codelen = get_ac_code_len(code);
        codeword = ac_lht_codes[code];
        bits_left = add_bits2buf(bits_left, &buf, codeword, codelen);

        // skip EOB and ZRL codes (because they don't have amps)
        if (code != 0 && code != 151) {
            amp = (*(++a));
            size = amp >> 12;
            amp &= 0x0fff;
            bits_left = add_bits2buf(bits_left, &buf, amp, size);
        }
    }
    free(codes);
    free(amps);


    // Return sizeof buf buffer (in bytes).
    return (buf - buf_start +1) * 2;
}


/*
 * Decodes a given huffman encoded 8x8 block.
 *
 * @prev_dcval: The previous DC value that came before this block.
 * @buf: The 16bit word buffer that holds the huffman encoded bitstring.
 * @outb: The 8x8 output array to which the decoded DCT will be written to.
 *
 * Returns the amount of data in bytes written to buf.
 */
static uint32_t
dehuff(int32_t prev_dcval, const uint8_t* restrict buf8, int32_t outb[8][8])
{
    const uint16_t* restrict buf = (const uint16_t* restrict) buf8;
    uint8_t bitoffset = 0;
    uint16_t word = 0;
    int len = 0;
    int idx = 0;
    int rl = 0;
    int size;
    int32_t amp = 0;
    uint8_t lin_idx = 1;
    uint8_t dig_row;
    uint8_t dig_col;
    const uint16_t* restrict buf_s = buf;

    memset(outb, 0, 64 * sizeof(int32_t));

    // Read DC codeword
    bitoffset = get_word_from_buf(
    		(const uint16_t** restrict) &buf,
    		&word, bitoffset);
    size = dc_size_from_codeword(word, &len);
    bitoffset += len;

    // Read DC amplitude
    bitoffset = get_word_from_buf(
    		(const uint16_t** restrict) &buf,
    		&word, bitoffset);
    bitoffset += size;
    outb[0][0] = prev_dcval + vli_to_amp(word, size);

    int codes_n = 127;
    while (codes_n-- != 0) {
        // Get huff codeword
        bitoffset = get_word_from_buf(
        		(const uint16_t** restrict) &buf,
        		&word, bitoffset);
        idx = ac_lht_idx_from_codeword(word, &len);
        bitoffset += len;

        // If it's zero, we're done (EOB)
        if (idx == 0) break;

        // Extract runlength and amp size from index
        rl = idx / 10;
        size = idx % 10;
        if (size == 0) {
            rl--;
            size = 10;
        }

        amp = 0;
        if (idx != 151) {
            // If not a Zeroblock (ZRL), get amplitude
            bitoffset = get_word_from_buf(
            		(const uint16_t** restrict) &buf,
            		&word, bitoffset);
            amp = vli_to_amp(word, size);
            bitoffset += size;
        }

        // at this point we've decoded the next zero runlength and
        // the amplitude following this runlength. Skip RL indexes
        // and then place amplitude.
        lin_idx += rl;
        if (lin_idx >= 64) break;
        dig_row = row_lin2diag[lin_idx];
        dig_col = col_lin2diag[lin_idx];
        outb[dig_row][dig_col] = amp;
        lin_idx++;
    }
    if (bitoffset >= 16) buf++;
    return (buf - buf_s + 1) * 2;
}
