#include "debug.h"

XImage *CreateTrueColorImage(Display *display, Visual *visual, uint32_t* image, int width, int height, int sf)
{
    int x, y, xx, yy;
    unsigned char *image32 = (unsigned char*) malloc(width*height*sf*sf*4);
    uint32_t *p = (uint32_t*) image32;
    int absx = 0;
    int absy = 0; 
    for(y=0; y<height; y++) {
        for (yy = 0; yy < sf; yy++, absy++) {
            absx = 0;
            for(x=0; x < width; x++) {
                for (xx = 0; xx < sf; xx++) {
                    p[absy * width * sf + absx++] = image[y * width + x]; // red / green / blue
                }
            }
        }
    }
    return XCreateImage(display, visual, 
            DefaultDepth(display,DefaultScreen(display)), 
            ZPixmap, 0, image32, width*sf, height*sf, 32, 0);
}

void processEvent(Display *display, Window window, XImage *ximage, int width, int height)
{
    XEvent ev;
    XNextEvent(display, &ev);
    switch(ev.type) {
        case Expose:
            XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
            break;
        case ButtonPress:
            exit(0);
    }
}

void 
show_vga(uint8_t* img)
{
    XImage *ximage;
    int width = 1024, height = 1024;
    Display *display = XOpenDisplay(NULL);
    Visual *visual = DefaultVisual(display, 0);
    Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);
    if(visual->class != TrueColor)
    {
        fprintf(stderr, "Cannot handle non true color visual ...\n");
        exit(1);
    }

    uint8_t b_bounds[] = {0, 64, 128, 192, 255}; 
    uint8_t rg_bounds[] = {0, 32, 64, 96, 128, 160, 192, 224, 255};

    uint32_t* expanded_img = calloc(WIDTH * HEIGHT, sizeof(uint32_t)); 
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            uint8_t c = img[y * WIDTH + x]; 
            uint8_t r = rg_bounds[(c & 0b11100000) >> 5];
            uint8_t g = rg_bounds[(c & 0b00011100) >> 2];
            uint8_t b = b_bounds[c & 0b00000011]; 
            expanded_img[y * WIDTH + x] = r << 16 | g << 8 | b; 
        }
    }

    XStoreName(display, window, "FLOATME");
    ximage=CreateTrueColorImage(display, visual, expanded_img, 256, 256, 4);
    XSelectInput(display, window, ButtonPressMask|ExposureMask);
    XMapWindow(display, window);
    while(1)
    {
        processEvent(display, window, ximage, width, height);
    }
}

#ifndef MAKE
int 
main(int argc, char **argv)
{
    uint8_t* img = malloc(WIDTH * HEIGHT * sizeof(char)); 
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            img[y * WIDTH + x] = x % 2 == 0? 0b00011100 : 0x00;
        }
    }
    show_vga(img);
}
#endif
