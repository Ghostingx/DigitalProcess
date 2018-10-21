#include <iostream>
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>

int cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale);

int main(int argc,char **argv) {
    if(argc < 2){
        fprintf(stderr,"缺少参数\n");
        return -1;
    }
    char *fileName = argv[1];
    cv::Mat im = cv::imread(fileName);
    if(im.empty()){
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    cv::namedWindow("display image",cv::WINDOW_AUTOSIZE);

    cv::imshow("C",im);
    cv::waitKey(0);
    return 0;

}
