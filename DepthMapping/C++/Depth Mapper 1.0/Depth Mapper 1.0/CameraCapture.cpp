#include "CameraCapture.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

CameraCapture::CameraCapture()
{
}

void CameraCapture::capture(int left, int right, int width, int height) {
	int x = 0;
	VideoCapture capl(left);
	VideoCapture capr(right);

	capl.set(CAP_PROP_FRAME_WIDTH, width);
	capl.set(CAP_PROP_FRAME_HEIGHT, height);

	capr.set(CAP_PROP_FRAME_WIDTH, width);
	capr.set(CAP_PROP_FRAME_HEIGHT, height);

	Mat imgLeft, imgRight;

	string folderLeft = "testImages\\left";
	string folderRight = "testImages\\right";

	string folderCreateCommandLeft = "mkdir " + folderLeft;
	system(folderCreateCommandLeft.c_str());

	string folderCreateCommandRight = "mkdir " + folderRight;
	system(folderCreateCommandRight.c_str());

	while (true) {
		std::ostringstream leftName, rightName;

		capl.read(imgLeft);
		capr.read(imgRight);

		imshow("Left", imgLeft);
		imshow("Right", imgRight);

		if (waitKey(30) > 0) {
			x++;

			leftName << "testImages\\left\\"<< x << ".jpg";
			rightName << "testImages\\right\\" << x << ".jpg";
			 
			cout << "Saving img pair " << x << endl;
			imwrite(leftName.str(), imgLeft);
			imwrite(rightName.str(), imgRight);
		}
	}
}


CameraCapture::~CameraCapture()
{
}
