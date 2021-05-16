#define CY_CPU_CORTEX_M0P

#include "cy_pdl.h"
#include "cybsp.h"
#include <stdint.h>

#include "vga.h"

/* Do not optimize this file as it would break magic,
 * cycle dependent timings.
 */
#pragma GCC optimize("Os")

int main(void)
{
	/* Compute the addresses of the shared memory buffers. These
	 * are defined in main.c in the CM4 project. They live in the
	 * shared section .cy_sharedmem defined in linker script.
	 */
    port9_fb = (uint8_t*) 0x8002000 + SCRN_SIZE;
    port8_fb = (uint8_t*) 0x8002000;


    cybsp_init();	// initialize all hardware as configured in modus device config.
    init_fb();		// initialize the shared framebuffers
    init_vga();		// initialize the DMAs, register interrupts, and start PWMs

    __enable_irq();

    // Jump to main() in CM4 Application.
    // Entry point: 0x10000000 + CM0P Flash Size (which is 0x5000)
    Cy_SysEnableCM4(0x10005000);

    for (;;)
    {
    	// Wait for HSYNC Interrupts...
    }
}

/* [] END OF FILE */
