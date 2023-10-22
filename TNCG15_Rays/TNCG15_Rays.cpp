// TNCG15_Rays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// INHERITANCE: https://msbrijuniversity.ac.in/assets/uploads/newsupdate/InheritanceinC++.pdf
// MULTITHREADING: https://www.educative.io/blog/modern-multithreading-and-concurrency-in-cpp

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <fstream>
#include <math.h>
#include "Camera.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "LightSource.h"
#include "Sphere.h"
#include <cassert>
#include <thread>
#include <ppl.h>
#include "Object.h"

class LightSource;

void writeCurrentPixelToFile(Camera& theCamera, size_t i, size_t j);

std::vector<Object*> Object::theObjects;
std::vector<Polygon*> Polygon::thePolygons;
std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;
std::vector<Sphere*> Sphere::theSpheres;

struct HitPoint{
public:
    glm::dvec3 point;
    Object* theObject;
};

double Ray::maxE = 0.0;

//-------SETTINGS-------//
double exposureMultiplier = 10;
double iterations = 50;

//----------------------//
int pixelIndex = 0;

std::ofstream img("picture.ppm");


int maxval = -4000;
int minval = 999999;

//-------------------------------------------------------------------------------------------------------//
int main()
{
    std::vector<std::thread> threads; //Sets up multi-threading capabilities

    std::cout << "Compilation successful. Welcome back commander. \n";

    glm::dvec3 theEye(-1, 0, 0.0);

    ColorDBL magenta = ColorDBL(1.0, 0.0, 1.0);
    ColorDBL red = ColorDBL(1.0,0.0,0.0);
    ColorDBL orange = ColorDBL(0.9, 0.4, 0.0);
    ColorDBL yellow = ColorDBL(1.0, 1.0, 0.0);
    ColorDBL green = ColorDBL(0.0, 1.0, 0.0);
    ColorDBL cyan = ColorDBL(0.0,1.0,1.0);
    ColorDBL blue = ColorDBL(0.0, 0.0, 1.0);
    ColorDBL white = ColorDBL(1.0, 1.0, 1.0);
    ColorDBL black = ColorDBL(1.0, 1.0, 1.0);

    std::cout << "Setting up primitives...\n\n";

    Camera theCamera(glm::dvec3(0, -1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1));

    //------GEOMETRY------//
        //------CIELING------//
        Rectangle cielingRect(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), magenta);
        Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), yellow);
        Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), red);
        
        //-------FLOOR-------//
        Rectangle floorRect(glm::dvec3(10, 6, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), green);

        //Rectangle testRect(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, -6, -5), glm::dvec3(13,6,5), white);

        Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), blue);  //Behind camera
        
        //-------WALLS-------//
        //Rectangle wallBlock(glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(10, -6, 5), white);
        //wallBlock.theMaterial.isMirror = true;
        Rectangle wallN(glm::dvec3(10, 6, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), yellow);
        Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), white);
        Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), black);
        Rectangle wallR(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), red);
        Rectangle wallR_F(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), orange);
        Rectangle wallL_F(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), white);
        wallL_F.theMaterial.isMirror = true;

        Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), white); //In front of camera

        LightSource areaLight(glm::dvec3(4.0, 2.0, 4.5), glm::dvec3(6.0, 2.0, 4.5), glm::dvec3(4.0, -2.0, 4.5), glm::dvec3(6.0, -2.0, 4.5), 100, white);
        //Rectangle wallTest(glm::dvec3(2.0, 1.0, 3), glm::dvec3(1.0, 1.0, 3), glm::dvec3(2.0, -1.0, 3), glm::dvec3(1.0, -1.0, 3), ColorDBL(0.4, 0.4, 0.4));
        //-----------------------//
        //Sphere sphere1(glm::dvec3(11, -1, 0), 2, white);
        Sphere sphere1(glm::dvec3(9, -3, -3), 2, white);
        sphere1.theMaterial.isMirror = true;
        
    std::cout << "Rendering & Writing image...\n\n";

    //--------------------RENDERING LOOP--------------------//
    img << "P3" << std::endl;
    img << Camera::x_res << " " << Camera::y_res << std::endl;
    img << "255" << std::endl;

    size_t maxSample = 4;
    double todaysSampleSize;

    concurrency::parallel_for(size_t(0), (size_t)Camera::x_res, [&](size_t i) 
    {
    //for (size_t i = 0; i < Camera::x_res; i++) {
        for (size_t j = 0; j < Camera::y_res; j++) {
            Ray aRay(theEye, theCamera.thePixels[i * Camera::x_res + j].position - theEye, white, 0, 5);

            glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), areaLight);

            theCamera.thePixels[i * Camera::x_res + j].pixelColor = aRay.RayColor;

        }
        //if (i % 50 == 0) std::cout << ((double)i / (double)Camera::x_res) * 100.0 << "% \n";
    //}
    });

    std::cout << "Writing pixel to file\n";
    pixelIndex = 0;

    for (size_t i = 0; i < Camera::x_res; i++) {
        for (size_t j = 0; j < Camera::y_res; j++) {
            writeCurrentPixelToFile(theCamera, i, j);
        }
    }

    std::cout << "Render Successful.\n\n";

    std::cout << "The image contains " << theCamera.thePixels.size() << " pixels.\n";
    std::cout << "The Ray::maxE was: " << Ray::maxE << "\n";
    std::cout << "Minval: " << minval << ", Maxval: " << maxval << "\n\n";

    system("explorer picture.ppm");
    
    return 0;
}

void writeCurrentPixelToFile(Camera& theCamera, size_t i, size_t j) {
    //-------Write image to file-------//
    int r = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.r * 255 * exposureMultiplier);
    int g = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.g * 255 * exposureMultiplier);
    int b = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.b * 255 * exposureMultiplier);

    //Give the other two channels some of the intensity of the highest colors
    if (r > 255) {
        int temp = (r - 255.0) / 50.0; 
        g += temp;
        b += temp;

        r = 255;
    }

    if (g > 255) {
        int temp2 = (g - 255) / 50;
        r += temp2;
        b += temp2;

        g = 255;
    }

    if (b > 255) {
        int temp3 = (b - 255) / 50;
        r += temp3;
        g += temp3;

        b = 255;
    }

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if ((r + g + b) / 3 > maxval) maxval = (r + g + b) / 3;
    if ((r + g + b) / 3 < minval) minval = (r + g + b) / 3;

    //std::cout << theCamera.thePixels[i * Camera::x_res + j].pixelColor.ToString() << "Which corresponds to ints r: " << r << ", g: " << g << ", b: " << b
    //    << "\n Raw pixelCol r: " << theCamera.thePixels[i * Camera::x_res + j].pixelColor.r << ", g: " << theCamera.thePixels[i * Camera::x_res + j].pixelColor.g << ", b:" << theCamera.thePixels[i * Camera::x_res + j].pixelColor.b << "\n    With a mean of: " << (r + g + b) / 3.0 << "\n\n";

    img << r << " " << g << " " << b << std::endl;

    pixelIndex++;
}

