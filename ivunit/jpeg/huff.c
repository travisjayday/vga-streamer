#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "table.c"

#define VLI_DC 12
#define VLI_AC 11
#define HUFF_START_PARSE_SYMS 0xffff

/*
 * Given a DC size category, return it's codelength 
 * in bits. 
 */
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
 
/* 
 * Given an AC size category, return its codelength in 
 * bits
 */
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

/*
 * Given an AC Luminance codeword, return its index
 * into the luminance table and the length of the
 * given codeword. 
 */
int ac_lht_idx_from_codeword(uint16_t b, int* len) {
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
    return -1;
}

/*
 * Converts an amplitude into a VLI coded number.
 * @amp: The amplitude
 * @amp_size: Output of the size category for this amplitude. 
 * @dc: 12 if this is a DC amplitude, if it's AC, 11. Use #defined 
 *      VLI_DC or VLI_AC
 */
int32_t 
amp_to_vli(int32_t amp, uint8_t* amp_size, uint8_t category) 
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
int32_t
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

    uint32_t absval; 

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
int 
get_word_from_buf(uint16_t** p, uint16_t* word, int bitoffset) 
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
    //*code_i = 0x00; // terminating sym

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
uint32_t
dehuff(int32_t prev_dcval, uint8_t* buf8, int32_t outb[8][8]) 
{
    uint16_t* buf = (uint16_t*) buf8; 
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
    uint16_t* buf_s = buf;

    memset(outb, 0, 64 * sizeof(int32_t)); 

    // Read DC codeword
    bitoffset = get_word_from_buf(&buf, &word, bitoffset);
    size = dc_size_from_codeword(word, &len);
    bitoffset += len; 

    // Read DC amplitude
    bitoffset = get_word_from_buf(&buf, &word, bitoffset);
    bitoffset += size; 
    outb[0][0] = prev_dcval + vli_to_amp(word, size); 
       
    int codes_n = 127;
    while (codes_n-- != 0) {
        // Get huff codeword
        bitoffset = get_word_from_buf(&buf, &word, bitoffset);
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
            bitoffset = get_word_from_buf(&buf, &word, bitoffset);
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

#ifdef JPEG_TEST
void _main() {
    //test_huff();
}
#endif

//#include "huff-test.c"
