/* DCT and IDCT - listing 1
 * Copyright (c) 2001 Emil Mikulic.
 * http://unix4lyfe.org/dct/
 *
 * Feel free to do whatever you like with this code.
 * Feel free to credit me.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "targa.h"
#include "string.h"


typedef uint8_t byte;

#define pixel(i,x,y) ( (i)->image_data[((y)*( (i)->width ))+(x)] )

#define DONTFAIL(x) do { tga_result res;  if ((res=x) != TGA_NOERR) { \
	printf("Targa error: %s\n", tga_error(res)); \
	exit(EXIT_FAILURE); } } while(0)



void load_tga(tga_image *tga, const char *fn)
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



#ifndef PI
 #ifdef M_PI
  #define PI M_PI
 #else
  #define PI 3.14159265358979
 #endif
#endif



/* S[u,v] = 1/4 * C[u] * C[v] *
 *   sum for x=0 to width-1 of
 *   sum for y=0 to height-1 of
 *     s[x,y] * cos( (2x+1)*u*PI / 2N ) * cos( (2y+1)*v*PI / 2N )
 *
 * C[u], C[v] = 1/sqrt(2) for u, v = 0
 * otherwise, C[u], C[v] = 1
 *
 * S[u,v] ranges from -2^10 to 2^10
 */

#define COEFFS(Cu,Cv,u,v) { \
	if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0; \
	if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0; \
	}

//#define USE_DCT_INT
//
#define NORMALIZE_DFT 0

/* 
 * w, c: width of block. For columnwise DFT, use w=8, c=colum
 *       For row-wise DCT, use w=0, c=0. 
 */
void dct8(double b[8], double out[8], uint8_t w, uint8_t c) {
    double s[8] = {};
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
#ifndef USE_DCT_INT
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

#define PI 3.14159
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


void dct_ii(const tga_image *tga, double data[8][8],
	const int xpos, const int ypos)
{
    // block = input block .Do not modify.
    double block[8][8] = {};
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            block[i][j] = tga->image_data[(ypos + i) * tga->width + j + xpos];

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
        dct8(data, out, 8, c); 
        for (i = 0; i < 8; i++) 
            data[i][c] = out[i];
    }
    putchar('\n');

#ifdef USE_DCT_INT
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            data[i][j] /= 32.0;
#endif
}

#define USE_INTS
#define IDF_SF 8

#define PI 3.14159

#ifdef USE_INTS
#define pix_t int32_t
#else
#define pix_t double
#endif

#define __check_rng(F) \
    for (int i = 8; i < 8; i++) {               \
        if (F[i] > 0x8000 || F[i] < -0x8000) {  \
            printf("OUTSIDE OF 16-BIT RANGE: %d", F[i]);  \
        }                                       \
    }                                           \

void idct8(pix_t in[8], pix_t out[8], uint8_t w, uint8_t c) {
    pix_t g[8] = {};
    pix_t f[8] = {};
    pix_t f_8; 

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

#ifndef USE_INTS
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

void idct_ii(uint8_t* buf, uint16_t width, pix_t data[8][8],
	const int xpos, const int ypos)
{
    // This calculates the columwise (up/down) transform
    uint8_t c = 0; 
    for (c = 0; c < 8; c++) 
        idct8(data, data, 8, c); 

    // This calculates the rowise (left/right) transforms
    uint8_t r = 0; 
    for (r = 0; r < 8; r++) 
        idct8((pix_t*) data + r * 8, &data[r][0], 1, 0); 

    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            buf[(ypos + i) * width + j + xpos] = (uint8_t) ((int) data[i][j]);
}


/* play with this bit */
void quantize(double dct_buf[8][8])
{
	int x,y;
    return;
	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	if (x > 3 || y > 3) dct_buf[y][x] = 0.0;
}

void huffman_encode(uint8_t* buf) {
    
}


int main()
{
	tga_image tga;
	double dct_buf[8][8];
	pix_t idct_buf[8][8];
	int i, j, k, l;

	load_tga(&tga, "lenna.tga");
    uint8_t* buf = (uint8_t*) malloc(tga.width * tga.height * sizeof(uint8_t)); 

    for (i = 0; i < tga.height; i++) {
        for (j = 0; j < tga.width; j++) {
            printf("%4d ", tga.image_data[i * tga.height + j]);
        }
        printf(";\n");
    }

	k = 0;
	l = (tga.height / 8) * (tga.width / 8);
    int cof = 0;
    int bits = 0;
	for (j=0; j<tga.height/8; j++)
	for (i=0; i<tga.width/8; i++)
	{
        dct_ii(&tga, dct_buf, i*8, j*8); 
    	quantize(dct_buf);
        for (int l = 0; l < 8; l++) {
            for (int m = 0; m < 8; m++) {
                idct_buf[l][m] = (pix_t) dct_buf[l][m];
            }
        }
	
        // sending idct_buf over the wire...
        for (int l = 0; l < 8; l++) {
            for (int m = 0; m < 8; m++) {
                pix_t v = idct_buf[l][m];
                v = v < 0? -v : v;
                if (v == 1) bits++;
                if (v >= 2 && v <= 3) bits += 2; 
                if (v >= 4 && v <= 7) bits += 3; 
                if (v >= 8 && v <= 15) bits += 4;
                if (v >= 16 && v <= 31) bits += 5; 
                if (v >= 32 && v <= 63) bits += 6; 
                if (v >= 64 && v <= 127) bits += 7; 
            }
        }
	
		idct_ii(buf, tga.width, idct_buf, i*8, j*8);
		printf("processed %d/%d blocks.\r", ++k,l);
		fflush(stdout);
	}
    printf("%d non-zero coefficients", cof);
    printf("%d data RLE bytes", bits / 8); 
	printf("\n");

	DONTFAIL( tga_write_mono("out.tga", buf,
		tga.width, tga.height) );

	tga_free_buffers(&tga);
	return EXIT_SUCCESS;
}