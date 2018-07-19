#include "opencv2\opencv.hpp"

class ImageRectifier
{
public:
	ImageRectifier();

	void rectifyImages(cv::Mat& leftImg, cv::Mat& rightImg, cv::Mat& rectifiedLeftImage, cv::Mat& rectifiedRightImage);

	~ImageRectifier();
};
