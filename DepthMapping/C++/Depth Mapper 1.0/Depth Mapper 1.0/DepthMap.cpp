#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "DepthMap.h"

using namespace cv;
using namespace std;

int w = 0;

bool inImg(Mat& img, int x, int y) {
	if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
		return true;
}

bool isLeftKeyPoint(int i, int j, vector< KeyPoint > kpl) {
	int n = kpl.size();
	return (i >= kpl[0].pt.x && i <= kpl[n - 1].pt.x && j >= kpl[0].pt.y && j <= kpl[n - 1].pt.y);
}

bool isRightKeyPoint(int i, int j, vector< KeyPoint > kpr) {
	int n = kpr.size();
	return (i >= kpr[0].pt.x && i <= kpr[n - 1].pt.x && j >= kpr[0].pt.y && j <= kpr[n - 1].pt.y);
}

long descCost(Point leftpt, Point rightpt, int w, Mat& img_left_desc, Mat& img_right_desc, vector< KeyPoint > kpl, vector< KeyPoint > kpr) {
	int x0r = kpr[0].pt.x;
	int y0r = kpr[0].pt.y;
	int ynr = kpr[kpr.size() - 1].pt.y;
	int x0l = kpl[0].pt.x;
	int y0l = kpl[0].pt.y;
	int ynl = kpl[kpl.size() - 1].pt.y;
	long cost = 0;
	for (int j = -w; j <= w; j++) {
		for (int k = -w; k <= w; k++) {
			if (!isLeftKeyPoint(leftpt.x + j, leftpt.y + k, kpl) || !isRightKeyPoint(rightpt.x + j, rightpt.y + k, kpr))
				continue;
			int idxl = (leftpt.x + j - x0l)*(ynl - y0l + 1) + (leftpt.y + k - y0l);
			int idxr = (rightpt.x + j - x0r)*(ynr - y0r + 1) + (rightpt.y + k - y0r);
			cost += norm(img_left_desc.row(idxl), img_right_desc.row(idxr), CV_L1);
		}
	}
	return cost / ((2 * w + 1)*(2 * w + 1));
}

int getCorresPointRight(Point p, int ndisp, Mat& img_left_desc, Mat& img_right_desc, vector< KeyPoint > kpl, vector< KeyPoint > kpr) {
	long minCost = 1e9;
	int chosen_i = 0;
	for (int i = p.x - ndisp; i <= p.x; i++) {
		long cost = descCost(p, Point(i, p.y), w, img_left_desc, img_right_desc, kpl, kpr);
		if (cost < minCost) {
			minCost = cost;
			chosen_i = i;
		}
	}
	if (minCost == 0)
		return p.x;
	return chosen_i;
}

int getCorresPointLeft(Point p, int ndisp, Mat& img_left_desc, Mat& img_right_desc, vector< KeyPoint > kpl, vector< KeyPoint > kpr) {
	long minCost = 1e9;
	int chosen_i = 0;
	for (int i = p.x; i <= p.x + ndisp; i++) {
		long cost = descCost(Point(i, p.y), p, w, img_left_desc, img_right_desc, kpl, kpr);
		if (cost < minCost) {
			minCost = cost;
			chosen_i = i;
		}
	}
	if (minCost == 0)
		return p.x;
	return chosen_i;
}

void computeDisparityMapORB(Mat& imgLeft, Mat& imgRight, Mat& img_left_desc, Mat& img_right_desc, vector< KeyPoint > kpl, vector< KeyPoint > kpr, int ndisp, Mat& img_disparity) {
	for (int i = ndisp + 1; i < imgLeft.cols; i++) {
		for (int j = 0; j < imgLeft.rows; j++) {
			if (!isLeftKeyPoint(i, j, kpl))
				continue;
			
			int right_i = getCorresPointRight(Point(i, j), ndisp, img_left_desc, img_right_desc,  kpl, kpr);
			int left_i = getCorresPointLeft(Point(right_i,j), ndisp, img_left_desc, img_right_desc, kpl, kpr);

			if (abs(left_i-i) > 4)
				continue;
			
			int disparity = abs(i - right_i);
			img_disparity.at<uchar>(j, i) = disparity;
		}
	}
}

void cacheDescriptorVals(Mat& imgLeft, Mat& imgRight, Mat& Out_img_left_desc, Mat& Out_img_right_desc, vector< KeyPoint > Output_kpl, vector< KeyPoint > Output_kpr) {
	Ptr<Feature2D> extractor = ORB::create();

	for (int i = 0; i < imgRight.cols; i++) {
		for (int j = 0; j < imgLeft.rows; j++) {
			Output_kpl.push_back(KeyPoint(i, j, 1));
			Output_kpr.push_back(KeyPoint(i, j, 1));
		}
	}
	extractor->compute(imgLeft, Output_kpl, Out_img_left_desc);
	extractor->compute(imgRight, Output_kpr, Out_img_right_desc);
}

DepthMap::DepthMap() {
}

void DepthMap::generateDisparityMap(Mat& imgLeft, Mat& imgRight, Mat& outDisparityMap)
{
	Mat img_left_desc, img_right_desc;
	vector< KeyPoint > kpl, kpr;

	while (true) {
		cacheDescriptorVals(imgLeft, imgRight, img_left_desc, img_right_desc, kpl, kpr);
		computeDisparityMapORB(imgLeft, imgRight, img_left_desc, img_right_desc, kpl, kpr, 40, outDisparityMap);
	}
}

DepthMap::~DepthMap() {
}