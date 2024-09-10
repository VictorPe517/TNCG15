#pragma once

#include "Camera.h"
#include "RenderSettings.h"
#include <iostream>
#include <fstream>

class ImageHandler
{
public:
	void writeCurrentPixelToStream(Camera& theCamera, size_t index, std::ofstream& img, RenderSettings theRenderSettings);
	std::string GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration, std::string sceneName);
	void DisplayRenderSuccessfulStats(Camera theCamera, double theDuration, double pixelsPerSec, std::string fileName);

	void CreateImageStream(std::ofstream& img, Camera theCamera);
	
	enum TonemappingType {
		linear,
		gamma,
		sigmoid,
		reinhard
	};

	double TonemappingFunc(double tone, TonemappingType theType, double exponent = 2.0, double exponentDivisor = 3.0);

private:
	double exposureMultiplier = 2.2;

	double rMax = 0;
	double rMin = INFINITY;

	double gMax = 0;
	double gMin = INFINITY;

	double bMax = 0;
	double bMin = INFINITY;
};

