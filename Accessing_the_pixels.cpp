// accessing the different pixels in cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <vector>
///////////////////////////////////////////////////////////////////////////////////////////////////

using namespace cv;
using namespace std;

int main() {

	Mat imgOriginal;		// input image
	Mat imgRed;		// red content of the image
	Mat imgBlue;		// blue content of the image
	Mat imgGreen;		// green content of the image
	Mat imgYellow;
	int i, j, k;
	Vec3b pixelVal;	//the 3 pixel values
	uchar singleVal;	//the grayscale value

	imgOriginal = imread("G:\\winter'15_project\\IP_samples\\shapes.jpg");			// open image

	if (imgOriginal.empty()) {									// if unable to open image
		std::cout << "error: image not read from file\n\n";		// show error message on command line
		return(0);												// and exit program
	}

	imgBlue = Mat(imgOriginal.rows, imgOriginal.cols, CV_8UC1);
	imgGreen = Mat(imgOriginal.rows, imgOriginal.cols, CV_8UC1);
	imgRed = Mat(imgOriginal.rows, imgOriginal.cols, CV_8UC1);
	imgYellow = Mat(imgOriginal.rows, imgOriginal.cols, CV_8UC1);

	for (i = 0; i < imgOriginal.rows; i++){
		for (j = 0; j < imgOriginal.cols; j++){
			pixelVal = imgOriginal.at<Vec3b>(i, j);			//read the pixel pointer	
			imgBlue.at<uchar>(i, j) = pixelVal.val[0];		//blue value	
			imgGreen.at<uchar>(i, j) = pixelVal.val[1];		//green value
			singleVal = pixelVal.val[2];
			imgRed.at<uchar>(i, j) = singleVal;				//red value
			imgYellow.at<uchar>(i, j) = (uchar)(((int)imgGreen.at<uchar>(i, j) + (int)imgRed.at<uchar>(i, j)) / 2);
			//yellow = red + green
		}
	}
	printf("Initialised.\n");
	
	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Red", CV_WINDOW_AUTOSIZE);
	namedWindow("Green", CV_WINDOW_AUTOSIZE);
	namedWindow("Blue", CV_WINDOW_AUTOSIZE);
	namedWindow("Yellow", CV_WINDOW_AUTOSIZE);

	imshow("Original", imgOriginal);
	imshow("Red", imgRed);
	imshow("Blue", imgBlue);
	imshow("Green", imgGreen);
	imshow("Yellow", imgYellow);

	waitKey(0);					// hold windows open until user presses a key

	return(0);
}