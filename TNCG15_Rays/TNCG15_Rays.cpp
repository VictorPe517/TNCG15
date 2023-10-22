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
        //wallL_F.theMaterial.isMirror = true;

        Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), white); //In front of camera

        LightSource areaLight(glm::dvec3(4.0, 2.0, 4.5), glm::dvec3(5.0, 2.0, 4.5), glm::dvec3(4.0, -2.0, 4.5), glm::dvec3(5.0, -2.0, 4.5), 100, white);
        //Rectangle wallTest(glm::dvec3(2.0, 1.0, 3), glm::dvec3(1.0, 1.0, 3), glm::dvec3(2.0, -1.0, 3), glm::dvec3(1.0, -1.0, 3), ColorDBL(0.4, 0.4, 0.4));
        //-----------------------//
        //Sphere sphere1(glm::dvec3(11, -1, 0), 2, white);
        Sphere sphere1(glm::dvec3(9, 0, 0), 2, white);
        sphere1.theMaterial.isMirror = true;
        
        

    std::cout << "Rendering & Writing image...\n\n";

    //--------------------RENDERING LOOP--------------------//
    img << "P3" << std::endl;
    img << Camera::x_res << " " << Camera::y_res << std::endl;
    img << "255" << std::endl;

    //for(size_t i = 0; i < Camera::x_res; i++) {
    //    for (size_t j = 0; j < Camera::y_res; j++) {
    //            Ray aRay(theEye, theCamera.thePixels[pixelIndex].position - theEye, white, 0);

    //            for (size_t l = 0; l < Polygon::thePolygons.size(); l++) {
    //                glm::dvec3 possibleIntersection = (*Polygon::thePolygons[l]).getIntersection(aRay);
    //                if (glm::dot((*Polygon::thePolygons[l]).normal(), aRay.direction) < 0.0 && possibleIntersection != glm::dvec3(-9999, -9999, -9999)) {
    //                    if (Polygon::thePolygons[l]->getMaterial().isMirror) {
    //                        Ray mirrorRay(possibleIntersection, aRay.getReflectedDirection(possibleIntersection, (*Polygon::thePolygons[l]).normal()),white,0);

    //                        glm::dvec3 secondHit = (*Polygon::thePolygons[l]).getIntersection(mirrorRay);

    //                        for (size_t iter = 0; iter < iterations; iter++) {
    //                            glm::dvec3 thePoint = areaLight.getRandomPoint();
    //                            Ray newRay(thePoint, secondHit - thePoint, white, areaLight.radiance);

    //                            theCamera.thePixels[i * Camera::x_res + j].pixelColor += mirrorRay.calcIrradiance((*Polygon::thePolygons[l]).normal(), areaLight.normal(), mirrorRay.radiance, areaLight.calculateArea(), secondHit, thePoint) / iterations; //Give color of rectangle or triangle to pixel
    //                        }
    //                        //theCamera.thePixels[i * Camera::x_res + j].pixelColor *= (*Polygon::thePolygons[l]).getColor();
    //                        break;
    //                        
    //                    }
    //                    else {
    //                        for (size_t iter = 0; iter < iterations; iter++) {
    //                            glm::dvec3 thePoint = areaLight.getRandomPoint();
    //                            Ray newRay(thePoint, possibleIntersection - thePoint, white, areaLight.radiance);

    //                            theCamera.thePixels[i * Camera::x_res + j].pixelColor += newRay.calcIrradiance((*Polygon::thePolygons[l]).normal(), areaLight.normal(), newRay.radiance, areaLight.calculateArea(), possibleIntersection, thePoint) / iterations; //Give color of rectangle or triangle to pixel
    //                        }
    //                        theCamera.thePixels[i * Camera::x_res + j].pixelColor *= (*Polygon::thePolygons[l]).getColor();
    //                        break;
    //                    }
    //                }
    //            }

    //            if (glm::dot(sphere1.normal(aRay), aRay.direction) < 0.0 && (sphere1.getIntersection(aRay)) != glm::dvec3(-9999, -9999, -9999)) {
    //                theCamera.thePixels[i * Camera::x_res + j].pixelColor = ColorDBL(0, 0, 0);
    //                for (size_t iter = 0; iter < iterations; iter++) {
    //                    glm::dvec3 thePoint = areaLight.getRandomPoint();
    //                    Ray newRay(thePoint, (sphere1.getIntersection(aRay) - thePoint), white, areaLight.radiance);

    //                    theCamera.thePixels[i * Camera::x_res + j].pixelColor += newRay.calcIrradiance(sphere1.normal(aRay), areaLight.normal(), newRay.radiance, areaLight.calculateArea(), sphere1.getIntersection(aRay), thePoint) / iterations; //Give color of rectangle or triangle to pixel
    //                    //theCamera.thePixels[i * Camera::x_res + j].pixelColor = ColorDBL(1, 1, 1);
    //                }
    //            }

    //            writeCurrentPixelToFile(theCamera, i, j);

    //    }
    //    if (i % 50 == 0) std::cout << ((double)i / (double)Camera::x_res)*100.0 << "% \n";
    //}
    size_t maxSample = 4;
    double todaysSampleSize;

    for (size_t i = 0; i < Camera::x_res; i++) {
        for (size_t j = 0; j < Camera::y_res; j++) {
            Ray aRay(theEye, theCamera.thePixels[pixelIndex].position - theEye, white, 0, 5);

            glm::dvec3 hitPos = aRay.getPointOfIntersection((Object::theObjects), areaLight);
            
            theCamera.thePixels[i * Camera::x_res + j].pixelColor = aRay.RayColor;
                
            writeCurrentPixelToFile(theCamera, i, j);
        }
        if (i % 50 == 0) std::cout << ((double)i / (double)Camera::x_res) * 100.0 << "% \n";
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

