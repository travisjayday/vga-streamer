#include <opencv2/opencv.hpp>
#include "screenshot.cpp"
#include <iostream>
#include "stdio.h"
#include <time.h>
#include "sys/time.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <string.h>

#define MAX 6096
#define PORT 8080
#define SA struct sockaddr

#define CONFIG 0

#if CONFIG==0
#define WIDTH 160
#define HEIGHT 128
#define NUM_COLORS 128
#define SUBSAMPLE_CHROMA 4
#elif CONFIG==1
#define WIDTH 192
#define HEIGHT 224
#define NUM_COLORS 32
#define SUBSAMPLE_CHROMA 4
#elif CONFIG==2
#define WIDTH 224
#define HEIGHT 256
#define NUM_COLORS 32
#define SUBSAMPLE_CHROMA 4
#elif CONFIG==3
#define WIDTH 256
#define HEIGHT 256
#define NUM_COLORS 256
#define SUBSAMPLE_CHROMA 1
#endif

#define LOCAL_SHOW 0
#define SAVE_IMG_AS_ARR 0

#define COLOR_SCALER (256/NUM_COLORS)

#include "ycrcb_decode.c"
#include "jpg.h"


int RECORD_X        = 175;
int RECORD_Y        = 175;
int RECORD_WIDTH    = WIDTH;
int RECORD_HEIGHT   = HEIGHT;

unsigned long 
micros()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (1000000*tv.tv_sec) + tv.tv_usec;
}

int service_client(int sockfd)
{
    //ScreenShot screen(0,50,WIDTH*1.66,HEIGHT);
    //ScreenShot screen(0,50,WIDTH,HEIGHT);
    ScreenShot screen(RECORD_X,RECORD_Y,RECORD_WIDTH,RECORD_HEIGHT);
    cv::Mat img;
    cv::Mat tmp;
    cv::Mat fimg;
    cv::Mat bgr;

    std::vector<cv::Mat> ycrcb;  

    /*The YCBCR BT.709 Color matrix
     * Red:RGB; Green:RGB; Blue:RGB
     * 0.2126   0.7152  0.0722
     * -0.1146  -0.3854 0.5
     *  0.5     -0.4542 -0.0458
     *  */
    float m[3][3] = {{-0.0458, -0.4542, 0.5},
                    {0.5, -0.3854, -0.1146}, 
                    {0.0722, 0.7152, 0.2126 }};
    cv::Mat M = cv::Mat(3, 3, CV_32FC1, m).t();

    uint8_t* y;
    uint8_t* cb;
    uint8_t* cr;

    float* yf;
    float* cbf;
    float* crf;

    struct pix* pixbuf = (struct pix*) malloc(WIDTH * HEIGHT * sizeof(struct pix));

    int subsample_chrom = SUBSAMPLE_CHROMA; 
    const int CHROM_WIDTH = WIDTH / subsample_chrom;
    const int CHROM_HEIGHT = HEIGHT / subsample_chrom;

    cv::Size chrom_size(CHROM_WIDTH, CHROM_HEIGHT);
    uint8_t* decoded_y  = (uint8_t*) calloc(1, WIDTH * HEIGHT * sizeof(uint8_t*));
    uint8_t* decoded_cr = (uint8_t*) malloc(WIDTH * HEIGHT * sizeof(uint8_t*));
    uint8_t* decoded_cb = (uint8_t*) malloc(WIDTH * HEIGHT * sizeof(uint8_t*));
    unsigned long last_fr_time = micros();
    uint8_t* sendbuf = (uint8_t*) malloc(256 * 256); 

    while(true) 
    {
        /* Capture screen */
        screen(bgr);
        cv::resize(bgr, bgr, cv::Size(WIDTH, HEIGHT));

        /* Convert Image to YCrCB */
        cv::cvtColor(bgr, fimg, cv::COLOR_BGRA2BGR);
        fimg.convertTo(fimg, CV_32FC3); 
        tmp = fimg.reshape(1, HEIGHT*WIDTH);
        tmp = tmp*M;
        img = tmp.reshape(3, HEIGHT);
        img.convertTo(img, CV_32FC3, 1/255.0);

        /* Downsample Chrome Channels */
        cv::split(img, ycrcb);
        ycrcb[0] += 0.5f;
        ycrcb[1] += 0.5f;
        cv::merge(ycrcb, img);
        img.convertTo(img, CV_8UC3, NUM_COLORS); 
        cv::split(img, ycrcb);
        cv::resize(ycrcb[1], ycrcb[1], chrom_size);
        cv::resize(ycrcb[0], ycrcb[0], chrom_size);

        /* Transmit YCrCb */
        y  = ycrcb[2].data; 
        cb = ycrcb[1].data; // these are correct
        cr = ycrcb[0].data;

        uint32_t comp_y_n = 0;
        uint32_t comp_cr_n = 0;
        uint32_t comp_cb_n = 0;
        uint8_t* comp_y  = compress_channel(&comp_y_n, y, WIDTH, HEIGHT);
        uint8_t* comp_cb = compress_channel(&comp_cb_n, cb, CHROM_WIDTH, CHROM_HEIGHT);
        uint8_t* comp_cr = compress_channel(&comp_cr_n, cr, CHROM_WIDTH, CHROM_HEIGHT);
        
        // compressed_image goes down the wire...
        uint32_t total_size = comp_y_n + comp_cr_n + comp_cb_n; 

#if LOCAL_SHOW==1
        cv::imshow("Chrominance (Cr)", ycrcb[0]);
        cv::imshow("Chrominance (Cb)", ycrcb[1]);
        cv::imshow("Luminance (Y)", ycrcb[2]);

        /* Decompress image data */
        // this creates incorrect cb,cr
        uint32_t data_i = decompress_channel(decoded_y, comp_y, WIDTH, HEIGHT);
        data_i += decompress_channel(decoded_cb, comp_cb, CHROM_WIDTH, CHROM_HEIGHT);
        data_i += decompress_channel(decoded_cr, comp_cr, CHROM_WIDTH, CHROM_HEIGHT);

        /* Display result */
        cv::Mat outy(HEIGHT, WIDTH, CV_8UC1, (uchar*) decoded_y);
        cv::Mat outcr(CHROM_HEIGHT, CHROM_WIDTH, CV_8UC1, (uchar*) decoded_cr);
        cv::Mat outcb(CHROM_HEIGHT, CHROM_WIDTH, CV_8UC1, (uchar*) decoded_cb);

        /* Decode YCrCB */
        // or this
        dec_ycrcb(decoded_y, decoded_cb, decoded_cr, subsample_chrom, pixbuf, WIDTH, HEIGHT);
        //dec_ycrcb(y, cb, cr, subsample_chrom, pixbuf, WIDTH, HEIGHT);
        cv::Mat out(HEIGHT, WIDTH, CV_8UC3, (uchar*) pixbuf);// this is correct

        cv::imshow("decompressed", out);

        cv::resize(out, out, cv::Size(640, 640));
        cv::imshow("upscaled", out);
        cv::imshow("outy", outy);

        cv::imshow("outcr", outcr);
        cv::imshow("outcb", outcb);

        cv::imshow("original", bgr);

        char k = cv::waitKey(1);
        if (k == 'q') break;
#endif

#if SAVE_IMG_AS_ARR==1
        if (k == 's') {
            printf("Printing image...\n");
            printf("#ifdef BENCH\n");
            printf("char test_comp_y[] = {");
            for (int i = 0; i < comp_y_n; i++) printf("0x%x,", comp_y[i]);
            printf("};\n");
            printf("char test_comp_cb[] = {");
            for (int i = 0; i < comp_cb_n; i++) printf("0x%x,", comp_cb[i]);
            printf("};\n");
            printf("char test_comp_cr[] = {");
            for (int i = 0; i < comp_cr_n; i++) printf("0x%x,", comp_cr[i]);
            printf("};\n");
            printf("#endif\n");
            fflush(stdout);
        }    
#endif

#if LOCAL_SHOW==0
        char* sig = "FRSTART";
        int preamble_size1 = strlen(sig) + 1;
        int preamble_size2 = strlen(sig) + 1 + sizeof(uint16_t);

        uint16_t size = comp_y_n + comp_cb_n + comp_cr_n;  
        memset(sendbuf, 0, 4096);
        memcpy(sendbuf,                                          sig,     preamble_size1);
        memcpy(sendbuf + preamble_size1,                         &size,   sizeof(uint16_t));
        memcpy(sendbuf + preamble_size2,                         comp_y,  comp_y_n);
        memcpy(sendbuf + preamble_size2 + comp_y_n,              comp_cb, comp_cb_n);
        memcpy(sendbuf + preamble_size2 + comp_y_n + comp_cb_n,  comp_cr, comp_cr_n);
     
        while (micros() - last_fr_time >= 28 * 1000) {
            printf("Time since last: %d\n", micros() - last_fr_time);
            last_fr_time = micros();

            int send_size = preamble_size2 + comp_y_n + comp_cb_n + comp_cr_n;
            write(sockfd, sendbuf, send_size);

#ifdef DEBUG
            printf("Original size: %d\n", WIDTH * HEIGHT * 3); 
            printf("Compressed size: %d\n", total_size);
            printf("Compression factor: %.2f\n", (float) total_size / (WIDTH * HEIGHT * 3));
            printf("Sent num bytes: %d\n", send_size);
            printf("Sent bytes:");
            for (int i = 0; i < 10; i++) printf("0x%x", comp_y[i]); 
            printf(" ... ");
            for (int i = comp_y_n - 10; i < comp_y_n; i++) printf("0x%x", comp_y[i]); 
            int sum = 0;
            int total_len = comp_cr_n + comp_cb_n + comp_y_n + strlen(sig) + 1 + sizeof(uint16_t);
            for (int i = 0; i < total_len; i++) 
                sum += sendbuf[i];
            int k = 0;
            while (total_len > 0) {
                if (total_len > 512) {
                    int sum  = 0;
                    for (int i = 0; i < 512; i++)
                        sum += sendbuf[k++]; 
                    printf("ChecksumMTU: %d\n", sum);
                    total_len -= 512;
                }
                else {
                    int sum  = 0;
                    for (int i = 0; i < 512; i++)
                        sum += sendbuf[k++]; 
                    printf("ChecksumMTU_fin: %d\n", sum);
                    total_len = 0; 
                }
            }
            printf("Frame huffman coded DCT Checksum: %d\n", sum); 
            printf("\n");
#endif
        }
#endif
        free(comp_y);
        free(comp_cb);
        free(comp_cr);
    }
}

 // Driver function
int main(int argc, char* argv[])
{
    std::cout << cv::getBuildInformation() << std::endl;

    char* ip_str = (char*) malloc(64);

    printf("\nPSoC/ESP Vga Streamer Server\n"
            "-------------------------------\n"
            "Usage: ./server.out IP_ADDR RECORD_X RECORD_Y "
            "RECORD_WIDTH RECORD_HEIGHT\n\n");

    /* Check commandline args for custom IP */
    if (argc > 1) strcpy(ip_str, argv[1]);
    else strcpy(ip_str, "192.168.1.152");

    if (argc == 1 + 1 + 4) {
        RECORD_X = strtol(argv[2], NULL, 10);
        RECORD_Y = strtol(argv[3], NULL, 10);
        RECORD_WIDTH  = strtol(argv[4], NULL, 10);
        RECORD_HEIGHT = strtol(argv[5], NULL, 10);
        printf("Will stream geometry %dx%d at (%d, %d)\n", 
                RECORD_WIDTH, RECORD_HEIGHT, RECORD_X, RECORD_Y);
    }

#if LOCAL_SHOW
    service_client(0);
#endif

    printf("Will host on %s...\n", ip_str);

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_str);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }


    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }


    // Function for chatting between client and server
    service_client(connfd);

    // After chatting close the socket
    close(sockfd);
}
