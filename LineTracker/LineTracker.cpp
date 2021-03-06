// LineTracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;

vector<Point> getLargestContour(Mat input) {
	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(3, 3), 0);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	vector<vector<Point>> contours;
	findContours(gray, contours, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
	Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
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
	return contours[maxIndex];
	//Scalar color = Scalar(255, 255, 255);
	//drawContours(drawing, contours, maxIndex, color, 2, LINE_8, hierarchy, 0, Point());
	///*int morphSize = 3;
	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4 * morphSize + 1, 2 * morphSize + 1),
	//Point(morphSize, morphSize));
	//morphologyEx(drawing, drawing, MORPH_CLOSE, element, Point(-1, -1), morphSize);*/
	//return drawing;
}

void drawContour(Mat input, vector<Point> contour, Scalar color) {
	vector<vector<Point>> arr(1);
	arr[0] = contour;
	drawContours(input, arr, 0, color, LINE_8);
}

int main()
{
	String winName = "Pi Camera";
	namedWindow(winName, CV_WINDOW_NORMAL);
	Mat frame = imread("White_Strip.jpg");
	int startY = frame.rows *(0);
	Rect roi(0, startY, frame.cols, frame.rows - startY);
	frame = frame(roi);
	vector<Point> contour = getLargestContour(frame);
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
	drawContour(drawing, contour, Scalar(0, 0, 255));
	// Get centroid.
	Moments moment = moments(contour, true);
	Point2f centroid = Point2f(moment.m10 / moment.m00, moment.m01 / moment.m00);
	circle(drawing, centroid, 4, Scalar(0, 0, 255), LINE_8);
	// Close drawing.
	const int MORPH_SIZE = 3;
	const int CLOSE_ITERATIONS = 3;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4 * MORPH_SIZE + 1, 2 * MORPH_SIZE + 1),
		Point(MORPH_SIZE, MORPH_SIZE));

	float xDist = abs(centroid.x - frame.cols / 2);
	float yDist = abs(frame.rows - centroid.y);
	float angle = atan2(xDist, yDist) * (180 / M_PI);
	cout << "Angle: " << angle << "\n";
	morphologyEx(drawing, drawing, MORPH_CLOSE, element, Point(-1, -1), CLOSE_ITERATIONS);
	// Overlay contour and centroid over original image.
	for (int i = 0; i < frame.rows; i++) {
		for (int j = 0; j < frame.cols; j++) {
			if (drawing.at<Vec3b>(i, j)[0] != 0 || drawing.at<Vec3b>(i, j)[1] != 0 || drawing.at<Vec3b>(i, j)[2] != 0) {
				frame.at<Vec3b>(i, j) = drawing.at<Vec3b>(i, j);
			}
		}
	}
	line(frame, centroid, Point2f(frame.cols / 2, frame.rows), Scalar(255, 0, 0));
	imshow(winName, frame);
	waitKey(0);
	return 0;
}




