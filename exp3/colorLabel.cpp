#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

void pass(const Mat& simg, Mat& dimg)
{
    if (simg.empty() ||
        simg.type() != CV_8UC1)
    {
        return;
    }

    dimg.release();
    simg.convertTo(dimg, CV_32SC1);

    int label = 1;
    vector<int> labelSet;
    labelSet.push_back(0);
    labelSet.push_back(1);

    int rows = simg.rows - 1;
    int cols = simg.cols - 1;
    for (int i = 1; i < rows; i++)
    {
        int* data_preRow = dimg.ptr<int>(i - 1);
        int* data_curRow = dimg.ptr<int>(i);
        for (int j = 1; j < cols; j++)
        {
            if (data_curRow[j] == 1)
            {
                vector<int> neighborLabels;
                neighborLabels.reserve(2);
                int leftPixel = data_curRow[j - 1];
                int upPixel = data_preRow[j];
                if (leftPixel > 1)
                {
                    neighborLabels.push_back(leftPixel);
                }
                if (upPixel > 1)
                {
                    neighborLabels.push_back(upPixel);
                }

                if (neighborLabels.empty())
                {
                    labelSet.push_back(++label);
                    data_curRow[j] = label;
                    labelSet[label] = label;
                }
                else
                {
                    sort(neighborLabels.begin(), neighborLabels.end());
                    int smallestLabel = neighborLabels[0];
                    data_curRow[j] = smallestLabel;
                    for (size_t k = 1; k < neighborLabels.size(); k++)
                    {
                        int tempLabel = neighborLabels[k];
                        int& oldSmallestLabel = labelSet[tempLabel];
                        if (oldSmallestLabel > smallestLabel)
                        {
                            labelSet[oldSmallestLabel] = smallestLabel;
                            oldSmallestLabel = smallestLabel;
                        }
                        else if (oldSmallestLabel < smallestLabel)
                        {
                            labelSet[smallestLabel] = oldSmallestLabel;
                        }
                    }
                }
            }
        }
    }
    for (size_t i = 2; i < labelSet.size(); i++)
    {
        int curLabel = labelSet[i];
        int preLabel = labelSet[curLabel];
        while (preLabel != curLabel)
        {
            curLabel = preLabel;
            preLabel = labelSet[preLabel];
        }
        labelSet[i] = curLabel;
    }
    for (int i = 0; i < rows; i++)
    {
        int* data = dimg.ptr<int>(i);
        for (int j = 0; j < cols; j++)
        {
            int& pixelLabel = data[j];
            pixelLabel = labelSet[pixelLabel];
        }
    }
}

Scalar RanColor()
{
    uchar r = 255 * (rand() / (1.0 + RAND_MAX));
    uchar g = 255 * (rand() / (1.0 + RAND_MAX));
    uchar b = 255 * (rand() / (1.0 + RAND_MAX));
    return Scalar(b, g, r);
}


void AddColor(const Mat& simg, Mat& dimg)
{
    if (simg.empty() ||
        simg.type() != CV_32SC1)
    {
        return;
    }

    map<int, Scalar> colors;

    int rows = simg.rows;
    int cols = simg.cols;

    dimg.release();
    dimg.create(rows, cols, CV_8UC3);
    dimg = Scalar::all(0);

    for (int i = 0; i < rows; i++)
    {
        const int* data_src = (int*)simg.ptr<int>(i);
        uchar* data_dst = dimg.ptr<uchar>(i);
        for (int j = 0; j < cols; j++)
        {
            int pixelValue = data_src[j];
            if (pixelValue > 1)
            {
                if (colors.count(pixelValue) <= 0)
                {
                    colors[pixelValue] = RanColor();
                }
                Scalar color = colors[pixelValue];
                *data_dst++ = color[0];
                *data_dst++ = color[1];
                *data_dst++ = color[2];
            }
            else
            {
                data_dst++;
                data_dst++;
                data_dst++;
            }
        }
    }
}


int main()
{
    Mat img = imread("A.png",0);
    Mat temp;
    img.copyTo(temp);
    threshold(img, temp, 50, 1, CV_THRESH_BINARY_INV);
    Mat labelImg;
    pass(temp, labelImg);

    Mat colorLabelImg;
    AddColor(labelImg, colorLabelImg);
    imshow("src", img);
    imshow("dest", colorLabelImg);
    waitKey(0);

    return 0;
}
