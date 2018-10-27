#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cmath>
using namespace cv;
using namespace std;

void Gaussian(const Mat &inputImg, Mat &outputImg, double sigma);
void MeanFilter(const Mat &input, Mat &output, int window_size);


int main() {

    string imageName;
    Mat inputImg = imread("A.png");
    //Mat inputImg = imread(imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << imageName << endl;
    }
    else {
        double sigma;
        cout << "" << endl;
        cin >> sigma;
            cin.clear();
            cin.ignore();
            cin.sync();
            namedWindow("", WINDOW_AUTOSIZE);
            imshow("", inputImg);
            Mat outputImg(inputImg.rows, inputImg.cols, CV_8UC3);
            Gaussian(inputImg, outputImg, sigma);
            waitKey();

    }
	return 0;
}
void Gaussian(const Mat &inputImg, Mat &outputImg, double sigma) {


	int windows = ((int)(6 * sigma - 1)) / 2 * 2 + 1;

	double *value = new double[windows];
	double sum = 0;
	int left = -windows / 2, right = windows / 2 + 1, border = windows / 2;
	for (int i = left; i < right; i++){
		value[i + border] = pow(2.718, -(i * i) / (2 * sigma * sigma)) / (sqrt(2 * 3.1415926) * sigma);
		sum += value[i + border];
	}
	for (int i = left; i < right; i++){
		value[i + border] = value[i + border] / sum;
		cout << value[i + border] << endl;
	}

	Mat tempImg(inputImg.rows + border * 2, inputImg.cols + border * 2, CV_8UC3);

	Mat fillImg(inputImg.rows + border * 2, inputImg.cols + border * 2, CV_8UC3);
	for (int i = border; i < inputImg.rows + border; i++) {
		for (int j = border; j < inputImg.cols + border; j++) {
			fillImg.at<Vec3b>(i, j)[0] = inputImg.at<Vec3b>((i-border) % (inputImg.rows - 1),(j-border) % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i, j)[1] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), (j - border) % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i, j)[2] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), (j - border) % (inputImg.cols - 1))[2];
		}
	}

	for (int i = 0; i < border; i++) {
		for (int j = 0; j < inputImg.cols + border * 2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>(i%(inputImg.rows-1), j%(inputImg.cols-1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>(i % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>(i % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];
		}
	}

	for (int i = inputImg.rows+border; i < inputImg.rows + border * 2; i++) {
		for (int j = 0; j < inputImg.cols + border * 2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i-border*2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - border * 2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - border * 2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];

		}
	}

	for (int i = border; i < inputImg.rows + border; i++) {
		for (int j = 0; j < border; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i-border) % (inputImg.rows - 1), j % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];

		}
	}

	for (int i = border; i < inputImg.rows + border; i++) {
		for (int j = inputImg.cols+border; j < inputImg.cols + border*2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), (j-border*2) % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), (j - border * 2) % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - border) % (inputImg.rows - 1), (j - border * 2) % (inputImg.cols - 1))[2];

		}
	}
	namedWindow("", WINDOW_AUTOSIZE);
	imshow("", fillImg);

	for (int i = 0; i < tempImg.rows; i++){
		for (int j = border; j < tempImg.cols - border; j++){
			tempImg.at<Vec3b>(i, j)[0] = 0;
			tempImg.at<Vec3b>(i, j)[1] = 0;
			tempImg.at<Vec3b>(i, j)[2] = 0;
			for (int k = left; k < right; k++){
				tempImg.at<Vec3b>(i, j)[0] += (fillImg.at<Vec3b>(i, j + k)[0] * value[k + border]);
				tempImg.at<Vec3b>(i, j)[1] += (fillImg.at<Vec3b>(i, j + k)[1] * value[k + border]);
				tempImg.at<Vec3b>(i, j)[2] += (fillImg.at<Vec3b>(i, j + k)[2] * value[k + border]);
			}
		}
	}


	for (int i = 0; i < outputImg.rows; i++){
		for (int j = 0; j < outputImg.cols; j++){
			outputImg.at<Vec3b>(i, j)[0] = 0;
			outputImg.at<Vec3b>(i, j)[1] = 0;
			outputImg.at<Vec3b>(i, j)[2] = 0;
			for (int k = left; k < right; k++){
				outputImg.at<Vec3b>(i, j)[0] += (tempImg.at<Vec3b>(i + k+border, j+border)[0] * value[k + border]);
				outputImg.at<Vec3b>(i, j)[1] += (tempImg.at<Vec3b>(i + k+border, j+border)[1] * value[k + border]);
				outputImg.at<Vec3b>(i, j)[2] += (tempImg.at<Vec3b>(i + k+border, j+border)[2] * value[k + border]);
			}
		}
	}

	namedWindow("", WINDOW_AUTOSIZE);
	imshow("", outputImg);
}

