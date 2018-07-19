#include "IndividualCalibration.h"
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

IndividualCalibration::IndividualCalibration() {
}

void IndividualCalibration::calibrateLeft(int board_width, int board_height, int num_imgs, float square_size) {

	vector< vector< Point3f > > object_points;
	vector< vector< Point2f > > image_points;
	vector< Point2f > corners;
	vector< Mat > rvecs, tvecs;
	Mat img, gray, K, D;
	Size im_size, board_size = Size(board_width, board_height);
	int flag = CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5;
	Size ;
	int board_n = board_width * board_height;

	for (int k = 1; k <= num_imgs; k++) {
		std::ostringstream name;
		name << "testImages\\left\\" << k << ".jpg";;

		img = imread(name.str(), CV_LOAD_IMAGE_COLOR);
		cv::cvtColor(img, gray, CV_BGR2GRAY);

		bool found = false;
		found = cv::findChessboardCorners(img, board_size, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found){
			cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray, board_size, corners, found);
			imshow("Left", gray);
			waitKey(1);
		}

		vector< Point3f > obj;
		for (int i = 0; i < board_height; i++)
			for (int j = 0; j < board_width; j++)
				obj.push_back(Point3f((float)j * square_size, (float)i * square_size, 0));

		if (found) {
			cout << k << ". Found corners!" << endl;
			image_points.push_back(corners);
			object_points.push_back(obj);
		}
	}

	calibrateCamera(object_points, image_points, img.size(), K, D, rvecs, tvecs, flag);

	FileStorage fs("leftConfig.yml", FileStorage::WRITE);
	fs << "K" << K;
	fs << "D" << D;
	fs << "board_width" << board_width;
	fs << "board_height" << board_height;
	fs << "square_size" << square_size;
}

void IndividualCalibration::calibrateRight(int board_width, int board_height, int num_imgs, float square_size) {

	vector< vector< Point3f > > object_points;
	vector< vector< Point2f > > image_points;
	vector< Point2f > corners;
	vector< Mat > rvecs, tvecs;
	Mat img, gray, K, D;
	Size im_size, board_size = Size(board_width, board_height);
	int flag = CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5;
	Size;
	int board_n = board_width * board_height;

	for (int k = 1; k <= num_imgs; k++) {
		std::ostringstream name;
		name << "testImages\\right\\" << k << ".jpg";;

		img = imread(name.str(), CV_LOAD_IMAGE_COLOR);
		cv::cvtColor(img, gray, CV_BGR2GRAY);

		bool found = false;
		found = cv::findChessboardCorners(img, board_size, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found) {
			cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray, board_size, corners, found);
			imshow("Right", gray);
			waitKey(1);
		}

		vector< Point3f > obj;
		for (int i = 0; i < board_height; i++)
			for (int j = 0; j < board_width; j++)
				obj.push_back(Point3f((float)j * square_size, (float)i * square_size, 0));

		if (found) {
			cout << k << ". Found corners!" << endl;
			image_points.push_back(corners);
			object_points.push_back(obj);
		}

		imshow("Right", gray);
		waitKey(1);
	}

	calibrateCamera(object_points, image_points, img.size(), K, D, rvecs, tvecs, flag);

	FileStorage fs("rightConfig.yml", FileStorage::WRITE);
	fs << "K" << K;
	fs << "D" << D;
	fs << "board_width" << board_width;
	fs << "board_height" << board_height;
	fs << "square_size" << square_size;
}

IndividualCalibration::~IndividualCalibration() {

}