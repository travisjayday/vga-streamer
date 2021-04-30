#include <opencv2/opencv.hpp>
#include "screenshot.cpp"
#include <iostream>
#include "stdio.h"
#include "ycrcb_decode.c"

#define WIDTH 256
#define HEIGHT 256

int main()
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

    int subsample_chrom = 8; 
    cv::Size chrom_size(HEIGHT / subsample_chrom, WIDTH / subsample_chrom);

    while(true) 
    {
        screen(bgr);

        cv::cvtColor(bgr, fimg, cv::COLOR_BGRA2BGR);
        fimg.convertTo(fimg, CV_32FC3); 
        tmp = fimg.reshape(1, HEIGHT*WIDTH);
        tmp = tmp*M;
        img = tmp.reshape(3, HEIGHT);

        img.convertTo(img, CV_32FC3, 1/255.0);

        cv::split(img, ycrcb);
        ycrcb[0] += 0.5f;
        ycrcb[1] += 0.5f;
        cv::merge(ycrcb, img);

        img.convertTo(img, CV_8UC3, 255); 

        cv::split(img, ycrcb);

        cv::resize(ycrcb[1], ycrcb[1], chrom_size);
        cv::resize(ycrcb[0], ycrcb[0], chrom_size);

        y  = ycrcb[2].data; 
        cb = ycrcb[1].data; 
        cr = ycrcb[0].data;

        cv::imshow("0", ycrcb[0]);
        cv::imshow("1", ycrcb[1]);
        cv::imshow("2", ycrcb[2]);

        dec_ycrcb(y, cb, cr, subsample_chrom, pixbuf, WIDTH, HEIGHT);

        cv::Mat out(HEIGHT, WIDTH, CV_8UC3, (uchar*) pixbuf);

        cv::imshow("win", out);
        cv::imshow("orig", bgr);
        

        char k = cv::waitKey(1);
        if (k == 'q')
            break;
    }
}
