// TNCG15_Rays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// INHERITANCE: https://msbrijuniversity.ac.in/assets/uploads/newsupdate/InheritanceinC++.pdf

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <fstream>
#include <math.h>



class Ray {
public:
    Ray(glm::dvec3 start, glm::dvec3 dir): startPosition{start}, direction{dir} {}
    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
    glm::dvec3 direction = glm::dvec3(0, 0, 0);
    Ray* prevRay;
    Ray* nextRay;
};


class ColorDBL {
public:
    ColorDBL(double _r, double _g, double _b):r{_r}, g{_g}, b{_b}{}
    double r, g, b;
};


class Pixel {
public:
    Pixel();

    Pixel(glm::dvec3 pos): position{pos}{}

    ColorDBL pixelColor = ColorDBL(0, 0, 0);
    glm::dvec3 position = glm::dvec3(0, 0, 0);

    void SetPosition(glm::dvec3 pos) {
        position = pos;
    }
};

class Camera {
public:
    Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4) : c1{ _c1 }, c2{ _c2 }, c3{ _c3 }, c4{ _c4 }{
        double delta = 2.0 / x_res;
        //We have our camera position from the above corners, !!!  X = 0  !!!
        //We need to set the position of each pixel in the camera to make sense
        for (size_t i = 0; i < x_res; i++) {
            for (size_t j = 0; j < y_res; j++) {
                thePixels.push_back(Pixel(glm::dvec3(0.0, -1.0*(j * delta - (1.0f-delta)), -1.0*(i * delta - (1.0f - delta))))); //TODO: wtf multiplicera in minustecken
            }
        }
    }

    static const int x_res = 1500;   //Static:   Shared between all instances of Camera
    static const int y_res = 1500;   //Const:    We pinky promise to the compiler that we won't change the resolution of the camera during runtime.

    glm::dvec3 c1, c2, c3, c4;
    std::vector<Pixel> thePixels;
};

class Polygon {
public:
    Polygon() = default;
    std::vector<Polygon> thePolygons;
};

class Triangle : public Polygon{
public:
    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, Color{ _Color } {
        theTriangles.push_back(*this);
        thePolygons.push_back(*this);
    }

    glm::dvec3 normal() {
        glm::dvec3 direction = glm::cross(v2 - v1,v3 - v1);
        return direction/glm::length(direction);
    }

    bool calculateIntersection(Ray theRay) {
        glm::dvec3 dir = theRay.direction;
        glm::dvec3 start = theRay.startPosition;
        glm::dvec3 vertex = this->v2;
        glm::dvec3 c1 = v3 - v2;
        glm::dvec3 c2 = v1 - v2;


        double t = glm::dot((vertex - start), this->normal()) / (glm::dot(dir, this->normal()));

        glm::dvec3 possibleIntersection = start + t * dir; //xi = s + t*d

        double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
        double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));

        return(a <= 0.0, b <= 1.0);
    };

    std::vector<Triangle> get_vec() {
        return theTriangles;
    }

    glm::dvec3 v1, v2, v3;
    ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
    static std::vector<Triangle> theTriangles;
};

class Rectangle: public Polygon {
public:
    Rectangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{_v4}, Color{_Color}{
        thePolygons.push_back(*this); 
        theRectangles.push_back(*this);
    }

    glm::dvec3 normal() {
        glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
        //std::cout << glm::to_string(direction) << ",    with length: " << glm::length(direction) << "\n";  //Debug shenanigans
        return direction / glm::length(direction);
    }

    bool calculateIntersection(Ray theRay) {
        glm::dvec3 vertex = v2;

        glm::dvec3 c1 = v3 - vertex;
        glm::dvec3 c2 = v1 - vertex;

        double t = glm::dot((this->v2 - theRay.startPosition), this->normal()) / (glm::dot(theRay.direction, this->normal()));

        glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

        double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
        double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));
        
        return(a <= 1.0 && a >= 0.0 && b >= 0.0 && b <= 1.0);
    };

    glm::dvec3 v1, v2, v3, v4;
    ColorDBL Color = ColorDBL(0,0,0);
    static std::vector<Rectangle> theRectangles;
};

std::vector<Rectangle> Rectangle::theRectangles;
std::vector<Triangle> Triangle::theTriangles;






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
        //-----------------------//


    std::cout << "Rendering image...\n\n";

    //--------------------RENDERING LOOP--------------------//
    int pixelIndex = 0;

    for (size_t i = 0; i < Camera::x_res; i++) {

        for (size_t j = 0; j < Camera::y_res; j++) {
            Ray aRay(theEye, theCamera.thePixels[pixelIndex].position-theEye);

            for (size_t l = 0; l < Triangle::theTriangles.size(); l++) {
                if (glm::dot(Triangle::theTriangles[l].normal(), aRay.direction) < 0 && Triangle::theTriangles[l].calculateIntersection(aRay)) {
                    //std::cout << "Intersection found!\n";
                    theCamera.thePixels[i * Camera::x_res + j].pixelColor = Triangle::theTriangles[l].Color; //Give color of rectangle or triangle to pixel
                }
            }

            for (size_t l = 0; l < Rectangle::theRectangles.size(); l++) {
                if (glm::dot(Rectangle::theRectangles[l].normal(), aRay.direction) < 0 && Rectangle::theRectangles[l].calculateIntersection(aRay)) {
                    //std::cout << "Intersection found!\n";
                    theCamera.thePixels[i * Camera::x_res + j].pixelColor = Rectangle::theRectangles[l].Color; //Give color of rectangle or triangle to pixel
                }
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
            int r = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.r * 230);
            int g = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.g * 230);
            int b = floor(theCamera.thePixels[i * Camera::x_res + j].pixelColor.b * 230);


            img << r << " " << g << " " << b << std::endl;
        }
    }

    system("explorer picture.ppm");
    
}


Pixel::Pixel()
{
}

