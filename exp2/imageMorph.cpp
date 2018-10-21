#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
Mat imageMorph(string _imageName){
    Mat inputImg = imread(_imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << _imageName << endl;
    }
    else {

        Mat outputImg(inputImg.rows,inputImg.cols,CV_8UC3);

        for (int i = 0; i < outputImg.rows; i++) {
            for (int j = 0; j < outputImg.cols; j++) {

                Vec3b outputP = outputImg.at<Vec3b>(i, j);

                double x = ((double)j - 0.5*outputImg.cols) / (0.5*outputImg.cols);
                double y = ((double)i - 0.5*outputImg.rows) / (0.5*outputImg.rows);


                double r = sqrt(pow(x,2)+pow(y,2));
                double o = pow((1 - r), 2);

                if (r >= 1) {
                    outputP = inputImg.at<Vec3b>(i, j);
                }
                else {

                    double ix = (cos(o)*x - sin(o)*y)*0.5*outputImg.cols+0.5*outputImg.cols;
                    double iy = (sin(o)*x + cos(o)*y)*0.5*outputImg.rows+0.5*outputImg.rows;

                    if (ix < inputImg.cols&&iy < inputImg.rows) {
                        outputP = inputImg.at<Vec3b>(iy, ix);
                    }
                    else {
                        outputP = inputImg.at<Vec3b>(inputImg.rows-1, inputImg.cols - 1);
                    }
                }
                outputImg.at<Vec3b>(i, j) = outputP;
            }
        }
        return outputImg;
    }
}

int main(int argc,char **argv){
    string imageName = "A.png";
    Mat inputImg = imread(imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << imageName << endl;
    }
    namedWindow("dis",WINDOW_AUTOSIZE);
    imshow("src", inputImg);
    imshow("dis", imageMorph(imageName));
    waitKey();
    return 0;
}
