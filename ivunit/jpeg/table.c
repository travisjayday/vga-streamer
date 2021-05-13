#include "jpg.h"

const uint16_t ac_lht_codes[] =  { 0b1010,    /* 0/0 -> 0x00 */
0b00,                  /* 0/1 -> 0x01 */   // idx=1
0b01,                  /* 0/2 -> 0x02 */   // idx=2
0b100,                 /* ... */           // idx=3
0b1011,                // idx=4
0b11010,               // idx=5
0b1111000,             // idx=6
0b11111000,            // idx=7
0b1111110110,          // idx=8
0b1111111110000010,    // idx=9
0b1111111110000011,    /* 0/A -> 0x0A (10) */     // idx=10
0b1100,                /* 1/1 -> 0x0B (11) */    // idx=11
0b11011,               // idx=12
0b1111001,             // idx=13
0b111110110,           // idx=14
0b11111110110,         // idx=15
0b1111111110000100,    /* 1/6 -> 0x10 (16) */     // idx=16
0b1111111110000101,    // idx=17
0b1111111110000110,    // idx=18
0b1111111110000111,    // idx=19
0b1111111110001000,    /* 1/A -> 0x14 (20) */     // idx=20
0b11100,               /* 2/1 -> 0x15 (21) */     // idx=21
0b11111001,            // idx=22
0b1111110111,          // idx=23
0b111111110100,        // idx=24
0b1111111110001001,    // idx=25
0b1111111110001010,    // idx=26
0b1111111110001011,    // idx=27
0b1111111110001100,    // idx=28
0b1111111110001101,    // idx=29
0b1111111110001110,    // idx=30
0b111010,              // idx=31
0b111110111,           // idx=32
0b111111110101,        // idx=33
0b1111111110001111,    // idx=34
0b1111111110010000,    // idx=35
0b1111111110010001,    // idx=36
0b1111111110010010,    // idx=37
0b1111111110010011,    // idx=38
0b1111111110010100,    // idx=39
0b1111111110010101,    /* 3/A -> 0x3A (40) */    // idx=40
0b111011,              /* 4/1 -> 0x41 */     // idx=41
0b1111111000,          // idx=42
0b1111111110010110,    // idx=43
0b1111111110010111,    // idx=44
0b1111111110011000,    // idx=45
0b1111111110011001,    // idx=46
0b1111111110011010,    // idx=47
0b1111111110011011,    // idx=48
0b1111111110011100,    // idx=49
0b1111111110011101,    // idx=50
0b1111010,             // idx=51
0b11111110111,         // idx=52
0b1111111110011110,    // idx=53
0b1111111110011111,    // idx=54
0b1111111110100000,    // idx=55
0b1111111110100001,    // idx=56
0b1111111110100010,    // idx=57
0b1111111110100011,    // idx=58
0b1111111110100100,    // idx=59
0b1111111110100101,    // idx=60
0b1111011,             // idx=61
0b111111110110,        // idx=62
0b1111111110100110,    // idx=63
0b1111111110100111,    // idx=64
0b1111111110101000,    // idx=65
0b1111111110101001,    // idx=66
0b1111111110101010,    // idx=67
0b1111111110101011,    // idx=68
0b1111111110101100,    // idx=69
0b1111111110101101,    // idx=70
0b11111010,            // idx=71
0b111111110111,        // idx=72
0b1111111110101110,    // idx=73
0b1111111110101111,    // idx=74
0b1111111110110000,    // idx=75
0b1111111110110001,    // idx=76
0b1111111110110010,    // idx=77
0b1111111110110011,    // idx=78
0b1111111110110100,    // idx=79
0b1111111110110101,    // idx=80
0b111111000,           // idx=81
0b111111111000000,     /* 8/2 -> 0x82 (82) */    // idx=82
0b1111111110110110,    /* 8/3 -> 0x83 */    // idx=83
0b1111111110110111,    // idx=84
0b1111111110111000,    // idx=85
0b1111111110111001,    // idx=86
0b1111111110111010,    // idx=87
0b1111111110111011,    // idx=88
0b1111111110111100,    // idx=89
0b1111111110111101,    // idx=90
0b111111001,           // idx=91
0b1111111110111110,    // idx=92
0b1111111110111111,    // idx=93
0b1111111111000000,    // idx=94
0b1111111111000001,    // idx=95
0b1111111111000010,    // idx=96
0b1111111111000011,    // idx=97
0b1111111111000100,    // idx=98
0b1111111111000101,    // idx=99
0b1111111111000110,    // idx=100
0b111111010,           // idx=101
0b1111111111000111,    // idx=102
0b1111111111001000,    // idx=103
0b1111111111001001,    // idx=104
0b1111111111001010,    // idx=105
0b1111111111001011,    // idx=106
0b1111111111001100,    // idx=107
0b1111111111001101,    // idx=108
0b1111111111001110,    // idx=109
0b1111111111001111,    // idx=110
0b1111111001,          // idx=111
0b1111111111010000,    // idx=112
0b1111111111010001,    // idx=113
0b1111111111010010,    // idx=114
0b1111111111010011,    // idx=115
0b1111111111010100,    // idx=116
0b1111111111010101,    // idx=117
0b1111111111010110,    // idx=118
0b1111111111010111,    // idx=119
0b1111111111011000,    // idx=120
0b1111111010,          // idx=121
0b1111111111011001,    // idx=122
0b1111111111011010,    // idx=123
0b1111111111011011,    /* C/4 -> 0xC4 (124) */    // idx=124
0b1111111111011100,    /* C/5 -> 0xC5 */    // idx=125
0b1111111111011101,    // idx=126
0b1111111111011110,    // idx=127
0b1111111111011111,    // idx=128
0b1111111111100000,    // idx=129
0b1111111111100001,    // idx=130
0b11111111000,         // idx=131
0b1111111111100010,    // idx=132
0b1111111111100011,    // idx=133
0b1111111111100100,    // idx=134
0b1111111111100101,    // idx=135
0b1111111111100110,    // idx=136
0b1111111111100111,    // idx=137
0b1111111111101000,    // idx=138
0b1111111111101001,    // idx=139
0b1111111111101010,    // idx=140
0b1111111111101011,    // idx=141
0b1111111111101100,    // idx=142
0b1111111111101101,    // idx=143
0b1111111111101110,    // idx=144
0b1111111111101111,    // idx=145
0b1111111111110000,    // idx=146
0b1111111111110001,    // idx=147
0b1111111111110010,    // idx=148
0b1111111111110011,    // idx=149
0b1111111111110100,    /* E/A -> 0xEA (150) */     // idx=150
0b11111111001,         /* F/0 -> 0XF0 (151) */     // idx=151
0b1111111111110101,    // idx=152
0b1111111111110110,    // idx=153
0b1111111111110111,    // idx=154
0b1111111111111000,    // idx=155
0b1111111111111001,    // idx=156
0b1111111111111010,    // idx=157
0b1111111111111011,    // idx=158
0b1111111111111100,    // idx=159
0b1111111111111101,    // idx=160
0b1111111111111110,    /* F/A -> 0xFA (161) */    // idx=161
};    


const uint16_t dc_lht_codes[] =  { 
0b00,                   // idx=0
0b010,                  // idx=1
0b011,                  // idx=2
0b100,                  // idx=3
0b101,                  // idx=4
0b110,                  // idx=5
0b1110,                 // idx=6
0b11110,                // idx=7
0b111110,               // idx=8
0b1111110,              // idx=9
0b11111110,             // idx=10
0b111111110             // idx=11
};

const uint8_t col_lin2diag[] = {
    0, 1, 0, 2, 1, 0, 3, 2, 1, 0, 4, 3, 2, 1, 0, 5, 4, 3, 2, 
    1, 0, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 
    5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 7, 6, 5, 4, 3, 7, 6, 5, 
    4, 7, 6, 5, 7, 6, 7
};

const uint8_t row_lin2diag[] = {
    0, 0, 1, 0, 1, 2, 0, 1, 2, 3, 0, 1, 2, 3, 4, 0, 1, 2, 3, 
    4, 5, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 7, 1, 2, 
    3, 4, 5, 6, 7, 2, 3, 4, 5, 6, 7, 3, 4, 5, 6, 7, 4, 5, 6, 
    7, 5, 6, 7, 6, 7, 7,
};

const uint8_t ac_lht_idx16[] =  {
/*0000000*/ 255,
/*0000001*/ 255,
/*0000010*/ 9,
/*0000011*/ 10,
/*0000100*/ 16,
/*0000101*/ 17,
/*0000110*/ 18,
/*0000111*/ 19,
/*0001000*/ 20,
/*0001001*/ 25,
/*0001010*/ 26,
/*0001011*/ 27,
/*0001100*/ 28,
/*0001101*/ 29,
/*0001110*/ 30,
/*0001111*/ 34,
/*0010000*/ 35,
/*0010001*/ 36,
/*0010010*/ 37,
/*0010011*/ 38,
/*0010100*/ 39,
/*0010101*/ 40,
/*0010110*/ 43,
/*0010111*/ 44,
/*0011000*/ 45,
/*0011001*/ 46,
/*0011010*/ 47,
/*0011011*/ 48,
/*0011100*/ 49,
/*0011101*/ 50,
/*0011110*/ 53,
/*0011111*/ 54,
/*0100000*/ 55,
/*0100001*/ 56,
/*0100010*/ 57,
/*0100011*/ 58,
/*0100100*/ 59,
/*0100101*/ 60,
/*0100110*/ 63,
/*0100111*/ 64,
/*0101000*/ 65,
/*0101001*/ 66,
/*0101010*/ 67,
/*0101011*/ 68,
/*0101100*/ 69,
/*0101101*/ 70,
/*0101110*/ 73,
/*0101111*/ 74,
/*0110000*/ 75,
/*0110001*/ 76,
/*0110010*/ 77,
/*0110011*/ 78,
/*0110100*/ 79,
/*0110101*/ 80,
/*0110110*/ 83,
/*0110111*/ 84,
/*0111000*/ 85,
/*0111001*/ 86,
/*0111010*/ 87,
/*0111011*/ 88,
/*0111100*/ 89,
/*0111101*/ 90,
/*0111110*/ 92,
/*0111111*/ 93,
/*1000000*/ 94,
/*1000001*/ 95,
/*1000010*/ 96,
/*1000011*/ 97,
/*1000100*/ 98,
/*1000101*/ 99,
/*1000110*/ 100,
/*1000111*/ 102,
/*1001000*/ 103,
/*1001001*/ 104,
/*1001010*/ 105,
/*1001011*/ 106,
/*1001100*/ 107,
/*1001101*/ 108,
/*1001110*/ 109,
/*1001111*/ 110,
/*1010000*/ 112,
/*1010001*/ 113,
/*1010010*/ 114,
/*1010011*/ 115,
/*1010100*/ 116,
/*1010101*/ 117,
/*1010110*/ 118,
/*1010111*/ 119,
/*1011000*/ 120,
/*1011001*/ 122,
/*1011010*/ 123,
/*1011011*/ 124,
/*1011100*/ 125,
/*1011101*/ 126,
/*1011110*/ 127,
/*1011111*/ 128,
/*1100000*/ 129,
/*1100001*/ 130,
/*1100010*/ 132,
/*1100011*/ 133,
/*1100100*/ 134,
/*1100101*/ 135,
/*1100110*/ 136,
/*1100111*/ 137,
/*1101000*/ 138,
/*1101001*/ 139,
/*1101010*/ 140,
/*1101011*/ 141,
/*1101100*/ 142,
/*1101101*/ 143,
/*1101110*/ 144,
/*1101111*/ 145,
/*1110000*/ 146,
/*1110001*/ 147,
/*1110010*/ 148,
/*1110011*/ 149,
/*1110100*/ 150,
/*1110101*/ 152,
/*1110110*/ 153,
/*1110111*/ 154,
/*1111000*/ 155,
/*1111001*/ 156,
/*1111010*/ 157,
/*1111011*/ 158,
/*1111100*/ 159,
/*1111101*/ 160,
/*1111110*/ 161,
};    
