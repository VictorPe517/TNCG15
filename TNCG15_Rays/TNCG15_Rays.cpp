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
#include "ImageHandler.h"
#include "HelperFunctions.h"

std::vector<Object*> Object::theObjects;
std::vector<Polygon*> Polygon::thePolygons;
std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;
std::vector<Sphere*> Sphere::theSpheres;
std::vector<Cube> Cube::theCubes;
std::vector<LightSource*> LightSource::theLightSources;

//---------------------[ STANDARD SETTINGS ]---------------------//
RenderSettings theRenderSettings;
ImageHandler theImageHandler;
HelperFunctions theHelperFunctions;

double exposureMultiplier = 10;
//---------------------------------------------------------------//

int main()
{
	size_t pixelIndex = 0;

	int maxval = (int)-INFINITY;
	int minval = (int)INFINITY;

	glm::dvec3 theEye(-1.0, 0.0, 0.0);

	std::cout << "Initializing Scene Geometry...\n\n";

#pragma region Scene_1
	////------GEOMETRY------//
	//Rectangle cielingRect(glm::dvec3(5.0, 0.0, 5.0), glm::dvec3(5.0, 6.0, 0.0), ColorDBL::White);
	//Rectangle floorRect(glm::dvec3(5.0, 0.0, -5.0), glm::dvec3(-5.0, 6.0, 0.0), ColorDBL::White);
	//Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), ColorDBL::White);
	//Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), ColorDBL::White);
	//Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), ColorDBL::White);  //Behind camera
	//Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), ColorDBL::White); //In front of camera
	////-------WALLS-------//
	//Rectangle wallN(glm::dvec3(5.0, 6.0, 0), glm::dvec3(-5.0, 0.0, 5.0), ColorDBL::Yellow);
	//Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), ColorDBL::White);
	//Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), ColorDBL::White);
	//Rectangle wallR(glm::dvec3(5.0, -6.0, 0), glm::dvec3(5.0, 0.0, 5.0), ColorDBL::Red);
	//Rectangle wallR_F(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), ColorDBL::Blue);
	//Rectangle wallL_F(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), ColorDBL::White);
	//wallL_F.theMaterial.isMirror = true;
	//std::cout << "Initializing Lights...\n\n";
	////------LIGHTS------//
	//LightSource areaLight2(glm::dvec3(8.0, 4.0, 5), glm::dvec3(10.0, 4.0, 5), glm::dvec3(8.0, -4.0, 5), glm::dvec3(10.0, -4.0, 5), 100, ColorDBL::White);


	//////-----SUBJECTS-----//
	//Sphere sphere1(glm::dvec3(10.0, 0.0, -3), 1.0, ColorDBL::White);
	//sphere1.theMaterial.isTransparent = true;


	//Sphere sphere2(glm::dvec3(8.0, 4.0, -4.0), 0.75, ColorDBL::Red);
	//sphere2.theMaterial.isMirror = true;

	//Cube newCube(glm::dvec3(6, -5, -2), 1.5);
	//newCube.theMaterial.isMirror = true;

#pragma endregion

#pragma region Cage
	// X+ = forward, Y+ = left, Z+ = up
	Rectangle ceiling(glm::dvec3(5.0, 0.0, 5.0), glm::dvec3(5.0, 5.0, 0.0), ColorDBL::White);
	Rectangle floor(glm::dvec3(5.0, 0.0, -5.0), glm::dvec3(-5.0, 5.0, 0.0), ColorDBL::White);

	Rectangle wall_F(glm::dvec3(10.0, 0.0, 0.0), glm::dvec3(0.0, -5.0, 5.0), ColorDBL::White);
	wall_F.theMaterial.isMirror = true;

	Rectangle wall_L(glm::dvec3(5.0, 2.0, 0.0), glm::dvec3(-5.0, 0.0, 5.0), ColorDBL::Yellow);
	Rectangle wall_R(glm::dvec3(5.0, -5.0, 0.0), glm::dvec3(5.0, 0.0, 5.0), ColorDBL::Red);

	Sphere sphere1(glm::dvec3(8.0, 2.0, -3), 1.5, ColorDBL::White);
	sphere1.theMaterial.isMirror = true;

	Sphere sphere2(glm::dvec3(8.0, -2.0, -3.0), 1.5, ColorDBL::White);
	sphere2.theMaterial.isTransparent = true;

	LightSource areaLight2(glm::dvec3(3.0, 2.0, 4.99), glm::dvec3(7.0, 2.0, 4.99), glm::dvec3(3.0, -2.0, 4.99), glm::dvec3(7.0, -2.0, 4.99), 100, ColorDBL::White);

#pragma endregion

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
					glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), *LightSource::theLightSources[0]);

					ColorDBL finalColor = aRay.GetRayColor() / theRenderSettings.GetAAIterations();
					// Save the resulting color information into that ray
					theCamera.thePixels[_currentYpixel * theCamera.GetResX() + _currentXpixel].pixelColor += finalColor;
				}
			}
			rowsDone++; // Concurrency fix

			theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());

			});
	}
	else {
		for (size_t _currentXpixel = 0; _currentXpixel < theCamera.GetResX(); _currentXpixel++) {
			for (size_t _currentYpixel = 0; _currentYpixel < theCamera.GetResY(); _currentYpixel++) {

				for (size_t ssaa_sample = 0; ssaa_sample < theRenderSettings.GetAAIterations(); ssaa_sample++) {
					int currentIndex = _currentXpixel * theCamera.GetResY() + _currentYpixel;

					glm::dvec3 pixelOffset = theCamera.GetSuperSamplingPixelOffset(ssaa_sample, theRenderSettings.GetAAIterations());
					glm::dvec3 importanceDirection = theCamera.thePixels[currentIndex].position + pixelOffset - theEye;

					int calculatedIterations = theRenderSettings.GetTotalIterations();

					// Create importance ray
					Ray aRay(theEye, importanceDirection, ColorDBL::White, 0, theRenderSettings.s_maxMirrorBounces);
					glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), *LightSource::theLightSources[0]);

					ColorDBL finalColor = aRay.GetRayColor() / theRenderSettings.GetAAIterations();
					// Save the resulting color information into that ray
					theCamera.thePixels[_currentYpixel * theCamera.GetResX() + _currentXpixel].pixelColor += finalColor;
				}
			}
			rowsDone++;
			theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());
		}
	}

	const auto stop = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::ratio<60>> duration = stop - start; //Log time in minutes

	std::string fileName = theImageHandler.GenerateFilename(theRenderSettings, theCamera, duration.count());
	std::ofstream img(fileName);

	theImageHandler.CreateImageStream(img, theCamera);

	// Logging
	std::cout << "\n =====[ Rendering complete after " << duration << "! ]======\n\n" << "Writing to file:\n";
	rowsDone = 0;

#pragma region WriteToFile

	for (size_t i = 0; i < theCamera.GetResX(); i++) {
		for (size_t j = 0; j < theCamera.GetResY(); j++) {
			theImageHandler.writeCurrentPixelToStream(theCamera, i, j, img, theRenderSettings);
		}
		rowsDone++;
		theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());
	}

	double pixelsPerSecond = (double)theCamera.thePixels.size() / (duration.count() * 60.0);
	theImageHandler.DisplayRenderSuccessfulStats(theCamera, duration.count(), pixelsPerSecond, fileName);
#pragma endregion

	return 0;
}