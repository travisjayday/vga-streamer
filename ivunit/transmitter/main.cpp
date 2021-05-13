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

#define MAX 6096
#define PORT 8080
#define SA struct sockaddr

#define CONFIG 0

#if CONFIG==0
#define WIDTH 160
#define HEIGHT 192
#define NUM_COLORS 32
#define SUBSAMPLE_CHROMA 4
#elif CONFIG==1
#define WIDTH 192
#define HEIGHT 224
#define NUM_COLORS 16
#define SUBSAMPLE_CHROMA 4
#elif CONFIG==2
#define WIDTH 224
#define HEIGHT 256
#define NUM_COLORS 16
#define SUBSAMPLE_CHROMA 4
#endif

#define LOCAL_SHOW 0

#define COLOR_SCALER (256/NUM_COLORS)

#include "ycrcb_decode.c"
#include "jpg.h"

unsigned long 
micros()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (1000000*tv.tv_sec) + tv.tv_usec;
}

#define RECORD_WIDTH 540
#define RECORD_HEIGHT   500

int service_client(int sockfd)
{
    //ScreenShot screen(0,50,WIDTH*1.66,HEIGHT);
    //ScreenShot screen(0,50,WIDTH,HEIGHT);
    ScreenShot screen(0,160,RECORD_WIDTH,HEIGHT);
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

        cv::imshow("0", ycrcb[0]);
        cv::imshow("1", ycrcb[1]);
        cv::imshow("2", ycrcb[2]);

        uint32_t comp_y_n = 0;
        uint32_t comp_cr_n = 0;
        uint32_t comp_cb_n = 0;
        uint8_t* comp_y  = compress_channel(&comp_y_n, y, WIDTH, HEIGHT);
        uint8_t* comp_cb = compress_channel(&comp_cb_n, cb, CHROM_WIDTH, CHROM_HEIGHT);
        uint8_t* comp_cr = compress_channel(&comp_cr_n, cr, CHROM_WIDTH, CHROM_HEIGHT);
        
        // compressed_image goes down the wire...
        uint32_t total_size = comp_y_n + comp_cr_n + comp_cb_n; 

#if LOCAL_SHOW==1
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
#endif
        cv::imshow("original", bgr);

        char k = cv::waitKey(1);
        if (k == 'q') break;

#if LOCAL_SHOW==0
        char* sig = "FRSTART";
        uint16_t size = comp_y_n + comp_cb_n + comp_cr_n;  
        int preamble_size1 = strlen(sig) + 1;
        int preamble_size2 = strlen(sig) + 1 + sizeof(uint16_t);
        memcpy(sendbuf,                                          sig,     preamble_size1);
        memcpy(sendbuf + preamble_size1,                         &size,   sizeof(uint16_t));
        memcpy(sendbuf + preamble_size2,                         comp_y,  comp_y_n);
        memcpy(sendbuf + preamble_size2 + comp_y_n,              comp_cb, comp_cb_n);
        memcpy(sendbuf + preamble_size2 + comp_y_n + comp_cb_n,  comp_cr, comp_cr_n);

        while (micros() - last_fr_time >= 48000) {
            last_fr_time = micros();
            write(sockfd, sendbuf, preamble_size2 + comp_y_n + comp_cb_n + comp_cr_n);

            printf("Original size: %d\n", WIDTH * HEIGHT * 3); 
            printf("Compressed size: %d\n", total_size);
            printf("Compression factor: %.2f\n", (float) total_size / (WIDTH * HEIGHT * 3));
            printf("Sent bytes:");
            for (int i = 0; i < 10; i++) printf("0x%x", comp_y[i]); 
            printf(" ... ");
            for (int i = comp_y_n - 10; i < comp_y_n; i++) printf("0x%x", comp_y[i]); 
            int sum = 0;
            for (int i = 0; i < comp_cr_n + comp_cb_n + comp_y_n + strlen(sig) + 1 + sizeof(uint16_t); i++) 
                sum += sendbuf[i];
            printf("Frame huffman coded DCT Checksum: %d\n", sum); 
            printf("\n");
        }
#endif
        free(comp_y);
        free(comp_cb);
        free(comp_cr);
    }
}

 // Driver function
int main()
{
#if LOCAL_SHOW
    service_client(0);
#endif

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

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.152");
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

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

    // Function for chatting between client and server
    service_client(connfd);

    // After chatting close the socket
    close(sockfd);
}
