#include "opencv2\opencv.hpp"
#include <string.h>
#include "CameraCapture.h"
#include "IndividualCalibration.h"
#include "StereoCaliberator.h"
#include "ImageRectifier.h"
#include "DepthMap.h"

int main()
{
	std::cout << "Enter : 1 : to capture calibaraton images" << std::endl;
	std::cout << "Enter : 2 : to calibrate" << std::endl;
	std::cout << "Enter : 3 : to create depth map" << std::endl;
	std::cout << std::endl;

	int value;
	std::cout << "Enter the value: ";
	std::cin >> value;
	std::cout << std::endl;

	if (value == 1) {
		CameraCapture camera;
		int left, right;
		int width, height;

		std::cout << "Enter the camera order" << std::endl;
		std::cout << "Left : ";
		std::cin >> left;
		std::cout << "Right : ";
		std::cin >> right;
		std::cout << std::endl;

		std::cout << "Enter the Image dimensions" << std::endl;
		std::cout << "width : ";
		std::cin >> width;
		std::cout << "height : ";
		std::cin >> height;

		std::cout << "Started capturing" << std::endl;
		camera.capture(left, right, width, height);
		std::cout << "finished capturing" << std::endl;

		camera.~CameraCapture();
	}
	
	if (value == 2) {
		IndividualCalibration singleCalibrator;
		StereoCaliberator stereoCalibrattor;

		std::cout << "Started calibrating left camera" << std::endl;
		singleCalibrator.calibrateLeft(7, 7, 30, 4.846F);
		std::cout << "finished calibrating left camera" << std::endl;

		std::cout << "Started calibrating right camera" << std::endl;
		singleCalibrator.calibrateRight(7, 7, 30, 4.846F);
		std::cout << "finished calibrating right camera" << std::endl;

		std::cout << "Started calibrating stereo camera" << std::endl;
		stereoCalibrattor.stereoCalibrate(7, 7, 30, 4.846F);
		std::cout << "finisheded calibrating stereo camera" << std::endl;
	}

	if (value == 3) {
		cv::Mat imageLeft, imageRight, rectifiedLeft, rectifiedRight, disparityMap;
		int left, right;
		int width, height;

		std::cout << "Enter the camera order" << std::endl;
		std::cout << "Left : ";
		std::cin >> left;
		std::cout << "Right : ";
		std::cin >> right;
		std::cout << std::endl;

		std::cout << "Enter the Image dimensions" << std::endl;
		std::cout << "width : ";
		std::cin >> width;
		std::cout << "height : ";
		std::cin >> height;
		std::cout << std::endl;

		//disparity map matrix need predefined sizes becaause depthmaap injects disparity values directly
		disparityMap = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));

		cv::VideoCapture leftCam = cv::VideoCapture(left);
		cv::VideoCapture rightCam = cv::VideoCapture(right);

		leftCam.set(cv::CAP_PROP_FRAME_WIDTH, width);
		leftCam.set(cv::CAP_PROP_FRAME_HEIGHT, height);

		rightCam.set(cv::CAP_PROP_FRAME_WIDTH, width);
		rightCam.set(cv::CAP_PROP_FRAME_HEIGHT, height);

		ImageRectifier rectifier;
		DepthMap depthMapCreator;

		while (true) {
			if (leftCam.grab() && rightCam.grab()) {
				leftCam.retrieve(imageLeft);
				rightCam.retrieve(imageRight);

				std::cout << "Started rectifying" << std::endl;
				rectifier.rectifyImages(imageLeft, imageRight, rectifiedLeft, rectifiedRight);
				std::cout << "finished rectifying" << std::endl;

				std::cout << "Started depthmap" << std::endl;
				depthMapCreator.generateDisparityMap(rectifiedLeft, rectifiedRight, disparityMap);
				std::cout << "finished depthmap" << std::endl;

				cv::imshow("Left", imageLeft);
				cv::imshow("Right", imageRight);
				cv::imshow("Rectified Left", rectifiedLeft);
				cv::imshow("Rectified Right", rectifiedRight);
				cv::imshow("Disparity", disparityMap);

				cv::waitKey(1);
			}
		}
	}
    
}

