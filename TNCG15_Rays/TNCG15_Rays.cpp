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
#include <cassert>
#include <thread>

std::vector<Polygon*> Polygon::thePolygons;
std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;
double Ray::maxE = 0.0;


//-------------------------------------------------------------------------------------------------------//
int main()
{
    std::vector<std::thread> threads; //Sets up multi-threading capabilities

    std::cout << "Compilation successful. Welcome back commander. \n";

    glm::dvec3 theEye(-1.0, 0.0, 0.0);

    ColorDBL magenta = ColorDBL(1.0, 0.0, 1.0);
    ColorDBL red = ColorDBL(1.0,0.0,0.0);
    ColorDBL orange = ColorDBL(0.9, 0.4, 0.0);
    ColorDBL yellow = ColorDBL(1.0, 1.0, 0.0);
    ColorDBL green = ColorDBL(0.0, 1.0, 0.0);
    ColorDBL cyan = ColorDBL(0.0,1.0,1.0);
    ColorDBL blue = ColorDBL(0.0, 0.0, 1.0);
    ColorDBL white = ColorDBL(1.0, 1.0, 1.0);


    std::cout << "Setting up primitives...\n\n";

    LightSource areaLight(glm::dvec3(5, 6, 4.8), glm::dvec3(8, 6, 4.8), glm::dvec3(5, 4, 4.8), glm::dvec3(8, 4, 4.8), 100, white);
    Camera theCamera(glm::dvec3(0, -1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1));
    
    //------GEOMETRY------//
        //------CIELING------//
        Rectangle cielingRect(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), white);
        //Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), white);
        //Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), white);
        
        //-------FLOOR-------//
        //Rectangle floorRect(glm::dvec3(10, 6, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), white);

        //Rectangle testRect(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, -6, -5), glm::dvec3(13,6,5), white);

        Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), white);  //Behind camera
        
        
        //-------WALLS-------//
        Rectangle wallN(glm::dvec3(10, 6, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), white);
        Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), white);
        Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), white);
        Rectangle wallS(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), white);
        Rectangle wallSW(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), white);
        Rectangle wallSE(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), white);

        Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), white); //In front of camera

        //-----------------------//
        

    std::cout << "Rendering & Writing image...\n\n";

    //--------------------RENDERING LOOP--------------------//
    int pixelIndex = 0;

    std::ofstream img("picture.ppm");
    img << "P3" << std::endl;
    img << Camera::x_res << " " << Camera::y_res << std::endl;
    img << "255" << std::endl;

    int maxval = -4000;
    int minval = 999999;

    double iterations = 1;


    for (size_t i = 0; i < Camera::x_res; i++) {
        for (size_t j = 0; j < Camera::y_res; j++) {
            Ray aRay(theEye, theCamera.thePixels[pixelIndex].position-theEye, white, 0);

            for (size_t l = 0; l < Polygon::thePolygons.size(); l++) {
                if (glm::dot((*Polygon::thePolygons[l]).normal(), aRay.direction) < 0.0 && (*Polygon::thePolygons[l]).getIntersection(aRay) != glm::dvec3(-9999, -9999, -9999)) {
                    for (size_t iter = 0; iter < iterations; iter++) {
                        glm::dvec3 thePoint = areaLight.getRandomPoint();
                        Ray newRay(thePoint, (*Polygon::thePolygons[l]).getIntersection(aRay) - thePoint, white, areaLight.radiance);

                        theCamera.thePixels[i * Camera::x_res + j].pixelColor += newRay.calcIrradiance((*Polygon::thePolygons[l]).normal(), areaLight.normal(), newRay.radiance, areaLight.calculateArea()) / iterations; //Give color of rectangle or triangle to pixel
                    }
                    break;
                }
            }

            //-------Write image to file-------//
            int r = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.r * 0.07 * 255.0 / 3200.0);
            int g = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.g * 0.07 * 255.0 / 3200.0);
            int b = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.b * 0.07 * 255.0 / 3200.0);

            if (r > maxval) maxval = r;
            if (r < minval) minval = r;

            //std::cout << theCamera.thePixels[i * Camera::x_res + j].pixelColor.ToString() << "Which corresponds to ints r: " << r << ", g: " << g << ", b: " << b
            //    << "\n Raw pixelCol r: " << theCamera.thePixels[i * Camera::x_res + j].pixelColor.r << ", g: " << theCamera.thePixels[i * Camera::x_res + j].pixelColor.g << ", b:" << theCamera.thePixels[i * Camera::x_res + j].pixelColor.b << "\n    With a mean of: " << (r + g + b) / 3.0 << "\n\n";



            img << r << " " << g << " " << b << std::endl;

            pixelIndex++;
        }
        if (i % 50 == 0) std::cout << ((double)i / (double)Camera::x_res)*100.0 << "% \n";
    }

    std::cout << "Render Successful.\n\n";

    std::cout << "The image contains " << theCamera.thePixels.size() << " pixels.\n";
    std::cout << "The Ray::maxE was: " << Ray::maxE << "\n";
    std::cout << "Minval: " << minval << ", Maxval: " << maxval << "\n\n";

    system("explorer picture.ppm");
    
    return 0;
}


Pixel::Pixel()
{
}

