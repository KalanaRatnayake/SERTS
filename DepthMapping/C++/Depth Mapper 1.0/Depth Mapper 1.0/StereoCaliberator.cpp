#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include "StereoCaliberator.h"

using namespace std;
using namespace cv;

StereoCaliberator::StereoCaliberator() {
}

void StereoCaliberator::stereoCalibrate(int boardWidth, int boardHeight, int numImgs, float squareSize) {
	vector< vector< Point3f > > object_points;
	vector< vector< Point2f > > imagePoints1, imagePoints2;
	vector< Point2f > corners1, corners2;
	vector< vector< Point2f > > left_img_points, right_img_points;
	Vec3d T;

	Mat img1, img2, gray1, gray2, Kl, Kr, R, F, E, Dl, Dr, R1, R2, P1, P2, Q;

	Size board_size = Size(boardWidth, boardHeight);
	int board_n = boardWidth * boardHeight;

	FileStorage fsl("leftConfig.yml", FileStorage::READ);
	FileStorage fsr("rightConfig.yml", FileStorage::READ);
		
	fsl["K"] >> Kl;
	fsr["K"] >> Kr;
	fsl["D"] >> Dl;
	fsr["D"] >> Dr;

	int flag = CV_CALIB_FIX_INTRINSIC;

	for (int i = 1; i <= numImgs; i++) {
		std::ostringstream leftName, rightName;

		leftName << "testImages\\left\\" << i << ".jpg";
		rightName << "testImages\\right\\" << i << ".jpg";

		img1 = imread(leftName.str(), CV_LOAD_IMAGE_COLOR);
		img2 = imread(rightName.str(), CV_LOAD_IMAGE_COLOR);

		cvtColor(img1, gray1, CV_BGR2GRAY);
		cvtColor(img2, gray2, CV_BGR2GRAY);

		bool found1 = false, found2 = false;

		found1 = cv::findChessboardCorners(img1, board_size, corners1, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		found2 = cv::findChessboardCorners(img2, board_size, corners2, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found1) {
			cv::cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			cv::drawChessboardCorners(gray1, board_size, corners1, found1);
			imshow("Left", gray1);
			waitKey(1);
		}

		if (found2) {
			cv::cornerSubPix(gray2, corners2, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			cv::drawChessboardCorners(gray2, board_size, corners2, found2);
			imshow("Right", gray2);
			waitKey(1);
		}

		vector< Point3f > obj;
		for (int i = 0; i < boardHeight; i++)
			for (int j = 0; j < boardWidth; j++)
				obj.push_back(Point3f((float)j * squareSize, (float)i * squareSize, 0));

		if (found1 && found2) {
			cout << i << ". Found corners!" << endl;
			left_img_points.push_back(corners1);
			right_img_points.push_back(corners2);
			object_points.push_back(obj);
		}
	}

	cv::stereoCalibrate(object_points, left_img_points, right_img_points, Kl, Dl, Kr, Dr, img1.size(), R, T, E, F);
	cv::stereoRectify(Kl, Dl, Kr, Dr, img1.size(), R, T, R1, R2, P1, P2, Q);

	cv::FileStorage fs1("stereoConfig.yml", cv::FileStorage::WRITE);
	fs1 << "K1" << Kl;
	fs1 << "K2" << Kr;
	fs1 << "D1" << Dl;
	fs1 << "D2" << Dr;
	fs1 << "R" << R;
	fs1 << "T" << T;
	fs1 << "E" << E;
	fs1 << "F" << F;
	fs1 << "R1" << R1;
	fs1 << "R2" << R2;
	fs1 << "P1" << P1;
	fs1 << "P2" << P2;
	fs1 << "Q" << Q;
}

StereoCaliberator::~StereoCaliberator() {
}