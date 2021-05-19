/*
 * vga.c
 *
 *  Created on: May 10, 2021
 *      Author: virt-tjz
 */

#include "vga.h"

/* Do not optimize this file, as this
 * would break the magic legacy code
 */
#pragma GCC optimize("Os")

/* Local FB pointers */
static volatile uint8_t* 	buf_ptr1;
static volatile uint8_t* 	buf_ptr2;

/* Horizontal line state */
static volatile int 		hline = 0;		// which horizontal line we're on
static volatile uint8_t 	in_frame = 0;	// if we're inside visible area (non-blanking)
static volatile uint8_t 	skipv = 0;		/* if we should advance to the next horizontal
												line or repeat the last (upscale vertical resolution) */
void
handle_error(void)
{
	/* Die */
	for(;;);
}

/*
 * ISR Triggered when Horizontal Line has been finished
 */
void
dma_line_done(void)
{
	__disable_irq();

	/* If we are in visible area (non-vertical blanking) */
	if (hline > 32 && hline < 32 + 480) {
		/* Notify CM4 that we're drawing the frame, so as to not make SPI requests
		 * during this time interval.
		 */
		port9_fb[0] = 0x00;
		in_frame = 1;

		/* Setup Channel.
		 * Go to next horizontal line every other line (expands vertical size by 2)
		 * This makes the picture look better because vertical resolution is so much
		 * higher than horizontal resolution (limiting factor is clock speed).
		 */
		if (skipv == 0) {
			buf_ptr1 += SCRN_WIDTH;
			buf_ptr2 += SCRN_WIDTH;
			skipv = 1;

			/* Below NOPs align the code cycles such that
			 * DW0 and DW1 start at the same time and are in sync.
			 * This is dark magic.
			 */
			asm("nop;nop;nop;");
			asm("nop;nop;nop;");
			asm("nop;nop;nop;");
			asm("nop;nop;nop;");
			asm("nop;nop;");
		}
		else {
			skipv = 0;
		}

		/* Reload the DMA Descriptors with updated buffer pointers */
		Cy_DMA_Descriptor_SetSrcAddress(&DMA1_Descriptor_0, (uint32_t *) (buf_ptr1));
		Cy_DMA_Descriptor_SetSrcAddress(&DMA2_Descriptor_0, (uint32_t *) (buf_ptr2));
		Cy_DMA_Channel_Enable(DMA1_HW, DMA1_CHANNEL);
		Cy_DMA_Channel_Enable(DMA2_HW, DMA2_CHANNEL);
	}
	else {
		/* If we're in the vertical blanking interval, notify the CM4 core
		 * that it's clear to start SPI transactions.
		 */
		port9_fb[0] = 0xff;

		/*
		 * Disable DMA channels once for safety. (Not sure if required).
		 */
		if (in_frame) {
			Cy_DMA_Channel_Disable(DMA1_HW, DMA1_CHANNEL);
			Cy_DMA_Channel_Disable(DMA2_HW, DMA2_CHANNEL);
			in_frame = 0;
		}

	}

	// Go to next horizontal line.
	hline += 1;

	// Clear interrupt flag and re-enable interrupts.
    Cy_TCPWM_ClearInterrupt(HSYNC_T_HW, HSYNC_T_NUM, 0xff);
    __enable_irq();
}

/*
 * ISR Triggered when VSYNC reached.
 */
void vsync_rise_isr(void)
{
	__disable_irq();
    Cy_TCPWM_ClearInterrupt(VSYNC_T_HW, VSYNC_T_NUM, 0xff);
    //port9_fb[0] = 0x7f;
    buf_ptr1 = port9_fb;
    buf_ptr2 = port8_fb;
    hline = 0;
    __enable_irq();
}

void
init_dma(DW_Type* dma_hw, int channel, cy_stc_dma_descriptor_t* descriptor,
		const cy_stc_dma_descriptor_config_t* dconfig, const cy_stc_dma_channel_config_t* channel_config,
		uint32_t* _port_addr, uint32_t* src_addr)
{
	cy_en_dma_status_t dma_init_status;

	 /* Init DAM Descriptor */
    dma_init_status = Cy_DMA_Descriptor_Init(descriptor, dconfig);
    if (dma_init_status!=CY_DMA_SUCCESS) { handle_error(); }

    /* Initialize DMA channel */
    dma_init_status = Cy_DMA_Channel_Init(dma_hw, channel, channel_config);
    if (dma_init_status!=CY_DMA_SUCCESS) { handle_error(); }

    /* Set source and destination for descriptor 1 */
    Cy_DMA_Descriptor_SetSrcAddress(descriptor, (uint32_t *) src_addr);
    Cy_DMA_Descriptor_SetDstAddress(descriptor, (uint32_t *) _port_addr);

    /* Setup Channel */
    Cy_DMA_Channel_SetDescriptor(dma_hw, channel, descriptor);
    Cy_DMA_Channel_SetPriority(dma_hw, channel, 0UL);
    Cy_DMA_Channel_Enable(dma_hw, channel);

}

void
init_vga()
{
	/* Find Port register address */
    port9_addr = (uint32_t*) Cy_GPIO_PortToAddr(9);
    port8_addr = (uint32_t*) Cy_GPIO_PortToAddr(8);

    /* Initialise DW0 and DW1 DMA controllers */
    init_dma(DMA1_HW, DMA1_CHANNEL, &DMA1_Descriptor_0, &DMA1_Descriptor_0_config,
    		&DMA1_channelConfig, (uint32_t*) port9_addr, (uint32_t*) port9_fb);
    init_dma(DMA2_HW, DMA2_CHANNEL, &DMA2_Descriptor_0, &DMA2_Descriptor_0_config,
    		&DMA2_channelConfig, (uint32_t*) port8_addr, (uint32_t*) port8_fb);

    /* Create DMA Line interrupt service routine
     * This interrupt reloads the DMA controllers with updated addresses.
     */
	cy_stc_sysint_t DMA_LINE_DONE_ISR =
	{
			.intrSrc      = (IRQn_Type) NvicMux1_IRQn,
			.cm0pSrc 	  = HSYNC_T_IRQ,	// Fire on each HSYNC pulse
			.intrPriority = 0u,
	};


	/* Disable all other IRQs for safety */
	for (int i = 0; i < 0xff; i++) __NVIC_DisableIRQ(i);

    /* Initialize and enable the interrupt */
    Cy_SysInt_Init(&DMA_LINE_DONE_ISR, &dma_line_done);
    __NVIC_EnableIRQ(DMA_LINE_DONE_ISR.intrSrc);

    /* Start PWM Timers which will trigger DMA
     *
     * HSYNC_T 	 -- Generates Hsync pulse that goees into the VGA monitor
     * HBLANK_T  -- A slightly delayed Hsync pulse that goes off after Hsync goes low.
     * 				When this PWM pulses, DW0 gets triggered to start the next horizontal
     * 				line transfer. This creates the back porch.
     * HBLANK_T2 -- A copy of HBLANK_T. This triggers DW1 transfer. (We need separate
     * 				TCPWM blocks because each only connects to one DataWire)
     */
    Cy_TCPWM_PWM_Init(HSYNC_T_HW, HSYNC_T_NUM, &HSYNC_T_config);
    Cy_TCPWM_PWM_Enable(HSYNC_T_HW, HSYNC_T_NUM);

    Cy_TCPWM_PWM_Init(HBLANK_T_HW, HBLANK_T_NUM, &HBLANK_T_config);
    Cy_TCPWM_PWM_Enable(HBLANK_T_HW, HBLANK_T_NUM);

    Cy_TCPWM_PWM_Init(HBLANK_T2_HW, HBLANK_T2_NUM, &HBLANK_T2_config);
    Cy_TCPWM_PWM_Enable(HBLANK_T2_HW, HBLANK_T2_NUM);

    Cy_DMA_Enable(DMA1_HW);
    Cy_DMA_Enable(DMA2_HW);

    /* Create and register vertical sync ISR */
    cy_stc_sysint_t VSYNC_RISE_ISR =
    {
    		.intrSrc      = (IRQn_Type) NvicMux0_IRQn,
			.cm0pSrc 	  = (IRQn_Type) VSYNC_T_IRQ,
			.intrPriority = 0u,
    };

    /* Initialize and enable the interrupt */
    Cy_SysInt_Init(&VSYNC_RISE_ISR, &vsync_rise_isr);
    NVIC_EnableIRQ(VSYNC_RISE_ISR.intrSrc);


    Cy_TCPWM_PWM_Init(VSYNC_T_HW, VSYNC_T_NUM, &VSYNC_T_config);
    Cy_TCPWM_PWM_Enable(VSYNC_T_HW, VSYNC_T_NUM);

    Cy_TCPWM_TriggerStart(VSYNC_T_HW, VSYNC_T_MASK);

    /* Magic number */
    CyDelayCycles(470);

    Cy_TCPWM_TriggerStart(HBLANK_T_HW, HBLANK_T_MASK);

    CyDelayUs(31);
    CyDelayCycles(92);
}

/*
 * Initialise the framebuffers. This is an ugly legacy function and
 * in an ideal world, it would be cut out. However, this function as is,
 * provides just enough delay such that the timers and DMA DW's get
 * initialised on the right clock cycle so that everything is in sync.
 * If this function changes, the next function (init_vga()) will break
 * as the timers won't start in sync anymore.
 */
void
init_fb()
{
    for (int i = 0; i < 50; i++) port9_fb[i] = 0x00;
    for (int i = 50; i < 100; i++) port9_fb[i] = 0x00;
    for (int i = 20; i < 50; i++) port9_fb[i] = i % 2? 0x00: 0xff;
    for (int i = 100; i < 250; i++) port9_fb[i] = 0xff;
    for (int i = 150; i < 300; i++) port9_fb[i] = 0x00;
    for (int i = 300 ; i < 400; i++) port9_fb[i] = 0xff;
    port9_fb[0] = 0x00;
    port9_fb[327] = 0x00;
    port9_fb[329] = 0xff;
    for (int i = 0; i < 456; i++) port9_fb[i] = i % 2? 0x00: 0xff;
    port9_fb[455] = 0x00;

    int k = 0;
    for (int i = 0; i < SCRN_SIZE; i++) port9_fb[i] = 0x00;
    for (int r = 0; r < SCRN_HEIGHT; r++) {
		int c;
		for (c= 0; c < SCRN_WIDTH; c++) {
			uint16_t v = (k++) % 2? 0xff : 0xff;
			port9_fb[r * SCRN_WIDTH + c] = v;
			port8_fb[r * SCRN_WIDTH + c] = v;

		}
		k++;
		port9_fb[r * SCRN_WIDTH + c - 1] = 0x00;
		port8_fb[r * SCRN_WIDTH + c - 1] = 0x00;
    }

    for (int r = 0; r < SCRN_HEIGHT; r++) {
    	for (int c = 0; c < SCRN_WIDTH; c++) {
    		port9_fb[r * SCRN_WIDTH + 20] = 0x00;
    		port8_fb[r * SCRN_WIDTH + 20] = 0x00;
    	}
    }
}


