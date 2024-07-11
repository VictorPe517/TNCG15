#pragma once
#include "glm/glm.hpp"
#include "Pixel.h"
#include <vector>

class Camera {
public:
	Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4, int resX, int resY, double resolutionScale) : c1{ _c1 }, c2{ _c2 }, c3{ _c3 }, c4{ _c4 } {

		x_res = (int)round((double)resX * resolutionScale);
		y_res = (int)round((double)resY * resolutionScale);

		double _delta = 2.0 / (double)x_res;

		double _yOffset = 1.0 - (double)y_res / (double)x_res;

		// TODO: Make contigous in memory for potential speedup?
		for (size_t i = 0; i < x_res; i++) {
			for (size_t j = 0; j < y_res; j++) {
				// Pixel order is (X+) RIGHT, (Y+) DOWN
				thePixels.push_back(Pixel(glm::dvec3(0.0, -1.0 * i * _delta + 1.0, -1.0 * j * _delta + 1.0 - _yOffset)));
			}
		}
	}

	size_t GetResX();
	size_t GetResY();
	void DisplayPixelPosition(size_t x, size_t y);

	std::vector<Pixel> thePixels;

private:
	size_t x_res;
	size_t y_res;

	glm::dvec3 c1, c2, c3, c4;
};

