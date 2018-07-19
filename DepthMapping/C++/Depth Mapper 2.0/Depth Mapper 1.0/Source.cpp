#include "opencv2\opencv.hpp"
#include "features2d\features2d.hpp"
#include "calib3d\calib3d.hpp"
#include "ximgproc.hpp"
#include "ximgproc\disparity_filter.hpp"

int main() {

	cv::VideoCapture left(0);
	cv::VideoCapture right(1);

	left.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	left.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	right.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	right.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	int winSize = 5;
	cv::Ptr<cv::StereoSGBM> leftMatcher;
	leftMatcher = cv::StereoSGBM::create(
		8,
		48,
		5,
		8*3*winSize^2,
		32*8*winSize^2,
		1,
		15,
		0,
		2,
		63,
		cv::StereoSGBM::MODE_SGBM_3WAY
		);

	cv::Ptr<cv::StereoMatcher> rightMatcher = cv::ximgproc::createRightMatcher(leftMatcher);

	cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(leftMatcher);
	wlsFilter->setLambda(80000);
	wlsFilter->setSigmaColor(1.2);

	cv::Mat LeftFrame, RightFrame, dispLeft, dispRight, filteredImg;

	while (true){
		if (left.grab() && right.grab()) {
			left.retrieve(LeftFrame);
			right.retrieve(RightFrame);

			cv::imshow("left frame", LeftFrame);
			cv::imshow("right frame", RightFrame);

			leftMatcher->compute(LeftFrame, RightFrame, dispLeft);
			dispLeft.convertTo(dispLeft, CV_16S);

			rightMatcher->compute(RightFrame, LeftFrame, dispRight);
			dispRight.convertTo(dispLeft, CV_16S);

			wlsFilter->filter(dispLeft, LeftFrame, filteredImg, dispRight);
			cv::normalize(filteredImg, filteredImg, 255.0, 0.0, cv::NORM_MINMAX);

			filteredImg.convertTo(filteredImg, CV_8UC1);
			cv::applyColorMap(filteredImg, filteredImg, cv::COLORMAP_JET);

			cv::imshow("Disparity Map", filteredImg);
			cv::waitKey(1);
		}

	}
}