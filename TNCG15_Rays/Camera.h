#pragma once
#include "glm/glm.hpp"
#include "Pixel.h"
#include <vector>

class RenderSettings;

class Camera {
public:
	Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4, RenderSettings aRenderSetting);

	const size_t GetResX();
	const size_t GetResY();
	void DisplayPixelPosition(size_t x, size_t y);

	std::vector<Pixel> thePixels;

private:
	size_t x_res;
	size_t y_res;

	glm::dvec3 c1, c2, c3, c4;
};

