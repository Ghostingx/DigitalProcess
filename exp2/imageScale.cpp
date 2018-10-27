#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
void imageScale(const Mat & _input, Mat & _output,double _sx,double _sy){
    int srccol = 0, srcrow = 0;
    double alpha = 0.0, beta = 0.0;

    for (int i = 0; i < _output.rows; i++) {
        //get the get the row pointer
        uchar* desdata = _output.ptr<uchar>(i);
        for (int j = 0; j <_output.cols; j++) {
            //get the integer row of soruce image  coors
            srcrow = int(i / _sx);
            if (srcrow >= _input.rows - 1) {
                srcrow = _input.rows - 2;
            }
            alpha = i / _sx - srcrow;
            if (alpha >= 1)
                alpha = 1;
            srccol = int(j / _sy);
            if (srccol >= _input.cols - 1)
                srccol = _input.cols - 2;
            beta = j / _sy - srccol;
            if (beta >= 1)
                beta = 1;
            //calculate
            for (int k = 0; k < 3; k++) {
                double kk = _input.at<Vec3b>(srcrow, srccol)[k] +
                    beta*(_input.at<Vec3b>(srcrow, srccol + 1)[k] - _input.at<Vec3b>(srcrow, srccol)[k]);
                double jj = _input.at<Vec3b>(srcrow + 1, srccol)[k] +
                    beta*(_input.at<Vec3b>(srcrow + 1, srccol + 1)[k] - _input.at<Vec3b>(srcrow + 1, srccol)[k]);
                desdata[j * 3 + k] = kk + alpha*(jj - kk);
            }
        }
    }
    return ;
}

int main(int argc,char **argv){
    string imageName = "A.png";
    Mat inputImg = imread(imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << imageName << endl;
    }
    else {
        double sx = 0.6;
        double sy = 0.6;
        namedWindow("dis",WINDOW_AUTOSIZE);
        imshow("scale", inputImg);
        int y = int(inputImg.cols*sy);
        int x = int(inputImg.rows*sx);
        Mat outputImg(x, y, CV_8UC3, Scalar::all(0));
        imageScale(inputImg, outputImg, sx, sy);
        imshow("", outputImg);
        waitKey();
        return 0;
    }
}
