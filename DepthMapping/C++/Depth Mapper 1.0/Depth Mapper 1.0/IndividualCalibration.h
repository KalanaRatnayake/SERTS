#pragma once
class IndividualCalibration
{
public:
	IndividualCalibration();

	void calibrateLeft(int board_width, int board_height, int num_imgs, float square_size);

	void calibrateRight(int board_width, int board_height, int num_imgs, float square_size);

	~IndividualCalibration();
};

