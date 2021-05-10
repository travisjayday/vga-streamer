struct pix {
    uint8_t r;
    uint8_t g;
    uint8_t b;  
};

#define CLAMP_F(f) f > 255? 255 : f < 0? 0 : (uint8_t) f 

//#define USE_FLOAT_COLORMAT 


void 
dec_ycrcb_blk(uint8_t* y_blk, uint8_t* cr_blk, uint8_t* cb_blk, 
        uint8_t subsampl_cr, struct pix* rgb_out, uint32_t width, uint32_t height)
{
    int32_t y;  
    int32_t cr;
    int32_t cb; 

    int i = 0; 
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            y  = y_blk[r * width + c];
            cr = cr_blk[(r/subsampl_cr * width/subsampl_cr) + c / subsampl_cr] - (256 / 2) / COLOR_SCALER; 
            cb = cb_blk[(r/subsampl_cr * width/subsampl_cr) + c / subsampl_cr] - (256 / 2) / COLOR_SCALER; 

#ifdef USE_FLOAT_COLORMAT
            float redf = 1 * y +   0      * cb +  1.5748 * cr; 
            float grnf = 1 * y +  -0.1873 * cb + -0.4681 * cr; 
            float bluf = 1 * y +   1.8556 * cb +  0      * cr; 

#else
            int16_t red = ((1<<21) * y + 0       * cb + 3302595 * cr) >> 21;
            int16_t grn = ((1<<21) * y - 392796  * cb - 981677  * cr) >> 21;        // same, more, less
            int16_t blu = ((1<<21) * y + 3891475 * cb + 0       * cr) >> 21;
#endif

            rgb_out[r * width + c].b = CLAMP_F(red * COLOR_SCALER);
            rgb_out[r * width + c].g = CLAMP_F(grn * COLOR_SCALER);
            rgb_out[r * width + c].r = CLAMP_F(blu * COLOR_SCALER);
        }
    }  
}

void
dec_ycrcb(uint8_t* y_blk, uint8_t* cb_blk, uint8_t* cr_blk, 
        uint8_t subsampl_cr, struct pix* rgb_out, uint32_t width, uint32_t height)
{
    for (uint32_t r = 0; r < height; r += 8) {
        for (uint32_t c = 0; c < width; c += 8) {
            dec_ycrcb_blk(y_blk + r * width + c, 
                         cr_blk + (r/subsampl_cr * width/subsampl_cr) + c/subsampl_cr, 
                         cb_blk + (r/subsampl_cr * width/subsampl_cr) + c/subsampl_cr, 
                         subsampl_cr, 
                         rgb_out + r * width + c, 
                         width, height);
        }
    } 
}
