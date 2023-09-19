// TNCG15_Rays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// INHERITANCE: https://msbrijuniversity.ac.in/assets/uploads/newsupdate/InheritanceinC++.pdf

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

std::vector<Polygon> Polygon::thePolygons;
std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;
double Ray::maxE = 0.0;


//-------------------------------------------------------------------------------------------------------//
int main()
{

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

    //------GEOMETRY------//
        //------CIELING------//
        Rectangle cielingRect(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), red);
        Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), orange);
        Triangle cielingTri2(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), yellow);
        
        //-------FLOOR-------//
        Rectangle floorRect(glm::dvec3(10, 6, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), blue);
        Triangle floorTri1(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), cyan);
        Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), magenta);
        
        //-------WALLS-------//
        Rectangle wallN(glm::dvec3(10, 6, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), green);
        Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), green);
        Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), green);
        Rectangle wallS(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), green);
        Rectangle wallSW(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), green);
        Rectangle wallSE(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), green);

        Camera theCamera(glm::dvec3(0, -1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1));


        LightSource areaLight(glm::dvec3(0, 3, 4.8), glm::dvec3(5, 3, 4.8), glm::dvec3(5, -3, 4.8), glm::dvec3(0, -3, 4.8), 100, white);
        //-----------------------//


    std::cout << "Rendering image...\n\n";

    //--------------------RENDERING LOOP--------------------//
    int pixelIndex = 0;

    for (size_t i = 0; i < Camera::x_res; i++) {

        for (size_t j = 0; j < Camera::y_res; j++) {
            Ray aRay(theEye, theCamera.thePixels[pixelIndex].position-theEye, white, 0);



            for (size_t l = 0; l < Rectangle::theRectangles.size(); l++) {
                if (glm::dot(Rectangle::theRectangles[l].normal(), aRay.direction) < 0 && Rectangle::theRectangles[l].getIntersection(aRay) != glm::dvec3(0,0,0)) {
                    //std::cout << "Intersection found!\n";
                    //theCamera.thePixels[i * Camera::x_res + j].pixelColor = Rectangle::theRectangles[l].Color; //Give color of rectangle or triangle to pixel
                    for (size_t iter = 0; iter < 90; iter++) {
                        glm::dvec3 thePoint = areaLight.getRandomPoint();
                        Ray newRay(thePoint, Rectangle::theRectangles[l].getIntersection(aRay) - thePoint, white, areaLight.radiance);

                        theCamera.thePixels[i * Camera::x_res + j].pixelColor += newRay.calcIrradiance(Rectangle::theRectangles[l].normal(), areaLight.normal(), newRay.radiance); //Give color of rectangle or triangle to pixel
                    }

                    
                }
                //theCamera.thePixels[i * Camera::x_res + j].pixelColor *= Rectangle::theRectangles[l].Color;
            }

            for (size_t l = 0; l < Triangle::theTriangles.size(); l++) {
                if (glm::dot(Triangle::theTriangles[l].normal(), aRay.direction) < 0 && Triangle::theTriangles[l].getIntersection(aRay) != glm::dvec3(0, 0, 0)) {
                    //std::cout << "Intersection found!\n";
                    //theCamera.thePixels[i * Camera::x_res + j].pixelColor = Rectangle::theRectangles[l].Color; //Give color of rectangle or triangle to pixel
                    for (size_t iter = 0; iter < 90; iter++) {
                        glm::dvec3 thePoint = areaLight.getRandomPoint();
                        Ray newRay(thePoint, Triangle::theTriangles[l].getIntersection(aRay) - thePoint, white, areaLight.radiance);

                        theCamera.thePixels[i * Camera::x_res + j].pixelColor += newRay.calcIrradiance(Triangle::theTriangles[l].normal(), areaLight.normal(), newRay.radiance); //Give color of rectangle or triangle to pixel
                    }


                }
                //theCamera.thePixels[i * Camera::x_res + j].pixelColor *= Triangle::theTriangles[l].Color;
            }

            pixelIndex++;
        }
    }
    std::cout << "Rendering Successful.\n";

    std::cout << "The image contains " << theCamera.thePixels.size() << " pixels.\n";

    std::cout << "Writing the image to disk... \n";
    
    //--------------------WRITING THE IMAGE TO PICTURE.PPM---------------//
    std::ofstream img("picture.ppm");
    img << "P3" << std::endl;
    img << Camera::x_res << " " << Camera::y_res << std::endl;
    img << "255" << std::endl;

    for (int i = 0; i < sqrt(theCamera.thePixels.size()); i++) {
        for (int j = 0; j < sqrt(theCamera.thePixels.size()); j++) {
            int r = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.r / Ray::maxE * 254.0);
            int g = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.g / Ray::maxE * 254.0);
            int b = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.b / Ray::maxE * 254.0);

            //std::cout << (r + b + g) / 3.0 << "\n";

            img << r << " " << g << " " << b << std::endl;
        }
    }

    system("explorer picture.ppm");
    
}


Pixel::Pixel()
{
}

