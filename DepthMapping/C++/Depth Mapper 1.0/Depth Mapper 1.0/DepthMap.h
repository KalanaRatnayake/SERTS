#pragma once
class DepthMap
{
public:
	DepthMap();

	void generateDisparityMap(cv::Mat& imgLeft, cv::Mat& imgRight, cv::Mat& outDisparityMap);

	~DepthMap();
};

