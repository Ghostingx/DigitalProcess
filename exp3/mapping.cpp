#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat mapping(Mat simg)
{
    Mat dimg;
    simg.copyTo(dimg);
    Mat *temp = new Mat[simg.channels()];
    split(simg, temp);
    int *a = new int[256];
    int *b = new int[256];

    for (int n = 0; n < simg.channels(); n++)
    {
        for (int i = 0; i < 256; i++)
        {
            a[i] = 0;
            b[i] = 0;
        }
        for (int x = 0; x < temp[n].rows; x++)
        {
            for (int y = 0; y < temp[n].cols; y++)
            {
                a[(int)temp[n].ptr(x, y)[0]] = a[(int)temp[n].ptr(x, y)[0]] + 1;
            }
        }
        for (int j = 0; j < 256; j++)
        {
            for (int i = 0; i < j; i++)
            {
                b[j] = b[j] + a[i];
            }
            double d = b[j];
            b[j]=d/ (temp[n].rows*temp[n].cols) * 255;
        }
        for (int x = 0; x < temp[n].rows; x++)
        {
            for (int y = 0; y < temp[n].cols; y++)
            {
                dimg.ptr(x, y)[n]= b[(int)temp[n].ptr(x, y)[0]];
            }
        }
    }
    return dimg;
}

int main()
{
    Mat img = imread("A.png");
    imshow("src", img);
    imshow("dest", mapping(img));
    waitKey();
    return 0;
}

