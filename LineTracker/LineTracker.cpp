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
	cvtColor(frame, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(3, 3), 0);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Mat edges;
	Canny(gray, edges, 50, 150);
	vector<vector<Point>> contours;
	findContours(gray, contours, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
	Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
	RNG rng;
	vector<Vec4i> hierarchy;
	int maxIndex;
	double maxArea = 0;
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i], false); 
		if (area > maxArea) {
			maxIndex = i;
			maxArea = area;
		}
	}
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	drawContours(drawing, contours, maxIndex, color, 2, LINE_8, hierarchy, 0, Point());
	int morph_size = 3;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(drawing, drawing, MORPH_CLOSE, element, Point(-1, -1), 3);
	imshow(winName, drawing);
	waitKey(0);
	return 0;
}


