#pragma once
class CameraCapture
{
public:
	CameraCapture();

	void capture(int left, int right, int width, int height);

	~CameraCapture();
};

