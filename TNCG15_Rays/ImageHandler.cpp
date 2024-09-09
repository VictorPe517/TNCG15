#include "ImageHandler.h"

double ImageHandler::TonemappingFunc(double tone, double exponent,double exponentDivisor, TonemappingType theType) {
	//Tone should be between 0 and 2
	if (theType == gamma) {
		return 0.5 * pow(tone, exponent / exponentDivisor);
	}
	else if (theType == sine) {
		if (tone > 0.99562) {
			return 0.97 + 0.08*glm::log(tone);
		}
		else {
			return 8.0 * pow((-0.5 * cos(0.4 * tone * 3.1415 + 0.1)), 3) + 0.98;
		}
	}
	else if (theType == sigmoid) {
		return 1.5 / (1.0 + exp(-1.5 * (tone - 0.5))) - 0.48;
	}
	
}

// Converts the pixel values into integers between 0-255 and saves them to the imagestream "img"
void ImageHandler::writeCurrentPixelToStream(Camera& theCamera, size_t index, std::ofstream& img, RenderSettings theRenderSettings) {
	//-------Write image to file-------//
	double r_d = (theCamera.thePixels[index].pixelColor.r);
	double g_d = (theCamera.thePixels[index].pixelColor.g);
	double b_d = (theCamera.thePixels[index].pixelColor.b);

	//rMax = glm::max(r_d, rMax);
	//gMax = glm::max(g_d, gMax);
	//bMax = glm::max(b_d, bMax);

	//rMin = glm::min(r_d, rMin);
	//gMin = glm::min(g_d, gMin);
	//bMin = glm::min(b_d, bMin);

	double r;
	double g;
	double b;

	if (useTonemapping) {
		r = TonemappingFunc(r_d, 2, 3.0, sine) * exposureMultiplier;
		g = TonemappingFunc(g_d, 2, 3, sine) * exposureMultiplier;
		b = TonemappingFunc(b_d, 2, 3, sine) * exposureMultiplier;
	}
	else {
		r = r_d * exposureMultiplier * 6.0;
		g = g_d * exposureMultiplier * 6.0;
		b = b_d * exposureMultiplier * 6.0;
	}


	//Give the other two channels some of the intensity of the highest colors
	if (r > 1.0) {
		double temp = ((r - 1.0) / 3.0);
		g += temp;
		b += temp;

		r = 1.0;
	}

	if (g > 1.0) {
		double temp2 = ((g - 1.0) / 3.0);
		r += temp2;
		b += temp2;

		g = 1.0;
	}

	if (b > 1.0) {
		double temp3 = ((b - 1.0) / 3.0);
		r += temp3;
		g += temp3;

		b = 1.0;
	}

	r = glm::clamp(r, 0.0, 255.0);
	g = glm::clamp(g, 0.0, 255.0);
	b = glm::clamp(b, 0.0, 255.0);

	int r_f = (int)round(r*255.0);
	int g_f = (int)round(g*255.0);
	int b_f = (int)round(b*255.0);

	r_f = glm::clamp(r_f, 0, 255);
	g_f = glm::clamp(g_f, 0, 255);
	b_f = glm::clamp(b_f, 0, 255);

	rMax = glm::max((double)r_f, rMax);
	gMax = glm::max((double)g_f, gMax);
	bMax = glm::max((double)b_f, bMax);

	rMin = glm::min((double)r_f, rMin);
	gMin = glm::min((double)g_f, gMin);
	bMin = glm::min((double)b_f, bMin);

	img << r_f << " " << g_f << " " << b_f << "\n";
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

	std::cout << "Max values: r = " << rMax << ", g = " << gMax << ", b = " << bMax << "\n";
	std::cout << "Min values: r = " << rMin << ", g = " << gMin << ", b = " << bMin << "\n";
}

void ImageHandler::CreateImageStream(std::ofstream& img, Camera theCamera)
{
	// Create image out stream
	img << "P3" << std::endl;
	img << theCamera.GetResY() << " " << theCamera.GetResX() << std::endl;
	img << "255" << std::endl;
}
