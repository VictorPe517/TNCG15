// TNCG15_Rays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// INHERITANCE: https://msbrijuniversity.ac.in/assets/uploads/newsupdate/InheritanceinC++.pdf

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

class Pixel {
public:
    glm::dvec3 Color;
    glm::dvec3 position;
};

class Camera {
public:
    Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4) : c1{ _c1 }, c2{ _c2 }, c3{ _c3 }, c4{ _c4 }{
        double delta = 0.0025;
        //We have our camera position from the above corners, !!!  X = 0  !!!
        //We need to set the position of each pixel in the camera to make sense
        for (int i = 0; i < 800; i++) {
            for (int j = 0; j < 800; j++) {
                allPixels[i][j].position = glm::dvec3(0,-1+i*delta,-1+j*delta);
            }
        }

    }

    static const int x_res = 800;   //Static:   Shared between all instances of Camera
    static const int y_res = 800;   //Const:    We pinky promise to the compiler that we won't change the resolution of the camera during runtime.

    glm::dvec3 c1, c2, c3, c4;
    Pixel allPixels[x_res][y_res];
};

class Polygon {
public:
    glm::dvec3 Color;
};

class Triangle : public Polygon{
public:
    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _Color) : v1{_v1}, v2{_v2}, v3{_v3}, Color{_Color} {}

    glm::dvec3 normal() {
        glm::dvec3 direction = glm::cross(glm::normalize(v2 - v1), glm::normalize(v3 - v1));
        std::cout << glm::to_string(direction) << ",    with length: " << glm::length(direction) << "\n";
        return direction/glm::length(direction);
    }

    glm::dvec3 v1, v2, v3, Color;
};

class Rectangle: public Polygon {
public:
    Rectangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, glm::dvec3 _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{_v4}, Color{_Color}{}

    glm::dvec3 normal() {
        glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
        std::cout << glm::to_string(direction) << ",    with length: " << glm::length(direction) << "\n";
        return direction / glm::length(direction);
    }

    glm::dvec3 v1, v2, v3, v4, Color;
};


//------------------------------------------------------------------------------------------S-------------//
int main()
{
    std::cout << "Welcome to Global lets do this!!! \n";

    glm::dvec3 theEye(-1.0, 0.0, 0.0);

    glm::dvec3 red(1.0, 0.0, 0.0);
    glm::dvec3 green(0.0, 1.0, 0.0);
    glm::dvec3 blue(0.0, 0.0, 1.0);

    //------GEOMETRY------//
        //------CIELING------//
    Rectangle cielingRect(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), red);
    Triangle cielingTri1(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), red);
    Triangle cielingTri2(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), red);
        
        //-------FLOOR-------//
    Rectangle floorRect(glm::dvec3(10, 6, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), blue);
    Triangle floorTri1(glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), glm::dvec3(0, -6, -5), blue);
    Triangle floorTri2(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), blue);
        
        //-------WALLS-------//
    Rectangle wallN(glm::dvec3(10, 6, 5), glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), green);
    Rectangle wallNW(glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), green);
    Rectangle wallNE(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), green);
    Rectangle wallS(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), green);
    Rectangle wallSW(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), green);
    Rectangle wallSE(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), green);

    Camera theCamera(glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(0, 1, 1), glm::dvec3(0, -1, 1));
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
