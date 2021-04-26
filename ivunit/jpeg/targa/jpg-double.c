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
#if 1
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
#if 1
    s[0] = sqrt(2);
    s[1] = 2.0 / (cos(1.0 * PI / 16.0));        
    s[2] = 2.0 / (cos(2.0 * PI / 16.0));
    s[3] = 2.0 / (cos(3.0 * PI / 16.0));
    s[4] = 2.0 / (cos(4.0 * PI / 16.0));
    s[5] = 2.0 / (cos(5.0 * PI / 16.0));
    s[6] = 2.0 / (cos(6.0 * PI / 16.0));
    s[7] = 2.0 / (cos(7.0 * PI / 16.0));
#else
    s[0] = 1;
    s[1] = 1;
    s[2] = 1;
    s[3] = 1;
    s[4] = 1;
    s[5] = 1;
    s[6] = 1;
    s[7] = 1;
#endif

    // Stage 7
    for (int i = 0; i < 8; i++) 
       out[i] = g[i] * s[i];

}

#define USE_INTS

void idct8(double in[8], double out[8], uint8_t w, uint8_t c) {
    double s[8] = {};
    double g[8] = {};
    double f[8] = {};
    double f_8; 

#define PI 3.14159
#ifndef USE_INTS
    s[0] = sqrt(2);
    s[1] = 2.0 / (cos(1.0 * PI / 16.0));        
    s[2] = 2.0 / (cos(2.0 * PI / 16.0));
    s[3] = 2.0 / (cos(3.0 * PI / 16.0));
    s[4] = 2.0 / (cos(4.0 * PI / 16.0));
    s[5] = 2.0 / (cos(5.0 * PI / 16.0));
    s[6] = 2.0 / (cos(6.0 * PI / 16.0));
    s[7] = 2.0 / (cos(7.0 * PI / 16.0));
#else
    s[0] = sqrt(2) /10;
    s[1] = .2 / (cos(1.0 * PI / 16.0));        
    s[2] = .2 / (cos(2.0 * PI / 16.0));
    s[3] = .2 / (cos(3.0 * PI / 16.0));
    s[4] = .2 / (cos(4.0 * PI / 16.0));
    s[5] = .2 / (cos(5.0 * PI / 16.0));
    s[6] = .2 / (cos(6.0 * PI / 16.0));
    s[7] = .2 / (cos(7.0 * PI / 16.0));
#endif

    for (int i = 0; i < 8; i++) 
       g[i] = in[i*w+c];

    printf("IDCT8 IN:\n");
    for (int k = 0; k < 8; k++) printf("%6.1f ", g[k]); 
    putchar('\n');

    
    for (int i = 0; i < 8; i++) 
       g[i] = g[i] / s[i];

    // Stage 7
    f[0] = g[0] * 4;
    f[5] = g[1] * 4; 
    f[2] = g[2] * 8;
    f[7] = g[3] * 4; 
    f[1] = g[4] * 8;
    f[4] = g[5] * 4;    // f4 = g4 + g7
    f[3] = g[6] * 8; 
    f[6] = g[7] * 4; 

    // Stage 6
    g[0] =  f[0];
    g[1] =  f[1]; 
    g[2] =  f[2];
    g[3] =  f[3]; 
    g[4] =  (f[4] - f[7]); 
    g[5] =  (f[5] + f[6]); 
    g[6] =  (f[5] - f[6]); 
    g[7] =  (f[4] + f[7]); 

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
#ifndef USE_INTS
    g[0] = f[0] / 1;          // *8
    g[1] = f[1] / 1;          // *8
    g[2] = f[2] / 0.707;
    g[3] = f[3] / 1;
    g[5] = f[5] / 0.707; 
    g[6] = ((0.541 + 0.382) * f[6] - 0.382 * f[4]);
    g[4] = (f[4] + 0.382 * g[6]) / (0.541 + 0.382); 
    g[7] = f[7] / 1;
#else
    g[0] = f[0] / 10;          
    g[1] = f[1] / 10;        
    g[2] = f[2] / 7;
    g[3] = f[3] / 10;
    g[5] = f[5] / 7;
    g[6] = (9 * f[6] - 4 * f[4]) / 100;
    g[4] = (10 * f[4] + 4 * g[6]) / 90; 
    g[7] = f[7] / 10;
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
    out[0] =  (g[0] + g[7]) / 2;
    out[1] =  (g[1] + g[6]) / 2; 
    out[2] =  (g[2] + g[5]) / 2;
    out[3] =  (g[3] + g[4]) / 2; 
    out[4] =  (g[3] - g[4]) / 2; 
    out[5] =  (g[2] - g[5]) / 2; 
    out[6] =  (g[1] - g[6]) / 2; 
    out[7] =  (g[0] - g[7]) / 2; 
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
}

void idct_ii(const tga_image *tga, double data[8][8],
	const int xpos, const int ypos)
{
    double block[8][8] = {};
    double out[8] = {};
    //printf("sst: %f", data[0][2]);

    uint8_t i = 0;
#if 1
    // This calculates the columwise (up/down) transform
    uint8_t c = 0; 
    for (c = 0; c < 8; c++) {
        idct8(data, out, 8, c); 
        printf("OUT: "); 
        for (i = 0; i < 8; i++)  {
            double v = out[i]; 
            printf("%6.1f", v);
            block[i][c] = v;
        }
    }
    putchar('\n');

    printf("BLOCK\n"); 
    for (int o = 0; o < 8; o++) {
        for (int r = 0; r <8; r++) {
            printf("%6.1f", block[o][r]); 
        }
        printf("\n");
    }
#endif

#if 1
    // This calculates the rowise (left/right) transforms
    uint8_t r = 0; 
    for (r = 0; r < 8; r++) {
        idct8((double*) block + r * 8, out, 1, 0); 
        for (i = 0; i < 8; i++)  {
            double v = out[i]; 
            if (v < 0) v = 0; 
            if (v > 255) v = 255;
            out[i] = v;
        }
     
        printf("\nOUT\n");
        for (int k = 0; k < 8; k++) printf("%6.1f ", out[k]); 
        memcpy(&data[r][0], out, 8 * sizeof(double));
    }
    printf("\nDATA\n"); 
    for (int o = 0; o < 8; o++) {
        for (int r = 0; r <8; r++) {
            printf("%6.1f", data[o][r]); 
        }
        printf("\n");
    }

#endif

    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            tga->image_data[(ypos + i) * tga->width + j + xpos] = (uint8_t) ((int) data[i][j]);

 
}


void dct(const tga_image *tga, double data[8][8],
	const int xpos, const int ypos)
{
	int u,v,x,y;
	
	for (v=0; v<8; v++)
	for (u=0; u<8; u++)
	{
		double Cu, Cv, z = 0.0;

		COEFFS(Cu,Cv,u,v);

		for (y=0; y<8; y++)
		for (x=0; x<8; x++)
		{
			double s, q;

			s = pixel(tga, x+xpos, y+ypos);

			q = s * cos((double)(2*x+1) * (double)u * PI/16.0) *
				cos((double)(2*y+1) * (double)v * PI/16.0);

			z += q;
		}

		data[v][u] = (int) (0.25 * Cu * Cv * z);
	}
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



void idct(tga_image *tga, double data[8][8],
	const int xpos, const int ypos)
{
	int u,v,x,y;

#if 0
	/* show the frequency data */
	double lo=0, hi=0;
	if (fabs(hi) > fabs(lo))
		lo = -hi;
	else
		hi = -lo;

	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	{
		byte z = (byte)( (data[y*8 + x] + 1024.0) / 2048.0 * 255.0);
		put_pixel(im, x+xpos, y+ypos, z);
	}

#else
	/* iDCT */
	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	{
		double z = 0.0;

		for (v=0; v<8; v++)
		for (u=0; u<8; u++)
		{
			double S, q;
			double Cu, Cv;
			
			COEFFS(Cu,Cv,u,v);
			S = data[v][u];

			q = Cu * Cv * S *
				cos((double)(2*x+1) * (double)u * PI/16.0) *
				cos((double)(2*y+1) * (double)v * PI/16.0);

			z += q;
		}

		z /= 4.0;
		if (z > 255.0) z = 255.0;
		if (z < 0) z = 0.0;

		pixel(tga, x+xpos, y+ypos) = (uint8_t) z;
	}
#endif
}



int main()
{
	tga_image tga;
	double dct_buf[8][8];
	double idct_buf[8][8];
	int i, j, k, l;

    double t[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    double o[8] = {0};
    for (i = 0; i < 8; i++) printf("%6.1f ", t[i]); 
    putchar('\n');
    dct8(t, o, 1, 0); 
    for (i = 0; i < 8; i++) printf("%6.1f ", o[i]); 
    putchar('\n');
    idct8(o, t, 1, 0); 
    for (i = 0; i < 8; i++) printf("%6.1f ", t[i]); 
    putchar('\n');


	load_tga(&tga, "im8.tga");

    for (i = 0; i < tga.height; i++) {
        for (j = 0; j < tga.width; j++) {
            //uint32_t d = tga.image_data[jj
            tga.image_data[i * tga.height + j] /= 16; 
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
        dct_ii(&tga, idct_buf, i*8, j*8); 
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                printf("%6d ", idct_buf[x][y]);
            }
            putchar('\n');
        }
        for (int l = 0; l < 8; l++) {
            for (int m = 0; m < 8; m++) {
                dct_buf[l][m] = (double) idct_buf[l][m];
            }
        }
	
        printf("\nDCTBUF");
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                printf("%6.2f ", dct_buf[x][y]);
            }
            putchar('\n');
        }
		quantize(dct_buf);
        /*for (int l = 0; l < 8; l++) {
            for (int m = 0; m < 8; m++) {
                double v = dct_buf[l][m];
                dct_buf[l][m] = (double) v;
                v = v < 0? -v : v;
                if (v == 1) bits++;
                if (v >= 2 && v <= 3) bits += 2; 
                if (v >= 4 && v <= 7) bits += 3; 
                if (v >= 8 && v <= 15) bits += 4;
                if (v >= 16 && v <= 31) bits += 5; 
                if (v >= 32 && v <= 63) bits += 6; 
                if (v >= 64 && v <= 127) bits += 7; 
            }
        }*/
	
		idct_ii(&tga, dct_buf, i*8, j*8);
        printf("\nIDCTBUF");
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                printf("%d ", tga.image_data[x + 8*y]);
            }
            putchar('\n');
        }
	
		printf("processed %d/%d blocks.\r", ++k,l);
		fflush(stdout);
	}
    printf("%d non-zero coefficients", cof);
    printf("%d data RLE bytes", bits / 8); 
	printf("\n");

    for (i = 0; i < tga.height; i++) {
        for (j = 0; j < tga.width; j++) {
            //uint32_t d = tga.image_data[jj
            tga.image_data[i * tga.height + j] *= 16; 
            printf("%4d ", tga.image_data[i * tga.height + j]);
        }
        printf(";\n");
    }


	DONTFAIL( tga_write_mono("out.tga", tga.image_data,
		tga.width, tga.height) );

	tga_free_buffers(&tga);
    printf("RAN DOUBLE");
	return EXIT_SUCCESS;
}
