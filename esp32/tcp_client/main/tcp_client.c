/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "soc/rtc_cntl_reg.h"
#include "rom/cache.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "esp_spi_flash.h"

#include "driver/uart.h"


/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

#ifdef CONFIG_EXAMPLE_IPV4
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#else
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#endif

#define PORT CONFIG_EXAMPLE_PORT

#define GPIO_HANDSHAKE 21
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 5
#define SPI_MTU 512

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;

static const char *TAG = "example";
static const char *payload = "Message from ESP32 ";

DMA_ATTR char spibuf[SPI_MTU]="";
DMA_ATTR char recvbuf[SPI_MTU]="";

char* server_ip_addr; 

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<GPIO_HANDSHAKE));
}

//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<GPIO_HANDSHAKE));
}


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        /* enable ipv6 */
        tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
        xEventGroupClearBits(wifi_event_group, IPV6_GOTIP_BIT);
        break;
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
        xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

        char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
        ESP_LOGI(TAG, "IPv6: %s", ip6);
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void wait_for_ip()
{
    uint32_t bits = IPV4_GOTIP_BIT | IPV6_GOTIP_BIT ;

    ESP_LOGI(TAG, "Waiting for AP connection...");
    xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");
}

static void tcp_client_task(void *pvParameters)
{
#define TCP_RX_SIZE 2*8192
    char* rx_buffer = malloc(TCP_RX_SIZE);
    char addr_str[512];
    int addr_family;
    int ip_protocol;

    while (1) {

        /* Establish TCP Connection with PC */
        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = inet_addr(server_ip_addr);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(8080);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");
        int err = connect(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Successfully connected");

        /* Create performance timers */
        struct timeval st, et, fr_st, fr_et;
        gettimeofday(&st, NULL);
        gettimeofday(&fr_st, NULL);
        int recvd_total = 0;

        /* Create SPI buffers and structs */
        memset(recvbuf, 0, 33);
        spi_slave_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length =8 * SPI_MTU;
        t.rx_buffer = recvbuf;
        t.tx_buffer = spibuf;

        /* Create Buffer pointers */
        int next_fr_size = 0;           // The amount of data we still need to receive to get a full frame 
        int spi_consumed_fr_size = 0;   // The amount of data we still need to transmit over SPI 
        char* write_ptr = rx_buffer;    // New TCP data will get written to here
        char* read_ptr = rx_buffer;     // Old TCP data will get read from here and sent over SPI

        /* New incoming frame signature */
        const char* sig = "FRSTART";

//#define DEBUG_FRAME
#ifdef DEBUG_FRAME
        char* debug_frame = malloc(SPI_MTU * 4);
        int debug_frame_i = 0; 
#endif

        int got_first_frame = 0;

        while (1) {
                      
            // Get TCP Data 
            int to_recv = next_fr_size != 0 && next_fr_size < SPI_MTU? next_fr_size : SPI_MTU;
            
#ifdef DEBUG_FRAME
            printf("Tryign to recv %d\n", to_recv);
#endif
            int recvd = 0;
            char* write_ptr_copy = write_ptr;
            do {
                int r = recv(sock, write_ptr_copy, to_recv - recvd, 0);
                write_ptr_copy += r;
                recvd += r;
#ifdef DEBUG_FRAME
                printf("Received %d. Have %d/%d", r, recvd, to_recv);
#endif
            } while (recvd != to_recv);

            // Check if a new frame started
            if (strcmp(write_ptr, sig) == 0) {
                got_first_frame = 1;

                // If the amount of data we need to transmit for 
                // current frame is not zero, we are lagging. 
                if (next_fr_size > 0) {
                    // Drop the next frame. 
                    printf("Dropping Frame...\n");
                }
                else {
                    // Get the next frame size from the rxbuffer which looks 
                    // like "FRSTART\0XX[...]" were XX is a UINT16 and [...] 
                    // is the frame data
                    next_fr_size = (int) *((uint16_t*) (write_ptr + strlen(sig) + 1)); 
                    next_fr_size += strlen(sig) + 1 + sizeof(uint16_t);
                    spi_consumed_fr_size = next_fr_size;

                    // Reset Read/Write pointers
                    read_ptr = rx_buffer; 
                    write_ptr = rx_buffer; 

                    // Collect Debug information 
#ifdef DEBUG_FRAME
                    gettimeofday(&fr_et, NULL);
                    float elapsed_ms = (((fr_et.tv_sec - fr_st.tv_sec) * 1000000) 
                            + (fr_et.tv_usec - fr_st.tv_usec)) / 1000.0f;
                    gettimeofday(&fr_st, NULL);

                    printf("Next frame size %d! Elapsed since last: %0.2fms\n", 
                            (int) next_fr_size, elapsed_ms); 
                    int sum = 0; 
                    for (int i = 0; i < debug_frame_i; i++) {
                        sum += debug_frame[i]; 
                    }
                    printf("Frame Huffman Coded DCT Checksum: %d\n", sum); 
                    debug_frame_i = 0; 
#endif
                }
            }
            if (got_first_frame == 0) {
                write_ptr = rx_buffer;
                continue;
            }

            // move write_ptr to next block 
            write_ptr += recvd; 
            recvd_total += recvd; 
            if (write_ptr - rx_buffer > TCP_RX_SIZE) 
                printf("Buffer ofverflow\n");

            next_fr_size -= recvd;

            // Check if current frame still needs bytes to be sent over SPI.
            if (spi_consumed_fr_size > 0) {
#ifdef DEBUG_FRAME
                printf("spi_consumed_fr_size: %d\n", spi_consumed_fr_size);
#endif
                if (spi_consumed_fr_size >= SPI_MTU && 
                        write_ptr - read_ptr >= SPI_MTU) {
                    // We have enough data, so 
                    // write the data that came in into the spi buffer 
                    memcpy(spibuf, read_ptr, SPI_MTU); 
                    
                    // send the chunk over SPI 
                    spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
                     
                    read_ptr += SPI_MTU;
                    spi_consumed_fr_size -= SPI_MTU;
#ifdef DEBUG_FRAME
                    int sum = 0;
                    for (int i = 0; i < SPI_MTU; i++) sum += read_ptr[i]; 
                    printf("ChecksumMTU: %d\n", sum);
                    memcpy(debug_frame + debug_frame_i, spibuf, SPI_MTU); 
                    debug_frame_i += SPI_MTU;
#endif
                }
                else if (spi_consumed_fr_size < SPI_MTU && 
                        write_ptr - read_ptr >= spi_consumed_fr_size) {
                    // empty the buffer and pad with zeros
                    memset(spibuf, 0, SPI_MTU); 
                    memcpy(spibuf, read_ptr, spi_consumed_fr_size); 

                    // send the last chunk over SPI 
                    spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);

                    read_ptr += spi_consumed_fr_size; 
                    spi_consumed_fr_size = 0; 

                    write_ptr = rx_buffer;
#ifdef DEBUG_FRAME
                    int sum = 0;
                    for (int i = 0; i < spi_consumed_fr_size; i++) sum += read_ptr[i]; 
                    printf("ChecksumMTU_Fin: %d\n", sum);
                    memcpy(debug_frame + debug_frame_i, spibuf, SPI_MTU); 
                    debug_frame_i += SPI_MTU;
#endif
                }
                else {
                    // Not enough TCP data available
                }
            }
            else {
                // No new data needed being written, we're chillin until next frame
            }

#if BENCHMARK
            // Error occured during receiving
            if (recvd_total > 10e6) {
                gettimeofday(&et, NULL);
                int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
                double elapsed_seconds = (double) elapsed / 1.0e6;
                ESP_LOGI(TAG, "Bytes Received: %d", recvd_total);
                ESP_LOGI(TAG, "Elapsed Seconds: %f seconds", elapsed_seconds);
                ESP_LOGI(TAG, "Speed: %f KB/s", 
                        (double) recvd_total / elapsed_seconds / 1024);

                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[recvd] = 0; // Null-terminate whatever we received and treat like a string
                recvd_total += recvd;
                //ESP_LOGI(TAG, "Received %d bytes.", recvd_total);
            }
#endif

        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void initialise_spi() {
    int n = 0;
    esp_err_t ret;
    printf("Initting SPI...\n");

    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=1,
        .spics_io_num=GPIO_CS,
        .queue_size=3,
        .flags=0,
        .post_setup_cb=my_post_setup_cb,
        .post_trans_cb=my_post_trans_cb
    };

    //Configuration for the handshake line
    gpio_config_t io_conf={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=(1<<GPIO_HANDSHAKE)
    };

    //Configure handshake line as output
    gpio_config(&io_conf);
    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Initialize SPI slave interface
    ret=spi_slave_initialize(VSPI_HOST, &buscfg, &slvcfg, 1);
    assert(ret==ESP_OK);
}

void initialise_uart(void)
{
    const int uart_buffer_size = (1024 * 2);
    char* uart0_queue = malloc(uart_buffer_size);

    /* Configure parameters of an UART driver, communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);

    //Set UART pins (using UART0 default pins ie no changes.)
    ESP_ERROR_CHECK( uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                                    UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) );

    //Install UART driver, and get the queue.
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0, uart_buffer_size, \
                    uart_buffer_size, 20, &uart0_queue, 0) );
}

char* get_ip_from_uart() {
	int size;
	char* ptr = malloc(128);
    char* str = ptr;

    struct timeval st, ct;
    gettimeofday(&st, NULL);
    gettimeofday(&ct, NULL);

	while (ct.tv_sec - st.tv_sec <= 3) {
        gettimeofday(&ct, NULL);
		size = uart_read_bytes(UART_NUM_0, (unsigned char *)ptr, 1, 1000 / portTICK_RATE_MS);
		if (size == 1) {
            gettimeofday(&st, NULL);
            printf("%c", *ptr);
            fflush(stdout);
			if (*ptr == '\n' || *ptr == '\r') {
				*ptr = 0;
                printf("\n");
				return str;
			}
			ptr++;
        }
    }
    return "192.168.1.152"; 
}

void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_wifi();
    wait_for_ip();
    initialise_spi();
    initialise_uart();

    printf("What ip to connnect to? [Default=192.168.1.152]?\n\r");
    server_ip_addr = get_ip_from_uart();
    printf("Using server ip: %s\n\r", server_ip_addr);

    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
