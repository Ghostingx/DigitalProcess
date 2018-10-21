#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main(int argc,char **argv){
    imageName = "A.png";
    Mat inputImg = imread(imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << imageName << endl;
    }
    else {
        double sx = 0.6;
        double sy = 0.6;
        namedWindow("dis",WINDOW_AUTOSIZE);
        imshow("scale", inputImg);
        y = int(inputImg.cols*sy);
        x = int(inputImg.rows*sx);
        Mat outputImg(x, y, CV_8UC3, Scalar::all(0));
        scale(inputImg, outputImg, sx, sy);
        imshow("", outputImg);
        waitKey();
        return 0;
    }
}

