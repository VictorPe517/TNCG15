// TNCG15_Rays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// INHERITANCE: https://msbrijuniversity.ac.in/assets/uploads/newsupdate/InheritanceinC++.pdf
// MULTITHREADING: https://www.educative.io/blog/modern-multithreading-and-concurrency-in-cpp

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <cassert>
#include <thread>
#include <ppl.h>
#include <chrono>
#include <ratio>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Camera.h"
#include "RenderSettings.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "LightSource.h"
#include "Sphere.h"
#include "Object.h"
#include "cube.h"


class LightSource;

void writeCurrentPixelToFile(Camera& theCamera, size_t i, size_t j, std::ofstream& img, int& maxval, int& minval);
void DisplayLoadingBar(int& rowsDone, const int& x_res, bool currentlyWriting);
std::string GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration);

std::vector<Object*> Object::theObjects;
std::vector<Polygon*> Polygon::thePolygons;
std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;
std::vector<Sphere*> Sphere::theSpheres;
std::vector<Cube> Cube::theCubes;
std::vector<LightSource*> LightSource::theLightSources;

//---------------------[ STANDARD SETTINGS ]---------------------//
RenderSettings theRenderSettings;

double exposureMultiplier = 10;
//---------------------------------------------------------------//

int main()
{
	bool currentlyWriting = false;
	size_t pixelIndex = 0;

	int maxval = -INFINITY;
	int minval = INFINITY;

	glm::dvec3 theEye(-1.0, 0.0, 0.0);

	std::cout << "Initializing Scene Geometry...\n\n";
	//------GEOMETRY------//
	Rectangle cielingRect(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), ColorDBL::Magenta);
	Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), ColorDBL::Yellow);
	Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), ColorDBL::Red);

	//-------FLOOR-------//
	Rectangle floorRect(glm::dvec3(10, 6, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), ColorDBL::Green);

	Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), ColorDBL::Blue);  //Behind camera

	//-------WALLS-------//
	Rectangle wallN(glm::dvec3(10, 6, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), ColorDBL::Yellow);
	Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), ColorDBL::White);
	Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), ColorDBL::Black);

	Rectangle wallR(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), ColorDBL::Red);
	Rectangle wallR_F(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), ColorDBL::Blue);
	Rectangle wallL_F(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), ColorDBL::White);
	wallL_F.theMaterial.isMirror = true;

	Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), ColorDBL::White); //In front of camera

	std::cout << "Initializing Lights...\n\n";

	//----LIGHTS----//
	LightSource areaLight2(glm::dvec3(6.0, 4.0, 4.5), glm::dvec3(8.0, 4.0, 4.5), glm::dvec3(6.0, -4.0, 4.5), glm::dvec3(8.0, -4.0, 4.5), 100, ColorDBL::White);

	//------------------------//

	Sphere sphere1(glm::dvec3(9, 0, -2), 2, ColorDBL::White);
	Sphere sphere2(glm::dvec3(8, 4, -4), 1, ColorDBL::Red);

	sphere1.theMaterial.isTransparent = true;
	sphere2.theMaterial.isMirror = true;
	Cube newCube(glm::dvec3(6, -4, -2), 1.5);
	newCube.setMirror(true);

	theRenderSettings.UserInputAndSettings();

	std::cout << "\nInitializing Camera...\n\n";
	Camera theCamera(glm::dvec3(0, -1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1), theRenderSettings);

	theRenderSettings.WriteSettingsToScreen(theCamera);

	std::cout << "\nRendering Image:\n";
	//--------------------RENDERING LOOP--------------------//
	int rowsDone = 0; // Concurrency fix

	const auto start = std::chrono::high_resolution_clock::now();

	if (theRenderSettings.s_useMulticore) {
		concurrency::parallel_for(size_t(0), (size_t)theCamera.GetResX(), [&](size_t _currentXpixel) {
			for (size_t _currentYpixel = 0; _currentYpixel < theCamera.GetResY(); _currentYpixel++) {
				for (size_t ssaa_sample = 0; ssaa_sample < theRenderSettings.GetAAIterations(); ssaa_sample++) {
					int currentIndex = _currentXpixel * theCamera.GetResY() + _currentYpixel;

					glm::dvec3 pixelOffset = theCamera.GetSuperSamplingPixelOffset(ssaa_sample, theRenderSettings.GetAAIterations());
					glm::dvec3 importanceDirection = theCamera.thePixels[currentIndex].position + pixelOffset - theEye;
					int calculatedIterations = theRenderSettings.GetTotalIterations();

					// Create importance ray
					Ray aRay(theEye, importanceDirection, ColorDBL::White, 0, theRenderSettings.s_maxMirrorBounces);
					glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), *LightSource::theLightSources[0], calculatedIterations);


					ColorDBL finalColor = aRay.GetRayColor() / theRenderSettings.GetAAIterations();
					// Save the resulting color information into that ray
					theCamera.thePixels[_currentYpixel * theCamera.GetResX() + _currentXpixel].pixelColor += finalColor;
				}
			}
			rowsDone++; // Concurrency fix

			DisplayLoadingBar(rowsDone, theCamera.GetResX(), currentlyWriting);

			});
	}
	else {
		for (size_t _currentXpixel = 0; _currentXpixel < theCamera.GetResX(); _currentXpixel++) {
			for (size_t _currentYpixel = 0; _currentYpixel < theCamera.GetResY(); _currentYpixel++) {

				for (size_t l = 0; l < LightSource::theLightSources.size(); l++) {

					Ray aRay(theEye, theCamera.thePixels[_currentYpixel * theCamera.GetResX() + _currentXpixel].position - theEye, ColorDBL::White, 0, theRenderSettings.s_maxMirrorBounces);

					glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), *LightSource::theLightSources[l], theRenderSettings.s_shadowrayIterations);

					theCamera.thePixels[_currentYpixel * theCamera.GetResX() + _currentXpixel].pixelColor += aRay.GetRayColor();
				}
			}
			rowsDone++;
			DisplayLoadingBar(rowsDone, theCamera.GetResX(), currentlyWriting);
		}
	}

	// Stop timer
	const auto stop = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::ratio<60>> duration = stop - start; //Log time in minutes for fun

	// Create file to save
	std::string fileName = GenerateFilename(theRenderSettings, theCamera, duration.count());
	std::ofstream img(fileName);

	// Create image out stream
	img << "P3" << std::endl;
	img << theCamera.GetResX() << " " << theCamera.GetResY() << std::endl;
	img << "255" << std::endl;

	// Logging
	std::cout << "\n =====[ Rendering complete after " << duration << "! ]======\n\n"
		<< "Writing to file:\n";

	// -----------{ Write all pixels to stream }----------- //
	rowsDone = 0; // Concurrency fix

	for (size_t i = 0; i < theCamera.GetResX(); i++) {
		for (size_t j = 0; j < theCamera.GetResY(); j++) {
			writeCurrentPixelToFile(theCamera, i, j, img, maxval, minval);

		}
		rowsDone++;
		DisplayLoadingBar(rowsDone, theCamera.GetResX(), false);
	}

	double pixelsPerSecond = (double)theCamera.thePixels.size() / (duration.count() * 60.0);
	// Logging
	std::cout << std::flush
		<< "/===============[ Render Successful! ]================\ \n"
		<< "  Total pixels processed: \n    " << theCamera.thePixels.size() << " px\n"
		<< "  Time Elapsed: \n    " << round((duration.count() * 60.0)) << " second(s)\n\n"
		<< "  Ray Tracing Speed: \n    " << round((60.0 * pixelsPerSecond / 10000.0)) / 100.0 << " Mil pixels/minute\n\n"
		<< "  Image saved as: \n    " << fileName << "\n"
		<< "\=====================================================/\n\n";

	return 0;
}


// Displays the Loading bar and its current progress
void DisplayLoadingBar(int& rowsDone, const int& x_res, bool _currentlyWriting) {

	rowsDone++;

	if (!_currentlyWriting && (int)floor((((double)x_res) / 50.0)) != 0 && rowsDone % (int)floor((((double)x_res) / 50.0)) == 0) {
		int percentDone = (int)floor(((double)rowsDone / (double)x_res) * 100.0);
		_currentlyWriting = true;
		std::cout << "\r [";

		for (int i = 0; i < (int)(floor((float)percentDone / 4.0)); i++) {
			std::cout << "I";
		}

		for (int i = 0; i < 50 - (int)(floor((float)percentDone / 4.0)); i++) {
			std::cout << "-";
		}

		std::cout << "] : ";

		std::cout << (int)floor(((double)rowsDone / (double)x_res) * 50.0) << "% \r";

		_currentlyWriting = false;
	}
}


// Writes a pixel of a camera intro an image stream
void writeCurrentPixelToFile(Camera& theCamera, size_t currentX, size_t currentY, std::ofstream& img, int& maxval, int& minval) {
	//-------Write image to file-------//
	int r = (int)floor(theCamera.thePixels[currentX * (theCamera.GetResY()) + currentY].pixelColor.r * 255 * exposureMultiplier);
	int g = (int)floor(theCamera.thePixels[currentX * (theCamera.GetResY()) + currentY].pixelColor.g * 255 * exposureMultiplier);
	int b = (int)floor(theCamera.thePixels[currentX * (theCamera.GetResY()) + currentY].pixelColor.b * 255 * exposureMultiplier);

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

	if ((r + g + b) / 3 > maxval) maxval = (int)round((r + g + b) / 3.0);
	if ((r + g + b) / 3 < minval) minval = (int)round((r + g + b) / 3.0);

	if (theRenderSettings.s_verboseDebugging) {
		std::cout << "PIXEL: ( " << currentX << " , " << currentY << " )" << std::endl;
		theCamera.DisplayPixelPosition(currentX, currentY);

		std::cout << theCamera.thePixels[currentX * theCamera.GetResY() + currentY].pixelColor.ToString()
			<< "Which corresponds to ints r: " << r << ", g: " << g << ", b: " << b
			<< "\n Raw pixelCol"
			<< "  r: " << theCamera.thePixels[currentX * theCamera.GetResY() + currentY].pixelColor.r
			<< ", g: " << theCamera.thePixels[currentX * theCamera.GetResY() + currentY].pixelColor.g
			<< ", b: " << theCamera.thePixels[currentX * theCamera.GetResY() + currentY].pixelColor.b
			<< "\n    With a mean of: " << (r + g + b) / 3.0 << "\n\n";
	}

	img << r << " " << g << " " << b << std::endl;
}

std::string GenerateFilename(RenderSettings renderSettings, Camera theCamera, double duration) {
	return std::to_string(theCamera.GetResX()) + "x" + std::to_string(theCamera.GetResY()) + "px__iterations-" + std::to_string((int)floor(renderSettings.s_shadowrayIterations)) + "__time-" + std::to_string(duration) + ".ppm";
}
