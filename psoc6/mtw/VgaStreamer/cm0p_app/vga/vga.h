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

#define SCRN_WIDTH 	352
#define SCRN_HEIGHT 480
#define SCRN_SIZE 	(SCRN_WIDTH * SCRN_HEIGHT)


/* MMIO Port Addresses */
static uint32_t* 			port9_addr;
static uint32_t* 			port8_addr;


//extern uint8 port9_fb[SCRN_SIZE] CY_SECTION(".cy_sharedmem");
//extern uint8 port8_fb[SCRN_SIZE] CY_SECTION(".cy_sharedmem");

uint8_t* port9_fb;
uint8_t* port8_fb;

void init_vga();
void init_fb();


#endif /* VGA_VGA_H_ */
