#include "jpg.h"
#include "targa.h"
#include <sys/time.h>

unsigned long 
micros()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (1000000*tv.tv_sec) + tv.tv_usec;
}

int main() {
    // load image from disk 
	tga_image tga;
	load_tga(&tga, "test.tga");

    uint32_t start_t = micros();
    for (int i = 0; i < 1000; i++) {
        // compress iamge 
        uint32_t encoded_data_n = 0;
        uint8_t* compressed_image = compress_channel(&encoded_data_n, tga.image_data, tga.width, tga.height); 
       
        // decompress image
        uint8_t* buf = (uint8_t*) calloc(1, tga.width * tga.height * sizeof(uint8_t)); 
        decompress_channel(buf, compressed_image, tga.width, tga.height);
    }
    uint32_t end_t = micros();
    printf("DT: %.2fms", (float) (end_t - start_t) / 1000.0f);
}
