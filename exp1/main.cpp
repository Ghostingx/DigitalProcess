#include <iostream>
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>

int cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale);

int main(int argc , char **argv) {
    char * fileName = argv[1];
    char * bg = argv[2];
    cv::Mat im = cv::imread(fileName,-1);
    cv::Mat im_b = cv::imread(bg);
    if(im.empty() || im_b.empty()){
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    cv::Mat img1_t1(im_b, cvRect(0, 0, im.cols, im.rows));
    cvAdd4cMat_q(img1_t1,im,1.0);

    cv::namedWindow("display image",cv::WINDOW_AUTOSIZE);


    cv::imshow("C",im_b);
    cv::waitKey(0);
    return 0;
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}

int cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale)
{
    if (dst.channels() != 3 || scr.channels() != 4)
    {
        return true;
    }
    if (scale < 0.01)
        return false;
    std::vector<cv::Mat>scr_channels;
    std::vector<cv::Mat>dstt_channels;
    cv::split(scr, scr_channels);
    cv::split(dst, dstt_channels);
    CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

    if (scale < 1)
    {
        scr_channels[3] *= scale;
        scale = 1;
    }
    for (int i = 0; i < 3; i++)
    {
        dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
        dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
    }
    cv::merge(dstt_channels, dst);
    return true;
}
