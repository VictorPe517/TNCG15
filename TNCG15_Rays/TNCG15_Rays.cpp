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
unsigned int Ray::nonHitters;
unsigned int Ray::rayAmount;

//---------------------[ STANDARD SETTINGS ]---------------------//
RenderSettings theRenderSettings;
ImageHandler theImageHandler;
HelperFunctions theHelperFunctions;
bool debug = true;
//---------------------------------------------------------------//

int main()
{
	std::string sceneName;
	Ray::nonHitters = 0;
	Ray::rayAmount = 0;

	size_t pixelIndex = 0;
	int maxval = (int)-INFINITY;
	int minval = (int)INFINITY;

	glm::dvec3 theEye(-1.0, 0.0, 0.0);

	std::cout << "Initializing Scene Geometry...\n\n";

#pragma region Scene_1
	////------GEOMETRY------//
	sceneName = "Scene1";
	Rectangle floorRect(glm::dvec3(5.0, 0.0, -5.0), glm::dvec3(-5.0, 6.0, 0.0), ColorDBL::White);
	Rectangle cielingRect(glm::dvec3(5.0, 0.0, 5.0), glm::dvec3(5.0, 6.0, 0.0), ColorDBL::White);
	Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), ColorDBL::White);
	Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), ColorDBL::White);
	Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), ColorDBL::White);  //Behind camera
	Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), ColorDBL::White); //In front of camera
	//-------WALLS-------//
	Rectangle wallL(glm::dvec3(5.0, 6.0, 0), glm::dvec3(-5.0, 0.0, 5.0), ColorDBL::Green);
	Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), ColorDBL::White);
	Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), ColorDBL::White);
	Rectangle wallR(glm::dvec3(5.0, -6.0, 0), glm::dvec3(5.0, 0.0, 5.0), ColorDBL::Red);
	Rectangle wallR_F(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), ColorDBL::Blue);
	Rectangle wallL_F(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), ColorDBL::White);
	wallL_F.theMaterial.isMirror = true;
	std::cout << "Initializing Lights...\n\n";

	//------LIGHTS------//
	//LightSource areaLight(glm::dvec3(8 , 2.0, 4.999), glm::dvec3(10 , 2.0, 4.999), glm::dvec3(8 , -2.0, 4.999), glm::dvec3(10 , -2.0, 4.999), 75.0, ColorDBL::White);
	//LightSource areaLight2(glm::dvec3(0, 2.0, 4.999), glm::dvec3(2, 2.0, 4.999), glm::dvec3(0, -2.0, 4.999), glm::dvec3(2, -2.0, 4.999), 75.0, ColorDBL::White);
	//
	//LightSource areaLight1(glm::dvec3(3.0 + 2.0, 1.0, 4.999), glm::dvec3(5.0 + 2.0, 1.0, 4.999), glm::dvec3(3.0 + 2.0, -1.0, 4.999),glm::dvec3(5.0 + 2.0, -1.0, 4.999), 100.0, ColorDBL::PureBlue);
	//LightSource areaLight2(glm::dvec3(6.0 + 2.0, -1.0, 4.999),glm::dvec3(8.0 + 2.0, -1.0, 4.999),glm::dvec3(6.0 + 2.0, -3.0, 4.999),glm::dvec3(8.0 + 2.0, -3.0, 4.999), 100.0, ColorDBL::PureGreen);
	//LightSource areaLight3(glm::dvec3(6.0 + 2.0, 3.0, 4.999), glm::dvec3(8.0 + 2.0, 3.0, 4.999), glm::dvec3(6.0 + 2.0, 1.0, 4.999), glm::dvec3(8.0 + 2.0, 1.0, 4.999),  100.0, ColorDBL::PureRed);

	LightSource areaLight1(glm::dvec3(1.0 + 2.0, 1.0, 4.999), glm::dvec3(3.0 + 2.0, 1.0, 4.999), glm::dvec3(1.0 + 2.0, -1.0, 4.999), glm::dvec3(3.0 + 2.0, -1.0, 4.999), 50.0, ColorDBL::White);
	LightSource areaLight2(glm::dvec3(6.0 + 2.0, -3.0, 4.999), glm::dvec3(8.0 + 2.0, -3.0, 4.999), glm::dvec3(6.0 + 2.0, -4.0, 4.999), glm::dvec3(8.0 + 2.0, -4.0, 4.999), 50.0, ColorDBL::White);
	LightSource areaLight3(glm::dvec3(6.0 + 2.0, 3.0, 4.999), glm::dvec3(8.0 + 2.0, 3.0, 4.999), glm::dvec3(6.0 + 2.0, 1.0, 4.999), glm::dvec3(8.0 + 2.0, 1.0, 4.999), 50.0, ColorDBL::White);

	//-----SUBJECTS-----//
	Sphere sphere1(glm::dvec3(8, -3.0, -1), 1.0, ColorDBL::White);
	sphere1.theMaterial.MatColor = ColorDBL::White;
	sphere1.theMaterial.isTransparent = true;

	Sphere sphere2(glm::dvec3(9.0, 4.0, -4.25), 0.75, ColorDBL::White);
	sphere2.theMaterial.MatColor = ColorDBL::White;
	sphere2.theMaterial.isMirror = true;

	double size = 2.0;
	Sphere sphere3(glm::dvec3(9.0, -0.5, -5.0+size), size, ColorDBL::White);
	sphere3.theMaterial.MatColor = ColorDBL::White;

#pragma endregion

#pragma region Cage
	////// X+ = forward, Y+ = left, Z+ = up
	//sceneName = "Cage";
	//Rectangle ceiling(glm::dvec3(5.0, 0.0, 5.0), glm::dvec3(5.0, 5.0, 0.0), ColorDBL::White);
	//Rectangle floor(glm::dvec3(5.0, 0.0, -5.0), glm::dvec3(-5.0, 5.0, 0.0), ColorDBL::White);

	//Rectangle wall_F(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), ColorDBL::Blue);
	//wall_F.theMaterial.isMirror = true;

	//Rectangle wall_L(glm::dvec3(5.0, 5.0, 0.0), glm::dvec3(-5.0, 0.0, 5.0), ColorDBL::Yellow);
	//Rectangle wall_R(glm::dvec3(5.0, -5.0, 0.0), glm::dvec3(5.0, 0.0, 5.0), ColorDBL::Red);

	//double size = 1.75;
	//Sphere sphere2(glm::dvec3(8.0, -(size+1), -5+size), size, ColorDBL::White);
	//sphere2.theMaterial.isTransparent = true;

	//Sphere sphere1(glm::dvec3(8.0, (size+1), -5 + size), size, ColorDBL::White);
	//sphere1.theMaterial.isMirror = true;

	//LightSource areaLight2(glm::dvec3(4.0, 1.0, 4.99), glm::dvec3(6.0, 1.0, 4.99), glm::dvec3(4.0, -1.0, 4.99), glm::dvec3(6.0, -1.0, 4.99), 100, ColorDBL::White);

#pragma endregion

#pragma region LegacyScene
	//	////------GEOMETRY------//
	//sceneName = "LegacyScene";
	//Rectangle floorRect(glm::dvec3(5.0, 0.0, -5.0), glm::dvec3(-5.0, 6.0, 0.0), ColorDBL::Green);
	//Rectangle cielingRect(glm::dvec3(5.0, 0.0, 5.0), glm::dvec3(5.0, 6.0, 0.0), ColorDBL::White);
	//Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), ColorDBL::White);
	//Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), ColorDBL::White);
	//Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), ColorDBL::Blue);  //Behind camera
	//Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), ColorDBL::White); //In front of camera
	////-------WALLS-------//
	//Rectangle wallL(glm::dvec3(5.0, 6.0, 0), glm::dvec3(-5.0, 0.0, 5.0), ColorDBL::Yellow);
	//Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), ColorDBL::White);

	//Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), ColorDBL::White);
	//Rectangle wallR(glm::dvec3(5.0, -6.0, 0), glm::dvec3(5.0, 0.0, 5.0), ColorDBL::Red);
	//Rectangle wallR_F(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), ColorDBL::Orange);
	//Rectangle wallL_F(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), ColorDBL::White);
	//std::cout << "Initializing Lights...\n\n";

	//// Legacy comparison light
	//LightSource areaLight(glm::dvec3(4.0, 3.0, 4.5), glm::dvec3(5.0, 3.0, 4.5), glm::dvec3(4.0, -3.0, 4.5), glm::dvec3(5.0, -3.0, 4.5), 100, ColorDBL::White);

	//// Legacy comparison spheres
	//Sphere sphere1(glm::dvec3(9, 0, -2), 2, ColorDBL::Red);
	//sphere1.theMaterial.isMirror = false;

	//Sphere sphere2(glm::dvec3(8.0, 4.0, -4.0), 1.0, ColorDBL::White);

	//Sphere sphere3(glm::dvec3(6.0, -3.0, -3.0), 2.0, ColorDBL::White);
	//sphere3.theMaterial.isTransparent = false;
#pragma endregion

	theRenderSettings.UserInputAndSettings();

	std::cout << "\n Initializing Camera...\n\n";
	Camera theCamera(glm::dvec3(0, -1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1), theRenderSettings);

	theRenderSettings.WriteSettingsToScreen(theCamera);
	std::cout << "\n Constructing Ray Path & Calculating Lighting: \n";


	//--------------------RENDERING LOOP--------------------//
	int rowsDone = 0; // Concurrency fix
	const auto start = std::chrono::high_resolution_clock::now();

	if (theRenderSettings.s_useMulticore) {
		concurrency::parallel_for(size_t(0), (size_t)theCamera.GetResX(), [&](size_t _currentXpixel) {
			for (size_t _currentYpixel = 0; _currentYpixel < theCamera.GetResY(); _currentYpixel++) {
				int currentPixelIndex = _currentXpixel * theCamera.GetResY() + _currentYpixel; // The pixel we are processing

				for (size_t ssaa_sample = 0; ssaa_sample < theRenderSettings.GetAAIterations(); ssaa_sample++) {

					glm::dvec3 pixelOffset = theCamera.GetSuperSamplingPixelOffset(ssaa_sample, theRenderSettings.GetAAIterations()); // Get random pixel offset
					glm::dvec3 importanceDirection = theCamera.thePixels[currentPixelIndex].position + pixelOffset - theEye; // Direction of import

					Ray* importanceRay = new Ray(theEye, importanceDirection, ColorDBL::White);
					importanceRay->CalculateRayPath((Object::theObjects), LightSource::theLightSources);
					importanceRay->CalculateRadianceFlow((Object::theObjects), LightSource::theLightSources);

					ColorDBL finalColor = importanceRay->GetRayColor() / (double)theRenderSettings.GetAAIterations();
					theCamera.thePixels[currentPixelIndex].pixelColor += finalColor;

					delete importanceRay;
				}
			}
			rowsDone++;
			theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());

			});
	}
	else {
		for (size_t _currentXpixel = 0; _currentXpixel < theCamera.GetResX(); _currentXpixel++) {
			for (size_t _currentYpixel = 0; _currentYpixel < theCamera.GetResY(); _currentYpixel++) {
				int currentPixelIndex = _currentXpixel * theCamera.GetResY() + _currentYpixel; // The pixel we are processing
				//std::cout << " Index : [" << currentPixelIndex << "]\n";
				
				for (size_t ssaa_sample = 0; ssaa_sample < theRenderSettings.GetAAIterations(); ssaa_sample++) {
					
					glm::dvec3 pixelOffset = theCamera.GetSuperSamplingPixelOffset(ssaa_sample, theRenderSettings.GetAAIterations()); // Get random pixel offset
					glm::dvec3 importanceDirection = theCamera.thePixels[currentPixelIndex].position + pixelOffset - theEye; // Direction of import

					Ray* importanceRay = new Ray(theEye, importanceDirection, ColorDBL::White);
					importanceRay->CalculateRayPath((Object::theObjects), LightSource::theLightSources);
					importanceRay->CalculateRadianceFlow((Object::theObjects), LightSource::theLightSources);

					ColorDBL finalColor = importanceRay->GetRayColor() / (double)theRenderSettings.GetAAIterations();
					theCamera.thePixels[currentPixelIndex].pixelColor += finalColor;

					delete importanceRay;
				}
			}
			rowsDone++;
			theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());
		}
	}


	const auto stop = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::ratio<60>> duration = stop - start; //Log time in minutes

	std::string fileName = theImageHandler.GenerateFilename(theRenderSettings, theCamera, duration.count(), sceneName);
	std::ofstream img(fileName);

	theImageHandler.CreateImageStream(img, theCamera);

	// Logging
	std::cout << "\n =====[ Rendering complete after " << duration << "! ]======\n\n" << "Writing to file:\n";
	rowsDone = 0;



#pragma region WriteToFile

	for (size_t i = 0; i < theCamera.GetResX(); i++) {
		for (size_t j = 0; j < theCamera.GetResY(); j++) {
			int currentPixelIndex = i * theCamera.GetResY() + j;
			//std::cout << " Writing Index : [" << currentPixelIndex << "]\n";
			//theImageHandler.writeCurrentPixelToStream(theCamera, i, j, img, theRenderSettings, maxRGBdistance, minRGBdistance);
			theImageHandler.writeCurrentPixelToStream(theCamera, currentPixelIndex, img, theRenderSettings);
		}
		rowsDone++;
		theHelperFunctions.DisplayLoadingBar(rowsDone, theCamera.GetResX());
	}

	double pixelsPerSecond = (double)theCamera.thePixels.size() / (duration.count() * 60.0);
	theImageHandler.DisplayRenderSuccessfulStats(theCamera, duration.count(), pixelsPerSecond, fileName);
#pragma endregion

	if (debug) {
		std::cout << "DEBUG:\n Rays that didn't hit nothing: [" << Ray::nonHitters << "]\n"
			<< "             Out of: [" << Ray::rayAmount << "]\n"
			<< "           Which is: " << round(((double)Ray::nonHitters / (double)Ray::rayAmount))*100.0 << "%\n";
	}

	return 0;
}