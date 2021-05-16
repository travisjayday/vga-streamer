/*
 * vga.h
 *
 *  Created on: May 10, 2021
 *      Author: virt-tjz
 */

#ifndef VGA_VGA_H_
#define VGA_VGA_H_

#include <stdint.h>
#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_pdl.h"
#include "cybsp.h"

/* Native, maximum resolution achieved by PSoC6 MCU */
#define SCRN_WIDTH 	352
#define SCRN_HEIGHT 480
#define SCRN_SIZE 	(SCRN_WIDTH * SCRN_HEIGHT)


/* MMIO Port Addresses */
static uint32_t* port9_addr;
static uint32_t* port8_addr;

/*
 * Pointers to store the base address of shared VGA buffers.
 * These buffers are defined in the custom shared linker script to live
 * in their own shared memory section, so that CM4 and CM0+ can access
 * it.
 */
uint8_t* port9_fb;
uint8_t* port8_fb;

/*
 * Initializes VGA.
 *
 * Configures device to output a signal as a standard 640x480 @ 60Hz.
 * Initializes DMA controllers to output pixel values to Port8/Port9
 * at a virtual resolution of 352x480. See main.c in the CM4 project
 * for how to populate the frame-buffers.
 */

void init_vga();
void init_fb();


#endif /* VGA_VGA_H_ */
