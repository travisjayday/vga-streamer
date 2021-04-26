#include "debug.h"
#include "test_img.c"
#include "test_img2.c"
#include "test_img3.c"
#include "rec.c"
#include "math.h"

uint8_t Q50[8][8] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92, 
    49, 64, 78, 87, 103, 121, 120, 101, 
    72, 92, 95, 98, 112, 100, 103, 99
};

void aan_dct(double i[]);

void dct(int32_t* arr) {

    double f[8] = {};
    double g[8] = {};
    double s[8] = {};
    double f_8; 

    for (int i = 0; i < 8; i++)  {
        f[i] = ((double) arr[i]); 
    }

    // Stage 1
    g[0] =  f[0] + f[7];
    g[1] =  f[1] + f[6]; 
    g[2] =  f[2] + f[5];
    g[3] =  f[3] + f[4]; 
    g[4] = -f[4] + f[3]; 
    g[5] = -f[5] + f[2]; 
    g[6] = -f[6] + f[1];
    g[7] = -f[7] + f[0]; 

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

#if 1 
    f_8  = -g[6] + g[4]; 
    f_8  =   0.382 * f_8; 

    // Stage 4
    f[0] =  1  *g[0];          // *8
    f[1] =  1  *g[1];          // *8
    f[2] =  0.707  *g[2];           // *6
    f[3] =  1  *g[3];          // *8
    f[4] =  0.541  *g[4] + f_8;    // *4
    f[5] =  0.707  *g[5];               // *6
    f[6] =  1.306 *g[6] + f_8;    // *10
    f[7] =  1  *g[7] ;          // *8*/
#else 
    f_8  = -g[6] + g[4]; 
    f_8  =  5 * f_8; 

    f[0] =  13  *g[0];          // *8
    f[1] =  13  *g[1];          // *8
    f[2] =  9  *g[2];           // *6
    f[3] =  13  *g[3];          // *8
    f[4] =  7  *g[4] + f_8;    // *4
    f[5] =  9  *g[5];               // *6
    f[6] =  17  *g[6] + f_8;    // *10
    f[7] =  13  *g[7] ;          // *8*/
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

    printf("LAST stage: %f", f[7]);

#define PI 3.14159
    s[0] = sqrt(2);
    s[1] = 2.0 / (cos(1.0 * PI / 16.0));        
    s[2] = 2.0 / (cos(2.0 * PI / 16.0));
    s[3] = 2.0 / (cos(3.0 * PI / 16.0));
    s[4] = 2.0 / (cos(4.0 * PI / 16.0));
    s[5] = 2.0 / (cos(5.0 * PI / 16.0));
    s[6] = 2.0 / (cos(6.0 * PI / 16.0));
    s[7] = 2.0 / (cos(7.0 * PI / 16.0));

    // Stage 7
    for (int i = 0; i < 8; i++) {
       g[i] *= s[i];
    }

    printf("\n\n");
    for (int i = 0; i < 8; i++) 
        printf("%f,", g[i]);
        //arr[i] = f[i];
}

void aan_dct(double i[]) {
    //for (int k = 0; k < 8; k++) 
        //i[k] *= 64; 

#if 1
  const double a1 = 8 * sqrt(.5);
  const double a2 = 8 * sqrt(2.) * cos(3./16. * 2 * M_PI);
  const double a3 = a1;
  const double a4 = 8 * sqrt(2.) * cos(1./16. * 2 * M_PI);
  const double a5 = 8 * cos(3./16. * 2 * M_PI);
#else
  const double a1 = 0.707;
  const double a2 = 0.541;
  const double a3 = 0.707;
  const double a4 = 1.307;
  const double a5 = 0.383;
#endif

  double b0 = i[0] + i[7];
  double b1 = i[1] + i[6];
  double b2 = i[2] + i[5];
  double b3 = i[3] + i[4];
  double b4 =-i[4] + i[3];
  double b5 =-i[5] + i[2];
  double b6 =-i[6] + i[1];
  double b7 =-i[7] + i[0];

  double c0 = b0 + b3;
  double c1 = b1 + b2;
  double c2 =-b2 + b1;
  double c3 =-b3 + b0;
  double c4 =-b4 - b5;
  double c5 = b5 + b6;
  double c6 = b6 + b7;
  double c7 = b7;

  double d0 = c0 + c1;
  double d1 =-c1 + c0;
  double d2 = c2 + c3;
  double d3 = c3;
  double d4 = c4;
  double d5 = c5;
  double d6 = c6;
  double d7 = c7;

  double d8 = (d4 + d6) * a5;

  double e0 = d0;
  double e1 = d1;
  double e2 = d2 * a1;
  double e3 = d3;
  double e4 = -d4 * a2 - d8;
  double e5 = d5 * a3;
  double e6 = d6 * a4 - d8;
  double e7 = d7;

  double f0 = e0;
  double f1 = e1;
  double f2 = e2 + e3;
  double f3 = e3 - e2;
  double f4 = e4;
  double f5 = e5 + e7;
  double f6 = e6;
  double f7 = e7 - e5;

  double g0 = f0;
  double g1 = f1;
  double g2 = f2;
  double g3 = f3;
  double g4 = f4 + f7;
  double g5 = f5 + f6;
  double g6 = -f6 + f5;
  double g7 = f7 - f4;

    printf("LAST stage: %f", g7);

#if 1
  const double s0 = (cos(0)*sqrt(.5)/2)/(1       );  // 0.353553
  const double s1 = (cos(1.*M_PI/16)/2)/(-a5+a4+1);  // 0.254898
  const double s2 = (cos(2.*M_PI/16)/2)/(a1+1    );  // 0.270598
  const double s3 = (cos(3.*M_PI/16)/2)/(a5+1    );  // 0.300672
  const double s4 = s0;  // (cos(4.*M_PI/16)/2)/(1       );
  const double s5 = (cos(5.*M_PI/16)/2)/(1-a5    );  // 0.449988
  const double s6 = (cos(6.*M_PI/16)/2)/(1-a1    );  // 0.653281
  const double s7 = (cos(7.*M_PI/16)/2)/(a5-a4+1 );  // 1.281458
#else
  const double s0 = 0.353553;
  const double s1 = 0.254898;
  const double s2 = 0.270598;
  const double s3 = 0.300672;
  const double s4 = 0.353553;
  const double s5 = 0.449988;
  const double s6 = 0.653281;
  const double s7 = 1.281458;
#endif

  i[0] = g0 * s0;
  i[4] = g1 * s4;
  i[2] = g2 * s2;
  i[6] = g3 * s6;
  i[5] = g4 * s5;
  i[1] = g5 * s1;
  i[7] = g6 * s7;
  i[3] = g7 * s3;

}

double quanitze_row(double items) {
}

void 
jencode(uint8_t* pixbuf) 
{
    show_vga(pixbuf);
}

int 
main() 
{
    //show_vga(rec);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 ; j++) {
            printf("%d ", Q50[i][j]);
        }
        printf("\n");
    }
    //uint32_t arr[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    //uint32_t arr[8] = {8, 9, 10, 11, 12, 13, 14, 15};
    //uint32_t arr[8] = {0x00080008, 0x00090009, 0x000A000A, 0x000B000B, 0x000C000C, 0x000D000D, 0x000E000E, 0x000F000F};
    //for (int i = 0; i < 8; i++) arr[i] = 0x00ff00ff;
    //uint32_t arr[8] = {52, 55, 61, 66, 70, 61, 64, 73};
    //int32_t arr[8] = {-76, -73, -67, -62, -58, -67, -64, -55};
    int32_t arr[8] = {12, 10, 10, 11, 9, 9, 10, 12};

    dct(arr); 
    putchar('\n');
    //jencode(test_img);
}
