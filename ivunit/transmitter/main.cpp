#include <opencv2/opencv.hpp>
#include "screenshot.cpp"
#include <iostream>
#include "stdio.h"
#include <time.h>

#define WIDTH 128
#define HEIGHT 128
#define NUM_COLORS 32
#define SUBSAMPLE_CHROMA 4

#define SHOW_LOCAL 0

#define COLOR_SCALER (256/NUM_COLORS)

#include "ycrcb_decode.c"
#include "jpg.h"

unsigned long 
micros()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

int service_client(int sockfd)
{
    ScreenShot screen(500,500,WIDTH,HEIGHT);
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

    struct pix* pixbuf = (struct pix*) malloc( WIDTH * HEIGHT * sizeof(struct pix));

    int subsample_chrom = SUBSAMPLE_CHROMA; 
    const int CHROM_WIDTH = WIDTH / subsample_chrom;
    const int CHROM_HEIGHT = HEIGHT / subsample_chrom;

    cv::Size chrom_size(CHROM_HEIGHT, CHROM_WIDTH);
    uint8_t* decoded_y  = (uint8_t*) calloc(1, WIDTH * HEIGHT * sizeof(uint8_t*));
    uint8_t* decoded_cr = (uint8_t*) calloc(1, WIDTH * HEIGHT * sizeof(uint8_t*));
    uint8_t* decoded_cb = (uint8_t*) calloc(1, WIDTH * HEIGHT * sizeof(uint8_t*));
    unsigned long last_fr_time = micros();

    while(true) 
    {
        /* Capture screen */
        screen(bgr);

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
        cb = ycrcb[1].data; 
        cr = ycrcb[0].data;

        /*
        cv::imshow("0", ycrcb[0]);
        cv::imshow("1", ycrcb[1]);
        cv::imshow("2", ycrcb[2]);
        */

        uint32_t comp_y_n = 0;
        uint32_t comp_cr_n = 0;
        uint32_t comp_cb_n = 0;
        uint8_t* comp_y  = compress_channel(&comp_y_n, y, WIDTH, HEIGHT);
        uint8_t* comp_cb = compress_channel(&comp_cb_n, cb, CHROM_WIDTH, CHROM_HEIGHT);
        uint8_t* comp_cr = compress_channel(&comp_cr_n, cr, CHROM_WIDTH, CHROM_HEIGHT);
        
        // compressed_image goes down the wire...
        uint32_t total_size = comp_y_n + comp_cr_n + comp_cb_n; 
        printf("Original size: %d\n", WIDTH * HEIGHT * 3); 
        printf("Compressed size: %d\n", total_size);
        printf("Compression factor: %.2f\n", (float) total_size / (WIDTH * HEIGHT * 3));
        printf("Sent bytes:");
        for (int i = 0; i < 10; i++) printf("0x%x", comp_y[i]); 
        printf(" ... ");
        for (int i = comp_y_n - 10; i < comp_y_n; i++) printf("0x%x", comp_y[i]); 
        printf("\n");
        printf("\e[1;1H\e[2J");

#if LOCAL_SHOW
        /* Decompress image data */
        uint32_t data_i = decompress_channel(decoded_y, comp_y, WIDTH, HEIGHT);
        data_i += decompress_channel(decoded_cb, comp_cb, CHROM_WIDTH, CHROM_HEIGHT);
        data_i += decompress_channel(decoded_cr, comp_cr, CHROM_WIDTH, CHROM_HEIGHT);

        /* Display result */
        cv::Mat outy(HEIGHT, WIDTH, CV_8UC1, (uchar*) decoded_y);
        cv::Mat outcr(CHROM_HEIGHT, CHROM_WIDTH, CV_8UC1, (uchar*) decoded_cr);
        cv::Mat outcb(CHROM_HEIGHT, CHROM_WIDTH, CV_8UC1, (uchar*) decoded_cb);

        /* Decode YCrCB */
        dec_ycrcb(decoded_y, decoded_cb, decoded_cr, subsample_chrom, pixbuf, WIDTH, HEIGHT);
        cv::Mat out(HEIGHT, WIDTH, CV_8UC3, (uchar*) pixbuf);

        cv::imshow("decompressed", out);
        cv::imshow("original", bgr);

        cv::resize(out, out, cv::Size(640, 640));
        cv::imshow("upscaled", out);
#endif

        /*
        cv::imshow("outy", outy);
        cv::imshow("outcr", outcr);
        cv::imshow("outcb", outcb);

        char k = cv::waitKey(1);
        if (k == 'q') break;
        */

        while (micros() - last_fr_time >= 16000) {
            last_fr_time = micros();
            write(sockfd, comp_y);
        }
    }
}

 // Driver function
int main()
{

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

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
