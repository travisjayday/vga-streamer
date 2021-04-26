#include "des.h"
#include "des_data.h"

uint64_t key_scheduler(uint8_t iter, uint64_t* cd);
uint32_t apply_f(uint32_t r, uint64_t k);

/**
 * encrypt_des_blk - Encrypt a 64-bit block with DES.
 * @block: A 64-bit block of data
 * @key:   A 64-bit key
 */
uint64_t 
encrypt_des_blk(uint64_t block, uint64_t key) 
{
    block = perm_bitstring(block, 64, perm_mask_ip); 

    uint32_t l_i = (0xffffffff00000000 & block) >> 32;
    uint32_t r_i = (0x00000000ffffffff & block);
    uint64_t cd_i = perm_bitstring(key, 64, perm_mask_pc1) & 0x00ffffffffffffff; 
    uint32_t tmp; 

    key = 0;
    int i;
    for (i = 0; i < 16; i++) {
        key = key_scheduler(i + 1, &cd_i);  // compute Key_i 
        tmp = r_i;                          // go to next 
        r_i = l_i ^ apply_f(r_i, key);      // layer in the
        l_i = tmp;                          // feistel network
    }

    block = (((uint64_t) r_i) << 32) | ((uint64_t) l_i); 
    return perm_bitstring(block, 64, perm_mask_ipi);
}

#define smask1   0x0100000010000000 /* set bit D[n+1] (C[0]) and bit C[n+1] */
#define smask1i  0x00ffffffefffffff /* discard C[0] and irrelevant bits     */
#define smask2   0x0300000030000000 /* set D[n+2:n+1] (C[1:0) and C[n+2:n+1]*/
#define smask2i  0x00ffffffcfffffff /* discard C[1:0] and irrelevant bits   */

/** key_scheduler - Given the current iteration CD blocks, update CD 
 *                  and  return the next key. 
 */
inline uint64_t 
key_scheduler(uint8_t iter, uint64_t* cd)
{
    // CD = 56bit number. C = 28 high bits. D = 28 low bits. 
    uint64_t out = *cd;

    if (iter == 1 || iter == 2 || iter == 9 || iter == 16) {
        // rotate C and D left by 1 
        out = (out << 1); 
        out = (out & smask1i) | ((out & smask1) >> 28); 
    }
    else {
        // rotate left by 2
        out = (out << 2); 
        out = (out & smask2i) | ((out & smask2) >> 28); 
    }

    // update C and D blocks
    *cd = out; 

    // return the 48 bit 
    out = perm_bitstring(out, 64, perm_mask_pc2); 
    out &= 0x0000ffffffffffff;
    
    return out;
}


/** apply_f - Apply the f(x), where f is defined as in DES Spec.
 */
inline uint32_t 
apply_f(uint32_t r, uint64_t k)
{
    int8_t   si;      
    uint8_t  cmd, val;       
    uint32_t out = 0; 
    uint8_t  idx; 

    // MSB of r is 1st position. 32 position is LSB.
    uint64_t t = (uint64_t) r;
    uint64_t re = (t & 0xf8000000) << 15 |
                  (t & 0x1f800000) << 13 |
                  (t & 0x01f80000) << 11 |
                  (t & 0x001f8000) << 9  |
                  (t & 0x0001f800) << 7  |
                  (t & 0x00001f80) << 5  |
                  (t & 0x000001f8) << 3  |
                  (t & 0x0000001f) << 1  | 
                  (t & 0x00000001) << 47 |
                  (t >> 31);

    uint64_t x = (uint64_t) re ^ k; 

    /**
     *  Do the sblock calculations. Convert 48 bit --> 32 bit output. 
     */ 
    for (si = 7; si >= 0; si--, x >>= 6) {
        /* Get 6 bits as cmd word. B8, ..., B1 */
        cmd = x & 0x3f;                             
        
        /* Compute the S-Block byte-level index into the table */
        idx = ((cmd >> 2) | (cmd << 4)) & 0x1f;

        /* Find S8, ..., S1 and add byte-level idx, then fetch*/
        val = sblocks[(si << 5) + idx];

        /* If cmd uses upper nibble, shift it */
        if (cmd & 2) val >>= 4; 

        /* Shift and or value into out */
        out >>= 4; 
        out |= val << 0x1c;
    }

    return perm_bitstring(out, 32, perm_mask_p); 
}
