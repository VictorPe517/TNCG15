#pragma once

#include "Camera.h"
#include "RenderSettings.h"
#include <iostream>
#include <fstream>

class ImageHandler
{
public:
	void writeCurrentPixelToStream(Camera& theCamera, size_t currentX, size_t currentY, std::ofstream& img, RenderSettings theRenderSettings);
	std::string GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration);
	void DisplayRenderSuccessfulStats(Camera theCamera, double theDuration, double pixelsPerSec, std::string fileName);

	void CreateImageStream(std::ofstream& img, Camera theCamera);

private:
	int exposureMultiplier = 7;
};

