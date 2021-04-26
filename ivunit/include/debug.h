#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <stdint.h>

#define WIDTH 256
#define HEIGHT 256

// Display a WIDTH * HEIGHT RGB332 encoded pixbuf using X11.
void show_vga(uint8_t* img); 

#endif
