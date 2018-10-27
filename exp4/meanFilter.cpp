#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cmath>
using namespace cv;
using namespace std;


void MeanFilter(const Mat &inputImg, Mat &outputImg, int window_size);
int main() {

    string imageName;
    Mat inputImg = imread("A.png");
    //Mat inputImg = imread(imageName);
    if (inputImg.empty()) {
        cout << "cannot open " << imageName << endl;
    }
    else {
        int size;
        cout << "" << endl;
        cin >> size;
        while (size % 2 == 0 || size <= 0) {
            cout << "" << endl;
            cin >> size;
        }
        cin.clear();
        cin.ignore();
        cin.sync();
        namedWindow("", WINDOW_AUTOSIZE);
        imshow("", inputImg);
        Mat outputImg(inputImg.rows, inputImg.cols, CV_8UC3);
        MeanFilter(inputImg, outputImg, size);
        waitKey();

    }

	return 0;
}

void MeanFilter(const Mat &inputImg, Mat &outputImg, int window_size) {



	int w = (window_size - 1) / 2;
	int z = window_size * window_size;


	Mat fillImg(inputImg.rows + w * 2, inputImg.cols + w * 2 , CV_8UC3);
	for (int i = w; i < inputImg.rows + w; i++) {
		for (int j = w; j < inputImg.cols + w; j++) {
			fillImg.at<Vec3b>(i, j)[0] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w) % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i, j)[1] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w) % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i, j)[2] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w) % (inputImg.cols - 1))[2];
		}
	}

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < inputImg.cols + w * 2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>(i % (inputImg.rows - 1), j % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>(i % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>(i % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];
		}
	}

	for (int i = inputImg.rows + w; i < inputImg.rows + w * 2; i++) {
		for (int j = 0; j < inputImg.cols + w * 2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i - w * 2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - w * 2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - w * 2) % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];

		}
	}

	for (int i = w; i < inputImg.rows + w; i++) {
		for (int j = 0; j < w; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), j % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), j % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), j % (inputImg.cols - 1))[2];

		}
	}

	for (int i = w; i < inputImg.rows + w; i++) {
		for (int j = inputImg.cols + w; j < inputImg.cols + w * 2; j++) {
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[0] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w * 2) % (inputImg.cols - 1))[0];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[1] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w * 2) % (inputImg.cols - 1))[1];
			fillImg.at<Vec3b>(i%fillImg.rows, j%fillImg.cols)[2] = inputImg.at<Vec3b>((i - w) % (inputImg.rows - 1), (j - w * 2) % (inputImg.cols - 1))[2];

		}
	}
	namedWindow("", WINDOW_AUTOSIZE);
	imshow("", fillImg);



	int ***integral;

	integral = new int**[fillImg.rows];
	for (int i = 0; i<fillImg.rows; i++) {
		integral[i] = new int*[fillImg.cols];
	}
	for (int i = 0; i < fillImg.rows; i++) {
		for (int j = 0; j < fillImg.cols; j++) {
			integral[i][j] = new int[3];
		}
	}


	for (int i = 0; i < fillImg.rows; i++) {
		for (int j = 0; j < fillImg.cols; j++) {

			//integral(i,j) = integral(i,j-1) + ColumnSum(j);
			int *columnSum=new int[3];
			columnSum[0] = 0;
			columnSum[1] = 0;
			columnSum[2] = 0;
			for (int k = 0; k < i+1; k++) {
				columnSum[0] += (int)fillImg.at<Vec3b>(k, j)[0];
				columnSum[1] += (int)fillImg.at<Vec3b>(k, j)[1];
				columnSum[2] += (int)fillImg.at<Vec3b>(k, j)[2];
				//cout << (int)columnSum[0] << " ";
			}
			if (j == 0) {
				integral[i][j][0] = columnSum[0];
				integral[i][j][1] = columnSum[1];
				integral[i][j][2] = columnSum[2];

			}
			else {
				integral[i][j][0] = integral[i][j - 1][0] + columnSum[0];
				integral[i][j][1] = integral[i][j - 1][1] + columnSum[1];
				integral[i][j][2] = integral[i][j - 1][2] + columnSum[2];
				//cout << (int)integral[i][j][0] << " ";
			}



		}
	}


	for (int i = w + 1; i < fillImg.rows - w - 1; i++) {
		for (int j = w + 1; j < fillImg.cols - w - 1; j++) {
			outputImg.at<Vec3b>(i-w-1, j-w-1)[0] = (integral[i + w][j + w][0] + integral[i - w - 1][j - w - 1][0] - integral[i + w][j - w - 1][0] - integral[i - w - 1][j + w][0]) / z;
			outputImg.at<Vec3b>(i-w-1, j-w-1)[1] = (integral[i + w][j + w][1] + integral[i - w - 1][j - w - 1][1] - integral[i + w][j - w - 1][1] - integral[i - w - 1][j + w][1]) / z;
			outputImg.at<Vec3b>(i-w-1, j-w-1)[2] = (integral[i + w][j + w][2] + integral[i - w - 1][j - w - 1][2] - integral[i + w][j - w - 1][2] - integral[i - w - 1][j + w][2]) / z;

		}
	}



	namedWindow("", WINDOW_AUTOSIZE);
	imshow("", outputImg);

	/*for (int i = 0; i<2; i++){
		for (int j = 0; j<20; j++){
			cout <<i<<" "<<j<<" "<< (int)integral[i][j][0] << endl;
		}
	}
*/
	for (int i = 0; i<fillImg.rows; i++)
	{
		for (int j = 0; j<fillImg.cols; j++)
			delete[] integral[i][j];
		delete[] integral[i];
	}
	delete[] integral;

}
