// LineTracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main()
{

	String winName = "Video Feed";
	namedWindow(winName, CV_WINDOW_NORMAL);
	Mat frame = imread("White_Strip.jpg");
	Mat gray;
	Mat edges;
	cvtColor(frame, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
	// Apply Morphological Filtering
	// Erode "erosionNum" times
	Canny(gray, edges, 0, 30, 3);
	threshold(edges, edges, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	bitwise_not(edges, edges);
	int erodeNum = 10;
	erode(gray, gray, NULL, Point(-1, -1), erodeNum, 0);
	int dilateNum = 5;
	//dilate(gray, gray, NULL, Point(-1, -1), dilateNum, 0);
	imshow(winName, edges);
	//if (waitKey(30) >= 30) break;
	waitKey(0);
	return 0;
}

