#include "Camera.h"

void Camera::DisplayPixelPosition(size_t x, size_t y)
{
	double delta = 2.0 / (double)x_res;

	std::cout << "At pos:( " << -(x * delta - 1.0) << " , " << -(y * delta - 1.0) << " )." << std::endl;
}

size_t Camera::GetResX()
{
	return x_res;
}

size_t Camera::GetResY()
{
	return y_res;
}
