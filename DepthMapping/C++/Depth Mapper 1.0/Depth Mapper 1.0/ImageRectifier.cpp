#include "ImageRectifier.h"
#include "opencv2\opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat R1, R2, P1, P2, Q;
Mat K1, K2, R;
Vec3d T;
Mat D1, D2;

ImageRectifier::ImageRectifier()
{
	cv::FileStorage fs1("stereoConfig.yml", cv::FileStorage::READ);

	fs1["K1"] >> K1;
	fs1["K2"] >> K2;
	fs1["D1"] >> D1;
	fs1["D2"] >> D2;
	fs1["R"] >> R;
	fs1["T"] >> T;

	fs1["R1"] >> R1;
	fs1["R2"] >> R2;
	fs1["P1"] >> P1;
	fs1["P2"] >> P2;
	fs1["Q"] >> Q;
}

void ImageRectifier::rectifyImages(Mat& leftImg, Mat& rightImg, Mat& rectifiedLeftImage, Mat& rectifiedRightImage)
{
	cv::Mat lmapx, lmapy, rmapx, rmapy;
	cv::Mat imgU1, imgU2;

	cv::initUndistortRectifyMap(K1, D1, R1, P1, leftImg.size(), CV_32F, lmapx, lmapy);
	cv::initUndistortRectifyMap(K2, D2, R2, P2, rightImg.size(), CV_32F, rmapx, rmapy);

	cv::remap(leftImg, rectifiedLeftImage, lmapx, lmapy, cv::INTER_LINEAR);
	cv::remap(rightImg, rectifiedRightImage, rmapx, rmapy, cv::INTER_LINEAR);
}

ImageRectifier::~ImageRectifier()
{
}
