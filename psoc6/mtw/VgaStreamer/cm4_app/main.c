#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "jpeg/jpg.h"

//#define BENCH
//#define FULLRES
#define DEBUG

/* Pin Definitions for SPI */
#define mSPI_MOSI               (P6_0)
#define mSPI_MISO               (P6_1)
#define mSPI_SCLK               (P6_2)
#define mSPI_SS                 (P6_3)
#define SPI_SLAVE_READY_PIN_PRT (P5_7_PORT)
#define SPI_SLAVE_READY_PIN 	(P5_7)		// ESP32 pulls high if data is avail
#define SPI_FREQ_HZ             (12000000UL)
#define SPI_MTU 				512u		// Bytes per SPI transaction

/*
 * Defines for Image Decompression
 * These must match the server's defines
 */
#define IMG_WIDTH 				160
#define IMG_HEIGHT 				128
#define NUM_COLORS 				128
#define SUBSAMPLE_CHROMA 		4

/* Decompression Helpers */
#define CHROMA_WIDTH 			(IMG_WIDTH / SUBSAMPLE_CHROMA)
#define CHROMA_HEIGHT 			(IMG_HEIGHT / SUBSAMPLE_CHROMA)
#define COLOR_SCALER 			(256/NUM_COLORS)
#define MAX_COLOR 				(NUM_COLORS-1)
#define CLAMP_F(f) 				f > MAX_COLOR? MAX_COLOR : f < 0? 0 : (uint8_t) f

/*
 * VGA Specific Defines. See vga.h.
 */
#define SCRN_WIDTH 	352
#define SCRN_HEIGHT 480
#define SCRN_SIZE 	(SCRN_WIDTH * SCRN_HEIGHT)

/*
 * Places an RGB pixel at a given row and column
 */
#define PUT_PIXEL(_r, _c, red, grn, blu) 						\
	port8_fb[(_r) * SCRN_WIDTH + _c] = (grn<<6) | (blu<<1);		\
    port9_fb[(_r) * SCRN_WIDTH + _c] = (red<<3) | (grn>>2);		\

/*
 * The main VGA Framebuffers which live in shared memory.
 *
 * DMA Controllers DW0, DW1 will read form these buffers and
 * sequentially output the bytes stored in them into their respective
 * GPIO Ports (Port8 and Port9).
 *
 * CM4 Core fills these buffers with RGB Data.
 *
 * CM0+ Core configures DW0, DW1 to iterate through these buffers for
 * each horizontal line in a frame, then resets the DataWires to start
 * at the beginning for netx frame.
 */
volatile uint8_t port9_fb[SCRN_SIZE] CY_SECTION(".cy_sharedmem");
volatile uint8_t port8_fb[SCRN_SIZE] CY_SECTION(".cy_sharedmem");
volatile uint8_t spi_ready;

#ifdef BENCH
/* Provides declaration and initialisation of huffman/DCT coded test image */
#include "huffman_test.c"
#endif
#ifdef FULLRES
#include "fullres_test.c"
#endif

/*
 * Abort Program Flow. Fatal Error.
 */
void handle_error(void);

/*
 * Returns microseconds since program time started.
 */
uint32_t micros(void);


/*
 * ISR For when ESP slave sets its ready pin high. New SPI Data available.
 */
static void
spi_data_ready(void *handler_arg, cyhal_gpio_irq_event_t event)
{
	spi_ready = 1;
	Cy_GPIO_ClearInterrupt(SPI_SLAVE_READY_PIN_PRT, SPI_SLAVE_READY_PIN);
}

/*
 * Color Converts an 8x8 block of ITU-R BT.709 encoded YCbCr image data into RGB Data
 * and then writes this RGB bits to the Port8 and Port9 framebuffers for output over GPIO.
 */
static inline void
dec_ycrcb_blk(const uint8_t* restrict y_blk, const uint8_t* restrict cr_blk, const uint8_t* restrict cb_blk,
        uint8_t subsampl_cr, uint32_t row_base, uint32_t col_base, uint32_t width, uint32_t height);

/*
 * Helper to iterate and paint all 8x8 blocks in a given YCbCr image to the VGA framebuffer.
 * Calls dec_ycrcb_blk.
 */
static void
dec_ycrcb(const uint8_t* restrict y_blk, const uint8_t* restrict cb_blk, const uint8_t* restrict cr_blk,
        uint8_t subsampl_cr, uint32_t row_base, uint32_t col_base, uint32_t width, uint32_t height);


/*
 * CM4 Core - Entry Point.
 * At this point VGA has been initialized by CM0+ Core.
 *
 * This Core is responsible for transferring incoming encoded image data
 * over SPI, decoding that image data into RGB, and filling the correct
 * shared VGA framebuffers.
 */
int
main(void)
{
	uint8_t* 	huffman_data;
	uint8_t* 	ychannel;
	uint8_t* 	cbchannel;
	uint8_t* 	crchannel;
	int 		huffman_data_i = 0;
	uint8_t 	transmit_data[512];
	const char* sig = "FRSTART";
	cyhal_spi_t mSPI;
    cy_rslt_t 	result;

    /* Initialize UART */
	result = cy_retarget_io_init( CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
							  CY_RETARGET_IO_BAUDRATE);

    /*
     * To avoid bus arbitration when the DMA DataWires are active, we can
     * set busmaster priorities in the MSx_CTL Protection Registers.
     *
     * See PSOC62 Register TRM for details. Note that the addresses the TRM
     * PDF provides are wrong. Looking through the source, the below addresses
     * are correct for the following busmasters.
     */
    volatile uint32_t* MS0_CTL = (uint32_t*) 0x40230000;
    volatile uint32_t* MS1_CTL = (uint32_t*) 0x40230004;
    volatile uint32_t* MS2_CTL = (uint32_t*) 0x40230008;
    volatile uint32_t* MS3_CTL = (uint32_t*) 0x4023000C;
    volatile uint32_t* MS14_CTL= (uint32_t*) 0x40230038;
    volatile uint32_t* MS15_CTL= (uint32_t*) 0x4023003C;

    /*
     * Give CM0+ Core, DW0 DMA Controller, priority zero (highest);
     * Give CM4 Core and other BusMasters priority three (lowest)
     */
    *MS14_CTL = 0b1100000011;		// CM4
    *MS15_CTL = 0b1100000011;		// TC
    *MS1_CTL  = 0b1100000011;		// TC
    *MS0_CTL  = 0b0000000011;		// CM0+
    *MS2_CTL  = 0b0000000011;		// DW0
    *MS3_CTL  = 0b0000000011;		// DW1

#ifdef DEBUG
    printf("MS0_CTL: %x\n", *MS0_CTL);
    printf("MS2_CTL: %x\n", *MS2_CTL);
    printf("MS3_CTL: %x\n", *MS3_CTL);
    printf("MS14_CTL: %x\n",*MS14_CTL);
#endif


	/* Init LED */
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_gpio_write(CYBSP_USER_LED, 0);


    /* Enable mirosecond counter */
    Cy_TCPWM_Counter_Init(UST_HW, UST_NUM, &UST_config);
    Cy_TCPWM_Counter_Enable(UST_HW, UST_NUM);
    Cy_TCPWM_TriggerStart(UST_HW, UST_MASK);

#ifdef BENCH
    printf("Startin test...\n");
	uint32_t start_t = micros();
	uint32_t bench_w = 344;
	uint32_t bench_h = 240;
	uint8_t* buf1 = malloc(bench_w * bench_h);
	uint8_t* buf2 = malloc(bench_w * bench_h);
	uint8_t* buf3 = malloc(bench_w * bench_h);
	uint32_t count = 2;
	for (int i = 0; i < count; i++) {
		decompress_channel(buf1, test_comp_y, bench_w, bench_h);
		decompress_channel(buf2, test_comp_cb, bench_w, bench_h);
		decompress_channel(buf3, test_comp_cr, bench_w, bench_h);
		dec_ycrcb(buf1, buf2, buf3, 1, 1, 3, bench_w, bench_h);
	}
	uint32_t end_tt = micros();
	printf("TD: %.2fms/frame\n", ((end_tt - start_t) / 1000.0f)/count);
	while(1);
#endif

#ifdef FULLRES
	int i = 0;
    for (int r = 0; r < SCRN_HEIGHT; r++) {
    	for (int c = 0; c < 344; c++) {
    		uint16_t val = fullres_test[i++];
    		port9_fb[r*SCRN_WIDTH+c] = val & 0xffff;
    		port8_fb[r*SCRN_WIDTH+c] = (val >> 8);
    		if (c == SCRN_WIDTH - 1) {
    			port9_fb[r*SCRN_WIDTH+c] = 0;
    			port8_fb[r*SCRN_WIDTH+c] = 0;
    		}

    	}
    }
    while (1);
#endif

	/* Configure SPI */
	printf("Configuring SPI master...\r\n");
	memset(&mSPI, 0, sizeof(mSPI));
	result = cyhal_spi_init(&mSPI,
			mSPI_MOSI, mSPI_MISO, mSPI_SCLK, mSPI_SS,
			NULL, 8, CYHAL_SPI_MODE_01_MSB, false);

	if (result != CY_RSLT_SUCCESS) {
		if (result == CYHAL_SPI_RSLT_PIN_CONFIG_NOT_SUPPORTED) printf("Unsupported Pins\n");
		else if (result == CYHAL_HWMGR_RSLT_ERR_INUSE) printf("CYHAL hardware in use...\n");
		else printf("Other SPI error\n");
		handle_error();
	}

	cyhal_spi_set_frequency(&mSPI, SPI_FREQ_HZ);

	/*
	 * Initialize SPI Handshake Pin. ESP32 will set this pin high when there
	 * is new data available and ready.
	 */
	cyhal_gpio_init(SPI_SLAVE_READY_PIN,
			CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
	cyhal_gpio_enable_event(SPI_SLAVE_READY_PIN,
			CYHAL_GPIO_IRQ_RISE, 0, true);
	cyhal_gpio_register_callback(SPI_SLAVE_READY_PIN,
			spi_data_ready, NULL);

	/* Allocate Buffers for received data */
	huffman_data 	= malloc(8192);
	ychannel  		= malloc(IMG_WIDTH * IMG_HEIGHT);
	cbchannel 		= malloc(CHROMA_WIDTH * CHROMA_HEIGHT);
	crchannel 		= malloc(CHROMA_WIDTH * CHROMA_HEIGHT);

    port9_fb[0] = 0x00;

    /* Set VGA Screen to Black */
    for (int r = 0; r < SCRN_HEIGHT; r++) {
    	for (int c = 0; c < SCRN_WIDTH; c++) {
    		port9_fb[r*SCRN_WIDTH+c] = 0;
    		port8_fb[r*SCRN_WIDTH+c] = 0;
    	}
    }

	/* Enable interrupts */
    __enable_irq();

    /* Initially we are ready */
    spi_ready = 1;

    /*
     * Flag to check if we've encountered our first start frame symbol
     * and are currently filling data for that frame
     */
    uint8_t in_frame = 0;

#ifdef DEBUG
    uint32_t last_fr_time = 0;
#endif

    /*
     * Flag to check how many SPI transfer's we're allowed to make.
     * Note that making an SPI transfer causes a missed horizontal line
     * on the VGA monitor, so we should only make transfers during the
     * vertical blanking interval and a little bit after.
     *
     * To help with this, CM0+ Sets port9_fb[0] to 0xff if we are in
     * the vertical blanking interval. Otherwise port9_fb[0] reads 0x00.
     */
    int spi_ok = 0;
	while (1) {
		/*
		 * Check if:
		 * 1. ESP32 has data and is waiting to accept an SPI transaction.
		 * 2. We are in the vertical blanking zone OR we've already started
		 * 		transferring the first chunk of a frame and we have clearance
		 * 		to get the next.
		 */
		if (spi_ready /*&& (port9_fb[0] == 0xff || spi_ok != 0)*/) {

			spi_ready = 0;

			/* If we haven't encountered a frame start symbol, write to beginning of buffer */
			if (in_frame == 0) huffman_data_i = 0;

			/* Do SPI transaction */
			cyhal_spi_transfer(&mSPI,
					transmit_data,
					SPI_MTU,
					huffman_data + huffman_data_i,
					SPI_MTU,
					0x00
			);

			/* Check if we're not in a frame and have encountered a frame start symbol */
			if (in_frame == 0 && strcmp((char*) huffman_data, sig) == 0) {
				huffman_data_i = SPI_MTU;
				spi_ok = 1;		// Give clearance to pull SPI as quickly as possible
								// (without having to wait for vertical blanking)
				in_frame = 1;	// We are now receiving subsequent frame data.
			}
			else {
				/* Check if we've received an SPI_MTU Block ending in zeros. If this is the
				 * case, then we know the frame is done. Note that it would probably be better
				 * to parse the size of the frame when encountering a start symbol instead of
				 * relying on this.
				 */
				if (huffman_data[huffman_data_i + SPI_MTU - 3] == 0 &&
						huffman_data[huffman_data_i + SPI_MTU - 2] == 0 &&
						huffman_data[huffman_data_i + SPI_MTU - 1] == 0) {

					in_frame = 0;
					spi_ok = 0;
					// frame is done. Process it.
#ifdef DEBUG
					uint32_t frame_dt = micros() - last_fr_time;
					last_fr_time = micros();
					printf("Time since last frmae: %dms\n\r", (frame_dt / 1000));

					int sum = 0;
					for (int i = 0; i < huffman_data_i + SPI_MTU; i++)
						sum += huffman_data[i];
					printf("Huffman checksum: %d\n\r", sum);
					uint32_t start_t = micros();
#endif

					/* Huffman Decode YCbCr channels */

					// +10 because we have a 10 byte signature at the beginning.
					huffman_data_i = decompress_channel(ychannel,
							huffman_data+10, IMG_WIDTH, IMG_HEIGHT);
					huffman_data_i += decompress_channel(cbchannel,
							10+huffman_data+huffman_data_i, CHROMA_WIDTH, CHROMA_HEIGHT);
					decompress_channel(crchannel,
							10+huffman_data+huffman_data_i, CHROMA_WIDTH, CHROMA_HEIGHT);

					/* YCbCr convert to RGB and draw fill the shared VGA buffer */
					dec_ycrcb(ychannel, cbchannel, crchannel,
							SUBSAMPLE_CHROMA, 0, 10, IMG_WIDTH, IMG_HEIGHT);
#ifdef DEBUG
					uint32_t end_t = micros();
					printf("Decompression time: %dms\n\r", (end_t-start_t)/1000);
#endif
				}
				/* If frame didn't end, next frame will be written to this idx */
				else huffman_data_i += SPI_MTU;
			}
		}
	}
}

void
handle_error(void)
{
	printf("PSoC Died...\n");

     /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
}

uint32_t
micros(void)
{
	return Cy_TCPWM_Counter_GetCounter(UST_HW, UST_NUM);
}

static void
dec_ycrcb(const uint8_t* restrict y_blk, const uint8_t* restrict cb_blk, const uint8_t* restrict cr_blk,
        uint8_t subsampl_cr, uint32_t row_base, uint32_t col_base, uint32_t width, uint32_t height)
{
    for (uint32_t r = 0; r < height; r += 8) {
        for (uint32_t c = 0; c < width; c += 8) {
            dec_ycrcb_blk(y_blk + r * width + c,
                         cr_blk + (r/subsampl_cr * width/subsampl_cr) + c/subsampl_cr,
                         cb_blk + (r/subsampl_cr * width/subsampl_cr) + c/subsampl_cr,
                         subsampl_cr,
						 row_base + r, col_base + c,
                         width, height);
        }
    }
}

static inline void
dec_ycrcb_blk(const uint8_t* restrict y_blk, const uint8_t* restrict cr_blk, const uint8_t* restrict cb_blk,
        uint8_t subsampl_cr, uint32_t row_base, uint32_t col_base, uint32_t width, uint32_t height)
{
    int32_t y;
    int32_t cr;
    int32_t cb;

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
            int32_t red = ((1<<21) * y + 0       * cb + 3302595 * cr) >> 21;
            int32_t grn = ((1<<21) * y - 392796  * cb - 981677  * cr) >> 21;        // same, more, less
            int32_t blu = ((1<<21) * y + 3891475 * cb + 0       * cr) >> 21;
#endif
            red = CLAMP_F(red);		grn = CLAMP_F(grn); 	blu = CLAMP_F(blu);

            red /= (NUM_COLORS / 32);
            grn /= (NUM_COLORS / 32);
            blu /= (NUM_COLORS / 32);

            /* Once we know the RGB555 (15bit number), we write them to the shared
             * framebuffer so that DMA can output it to the three DACs over GPIO
             * Format:
             *
             * 	     Port8 (LSB->MSB)			 Port9 (LSB->MSB)
             * 	[ X B0 B1 B2 B3 B4 G0 G1 ]	[ G2 G3 G4 R0 R1 R2 R3 R4 ]
             * 	     \...DAC..../   \.....DAC......./   \...DAC..../
             * 	          |                |                 |
             * 		   Analog B         Analog G          Analog R
             */
            int _r = row_base + r;
            int _c = col_base + c;

            /* Upsample each pixel into 4 pixels */
            PUT_PIXEL(2*_r,   2*_c,   red, grn, blu)
            PUT_PIXEL(2*_r+1, 2*_c,   red, grn, blu)
            PUT_PIXEL(2*_r,   2*_c+1, red, grn, blu)
            PUT_PIXEL(2*_r+1, 2*_c+1, red, grn, blu)
        }
    }
}



/* [] END OF FILE */
