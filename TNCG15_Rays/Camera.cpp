#include "Camera.h"
#include "RenderSettings.h"

Camera::Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4, RenderSettings aRenderSetting) : c1{ _c1 }, c2{ _c2 }, c3{ _c3 }, c4{ _c4 } {

	x_res = (int)round((double)aRenderSetting.s_renderingResolution.x * aRenderSetting.s_resolutionScale);
	y_res = (int)round((double)aRenderSetting.s_renderingResolution.y * aRenderSetting.s_resolutionScale);

	double _delta = 2.0 / (double)x_res;

	double _yOffset = 1.0 - (double)y_res / (double)x_res;

	for (size_t i = 0; i < x_res; i++) {
		for (size_t j = 0; j < y_res; j++) {
			// Pixel order is (X+) RIGHT, (Y+) DOWN
			thePixels.push_back(Pixel(glm::dvec3(0.0, -1.0 * i * _delta + 1.0, -1.0 * j * _delta + 1.0 - _yOffset)));
		}
	}
}

void Camera::DisplayPixelPosition(size_t x, size_t y)
{
	double delta = 2.0 / (double)x_res;

	std::cout << "At pos:( " << -(x * delta - 1.0) << " , " << -(y * delta - 1.0) << " )." << std::endl;
}

glm::dvec3 Camera::GetSuperSamplingPixelOffset(size_t currentIteration, size_t totalIterations)
{
	// Super quick & dirty implementation, offset each ray using a circle within the pixel 
	// increase the frequency of the steps when totalIterations is increased.

	if (totalIterations <= 1) return glm::dvec3(0.0, 0.0, 0.0);

	double _delta = 2.0 / (double)x_res; // Pixel size
	double degToCover = 360.0; // Degrees of the circle to cover

	double degOffset = 45.0; // Offset to upper right
	double deg2rad = (3.14159 / 180.0);

	double degsPerSegment = (degToCover / (double)totalIterations);

	double _radius = _delta / (double)2.5; // Chosen after quick testing for aesthetics

	double radToEvaluate = (degOffset + degsPerSegment * currentIteration) * deg2rad;

	double x = _radius * cos(radToEvaluate);
	double y = _radius * sin(radToEvaluate);


	return glm::dvec3(0.0, x, y);
}

const size_t Camera::GetResX()
{
	return x_res;
}

const size_t Camera::GetResY()
{
	return y_res;
}
