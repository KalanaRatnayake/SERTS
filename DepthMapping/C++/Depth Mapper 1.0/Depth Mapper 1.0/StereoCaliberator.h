#pragma once
class StereoCaliberator
{
public:
	StereoCaliberator();

	void stereoCalibrate(int boardWidth, int boardHeight, int numImgs, float squareSize);

	~StereoCaliberator();
};

