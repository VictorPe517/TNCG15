#include "ImageHandler.h"

// Converts the pixel values into integers between 0-255 and saves them to the imagestream "img"
void ImageHandler::writeCurrentPixelToStream(Camera& theCamera, size_t index, std::ofstream& img, RenderSettings theRenderSettings) {
	//-------Write image to file-------//
	int r = (int)round((theCamera.thePixels[index].pixelColor.r) * 255.0 * exposureMultiplier);
	int g = (int)round((theCamera.thePixels[index].pixelColor.g) * 255.0 * exposureMultiplier);
	int b = (int)round((theCamera.thePixels[index].pixelColor.b) * 255.0 * exposureMultiplier);

	//Give the other two channels some of the intensity of the highest colors
	if (r > 255) {
		int temp = (int)round((r - 255.0) / 2.0);
		g += temp;
		b += temp;

		r = 255;
	}

	if (g > 255) {
		int temp2 = (int)round((g - 255.0) / 2.0);
		r += temp2;
		b += temp2;

		g = 255;
	}

	if (b > 255) {
		int temp3 = (int)round((b - 255.0) / 2.0);
		r += temp3;
		g += temp3;

		b = 255;
	}

	r = glm::clamp(r, 0, 255);
	g = glm::clamp(g, 0, 255);
	b = glm::clamp(b, 0, 255);

	img << r << " " << g << " " << b << "\n";
}


// Generates a unique filename for the file preventing overwrite
std::string ImageHandler::GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration) {
	return std::to_string(theCamera.GetResX()) + "x" + std::to_string(theCamera.GetResY()) + "px__iterations-" + std::to_string((int)floor(renderSettings.s_GlobalIterations)) + "__time-" + std::to_string(duration) + ".ppm";
}

// Displays stats after a successful render
void ImageHandler::DisplayRenderSuccessfulStats(Camera theCamera, double theDuration, double pixelsPerSec, std::string fileName)
{
	std::cout << std::flush
		<< "/===============[ Render Successful! ]================\n"
		<< "  Total pixels processed: \n    " << theCamera.thePixels.size() << " px\n"
		<< "  Time Elapsed: \n    " << round((theDuration * 60.0)) << " second(s)\n\n"
		<< "  Ray Tracing Speed: \n    " << round((60.0 * pixelsPerSec / 10000.0)) / 100.0 << " Mil pixels/minute\n\n"
		<< "  Image saved as: \n    " << fileName << "\n"
		<< "=====================================================/\n\n";
}

void ImageHandler::CreateImageStream(std::ofstream& img, Camera theCamera)
{
	// Create image out stream
	img << "P3" << std::endl;
	img << theCamera.GetResY() << " " << theCamera.GetResX() << std::endl;
	img << "255" << std::endl;
}
