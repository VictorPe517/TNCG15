#pragma once

#include "Camera.h"
#include "RenderSettings.h"
#include <iostream>
#include <fstream>

class ImageHandler
{
public:
	void writeCurrentPixelToStream(Camera& theCamera, size_t index, std::ofstream& img, RenderSettings theRenderSettings);
	std::string GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration);
	void DisplayRenderSuccessfulStats(Camera theCamera, double theDuration, double pixelsPerSec, std::string fileName);

	void CreateImageStream(std::ofstream& img, Camera theCamera);
	
	enum TonemappingType {
		gamma,
		sine,
		sigmoid
	};

	double TonemappingFunc(double tone, double exponent, double exponentDivisor, TonemappingType theType);

private:



	bool useTonemapping = true;

	double exposureMultiplier = 1.0;

	double rMax = 0;
	double rMin = INFINITY;

	double gMax = 0;
	double gMin = INFINITY;

	double bMax = 0;
	double bMin = INFINITY;
};

