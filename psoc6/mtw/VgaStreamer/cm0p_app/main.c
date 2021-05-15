/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM0+ in the the Dual CPU Empty 
*              Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2020-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#define CY_CPU_CORTEX_M0P

#include "cy_pdl.h"
#include "cybsp.h"
#include <stdint.h>

#include "vga.h"

#pragma GCC optimize("Os")


int main(void)
{
    port9_fb = 0x8002000 + SCRN_SIZE;
    port8_fb = 0x8002000;

    cybsp_init();
    init_fb();
    init_vga();

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
