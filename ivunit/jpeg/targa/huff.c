#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "table.c"
#include "reverse_table.c"

void _print_bits(uintmax_t n, uint32_t bits) {
    char c;
    for (size_t i = bits; i-- != 0;)
    {
        if ((n & (1UL << i)) != 0) c = '1';
        else c = '0';
        printf ("%c", c);
    }
    putchar('\n');
}

uint8_t get_dc_code_len(uint8_t category) {
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
 
uint8_t get_ac_code_len(uint8_t run_size) {
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

int32_t 
amp_to_vli(int32_t amp, uint8_t* amp_size) 
{
    if (amp == 0) {
        *amp_size = 0; 
        return 0; 
    }
    for (*amp_size = 1; (*amp_size) < 12; (*amp_size)++) {
        int bound = 1 << (*amp_size); 
        if (abs(amp) < bound) {
            if (amp < 0) amp = (bound - 1) + amp; 
            return amp;
        }
    }   
    printf("INVALID SIZE: %d", amp); exit(-1);
    return -1; 
}

int32_t
vli_to_amp(uint16_t vli, uint8_t vli_size)
{
    int sign = (vli & 0x8000)? 1 : -1; 
    int32_t amp = vli >> (16 - vli_size); 
    if (sign < 0) {
        int bound = 1 << vli_size; 
        amp = -(bound - 1) + (int32_t) amp; 
    }
    return amp; 
}

uint8_t runlen = 0; 
uint8_t zeroblocks = 0; 
void parse_syms(int32_t val, uint8_t** codes, uint16_t** amps) {
    uint32_t absval; 

    if (val == 0) {
        if (++runlen > 15) {
            // too many zeros!
            zeroblocks++;
            runlen = 0; 
        }
    }
    else {
        while (zeroblocks > 0) {
            **codes = 151;
            (*codes)++; 
            zeroblocks--;
            printf("151/+16 ZEROS");
        }

        int32_t v = val;

        /* Compute size and magnitude representation */
        uint8_t size = 0; 
        val = amp_to_vli(val, &size);

        uint16_t c = (runlen * 10) + size;
        **codes = c; 
        printf("RL/Size: %d/%d --> index %d, Val %d -- > Amp: %d\n", runlen, size, **codes, v, val); 
        (*codes)++; 
        **amps = val | (size << 12); 
        (*amps)++; 
        runlen = 0; 
    }
}

uint16_t
add_bits2buf(uint16_t bits_left, uint16_t** buf, uint16_t word, uint8_t len)
{
    //printf("Adding %x with len %d\n", (uint32_t) word, (uint32_t) len);
    if (bits_left >= len) {
        **buf |= (word << (bits_left - len)); 
        bits_left -= len; 
        if (bits_left == 0) {
            (*buf)++; 
            bits_left = 16;
        }
    }
    else {
        uint16_t upper = word >> (len - bits_left); 
        **buf |= upper; 
        (*buf)++; 
        bits_left = 16 - (len - bits_left);
        **buf |= (word << bits_left); 
    }
    //printf("Bits left: %d\n", (uint32_t) bits_left);
    return bits_left; 
}


uint8_t 
huff(int8_t prev_dcval, int32_t b[8][8], uint16_t* buf) 
{
    runlen = 0; 
    zeroblocks = 0; 
    uint8_t outs = 0; 

    uint8_t* codes = calloc(128, 1); 
    uint16_t* amps = calloc(128, 1); 

    uint8_t dcval = b[0][0];

    uint8_t* code_i = codes; 
    uint16_t* amp_i = amps; 
    int x = 0; 
    int y = 0; 
    for (int d = 1; d < 8; d++) {
        x = d; y = 0; 
        while (x >= 0) {
            int32_t v = b[y][x]; 
            parse_syms(v, &code_i, &amp_i); 
            y++; 
            x--;
        }
    }
    for (int d = 1; d < 8; d++) {
        x = 7; y = d; 
        while (y < 8) {
            int32_t v = b[y][x]; 
            parse_syms(v, &code_i, &amp_i); 
            y++; 
            x--;
        }
    }
    *code_i = 0x00; // terminating sym

    /* The following converts intermediate codes to huffman codewords */
    printf("\nIntermediate coding:\n");
    uint8_t* p = codes - 1; 
    uint16_t* a = amps - 1; 
    int i = 0; 
    int bits_left = 16; 
    uint16_t* bufs = buf - 1; 

    /* Find DC amplitude and category (size) */
    int32_t dc_amp = b[0][0] - prev_dcval;
    uint8_t dcsize = 0; 
    dc_amp = amp_to_vli((uint32_t) dc_amp, &dcsize);
    
    /* Add DC huffman code (VLC) for size, then VLI for amplitude */
    uint8_t codelen = get_dc_code_len(dcsize);
    uint16_t codeword = dc_lht_codes[dcsize];
    bits_left = add_bits2buf(bits_left, &buf, codeword, codelen); 
    bits_left = add_bits2buf(bits_left, &buf, dc_amp, dcsize); 

    while (p != code_i) {

        /* Add AC huffman code (VLC) for size, then VLI for amplitude */
        uint8_t code = *(++p); // index into table
        codelen = get_ac_code_len(code);   
        codeword = ac_lht_codes[code]; 
        bits_left = add_bits2buf(bits_left, &buf, codeword, codelen);

        printf("Table Index: %d -> Encodes to [%d bits] %x; \n", code, codelen, codeword); 

        // skip EOB and ZRL codes (because they don't have amps) 
        if (code != 0 && code != 151) {
            uint16_t amp = (*(++a));
            uint8_t amp_size = amp >> 12; 
            amp &= 0x0fff;
            bits_left = add_bits2buf(bits_left, &buf, amp, amp_size);
            //printf("Amp: %x [%d]\n", amp, amp_size);
        }

        //_print_bits(codeword, 16); 
    }
    if (bits_left != 0) {
        buf++;
    }
    return buf - bufs - 1; 
}

int ac_idx_from_codeword(uint16_t b, int* len) {
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

/* len = codelen */
int dc_size_from_codeword(uint16_t b, int* len) {
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
}

int 
get_word_from_buf(uint16_t** p, uint16_t* word, int bitoffset) 
{
    while (bitoffset >= 16) {
        bitoffset -= 16; 
        (*p)++; 
    }
    if (bitoffset == 0) 
        *word = **p; 
    else 
        *word = ((**p) << bitoffset) | ((*((*p)+1) >> (16 - bitoffset))); 
    return bitoffset;
}
    

void 
dehuff(int32_t prev_dcval, uint16_t* buf, 
        int32_t* outb, uint16_t dst_col_offset, uint16_t dst_width) 
{
    uint16_t* p = (uint16_t*) buf; 
    int16_t* amps = calloc(128, 1); 
    uint8_t bitoffset = 0; 

    uint16_t word; 
    int len; 
    int idx;
    int rl; 
    int size; 

    bitoffset = get_word_from_buf(&p, &word, bitoffset);
    printf("Codeword: %x", word);
    size = dc_size_from_codeword(word, &len);
    bitoffset += len; 

    printf("Len/Size = %d/%d\n", len, size); 
    bitoffset = get_word_from_buf(&p, &word, bitoffset);
    bitoffset += size; 
    int32_t dc_offset = vli_to_amp(word, size); 
    int32_t dc_val = prev_dcval + dc_offset; 
    outb[0 + dst_col_offset] = dc_val; 

    printf("DC Amp: %x", dc_offset);

    uint8_t lin_idx = 1; 
    uint8_t dig_row = 1;
    uint8_t dig_col = 1;
        
    int codes_n = 128;
    printf("\n");
    while (codes_n-- != 0) {
        bitoffset = get_word_from_buf(&p, &word, bitoffset);
        printf("Raw word: %x", word);
        // 0x1f20
        idx = ac_idx_from_codeword(word, &len); 
        if (idx == 0) break;
        rl = idx / 10; 
        size = idx % 10; 
        //0/10 -> 3/8
        if (size == 0) {
            rl--;
            size = 10;
        }
        printf("RL/Size = %d/%d; [len:%d] ", rl, size, len); 
        bitoffset += len; 

        int32_t amp; 
        if (idx != 151) {
            bitoffset = get_word_from_buf(&p, &word, bitoffset);
            amp = vli_to_amp(word, size); 
            printf("Amp: %d\n", amp);
            bitoffset += size; 
        }
        else {
            printf("Skipping amp!\n");
            amp = 0; 
        }

        // at this point we've decoded the next zero runlength and
        // the amplitude following this runlength. 
        lin_idx += rl; 
        dig_row = row_lin2diag[lin_idx];  
        dig_col = col_lin2diag[lin_idx];  
        printf("[%d] (%d, %d) <- %d", lin_idx, dig_row, dig_col, amp);
        outb[dig_row * dst_width + dig_col + dst_col_offset] = amp;  
        lin_idx++;
    }
}

#ifndef NO_MAIN
int main() {
    int32_t amp = 34; 
    uint8_t size = 0;
    printf("Amp: %d/%d\n", amp, size); 
    amp = amp_to_vli(amp, &size);
    amp <<= (16 - size); 
    printf("VLI: 0x%x/%d\n", amp, size); 
    amp = vli_to_amp(amp, size); 
    printf("Amp: %d/%d\n", amp, size); 

    uint16_t* buf = malloc(1024); 

    int32_t block[8][8] = {
        57, 20, -1, 0, 0, 0, 0, 0, 
        10, -3, 0, 1, 0, 0, 0, 1,
        5, 2, 0, 0, 0, 0, 2, 0,
        1, 0, 0, 0, 0, 3, 0, 0,
        0, 0, 0, 0, -10, 0, 0, 0,
        0, 0, -12, 0, 0, 0, 6, 0,
        0, 0, 0, 0, 0, 7, 0, 0,
        8, 0, 0, 0, 0, 0, 0, 1,
    };
    uint8_t prev_dcval = 12; 
    /*int32_t block[8][8] = {
        15,  0,-1, 0, 0, 0, 0, 0, 
        -2, -1, 0, 0, 0, 0, 0, 0,
        -1, -1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    int32_t block[8][8] = {
        0, 
        1, 2, 3, 4, 5, 6, 7, 8, 
        9, 10, 11, 12, 13, 14, 15, 16, 
        17, 18, 19, 20, 21, 22, 23, 24, 
        25, 26, 27, 28, 29, 30, 31, 32, 
        33, 34, 35, 36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 46, 47, 48, 
        49, 50, 51, 52, 53, 54, 55, 56, 
        57, 58, 59, 60, 61, 62, 63
    };*/
 
    uint8_t outs = huff(prev_dcval, block, buf);
    for (uint8_t i = 0; i< outs; i++) {
        printf("%04x ", (uint32_t) buf[i]); 
    }

    int8_t* outb = calloc(255, 1);
    /*// idx 42, 31
    buf[0] = 0b1111111000111010;
    dehuff(buf, 2); 

    // idx 154, 151, 22
    buf[0] = 0b1111111111110111;
    buf[1] = 0b1111111100111111;
    buf[2] = 0b0010000000000000;*/
    //outbmalloc
//  void dehuff(uint16_t* buf, uint8_t* outb, uint16_t dst_col_offset, uint16_t dst_width) {

    dehuff(prev_dcval, buf, outb, 0, 8); 
    printf("\n");
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            printf("%d ", (int) *(outb + r * 8 + c)); 
        }
        printf("\n");
    }
    test_huff();
}
#endif

#include "huff-test.c"
