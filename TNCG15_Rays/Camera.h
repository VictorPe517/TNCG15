#pragma once
#include "glm/glm.hpp"
#include "Pixel.h"
#include <vector>

class Camera {
public:
    Camera(glm::dvec3 _c1, glm::dvec3 _c2, glm::dvec3 _c3, glm::dvec3 _c4) : c1{ _c1 }, c2{ _c2 }, c3{ _c3 }, c4{ _c4 } {
        double delta = 2.0 / x_res;
        //We have our camera position from the above corners, !!!  X = 0  !!!
        //We need to set the position of each pixel in the camera to make sense
        for (size_t i = 0; i < x_res; i++) {
            for (size_t j = 0; j < y_res; j++) {
                thePixels.push_back(Pixel(glm::dvec3(0.0, -1.0 * (j * delta - (1.0f - delta)), -1.0 * (i * delta - (1.0f - delta))))); //TODO: wtf multiplicera in minustecken
            }
        }
    }

    static const int x_res = 200;   //Static:   Shared between all instances of Camera
    static const int y_res = 200;   //Const:    We pinky promise to the compiler that we won't change the resolution of the camera during runtime.

    glm::dvec3 c1, c2, c3, c4;
    std::vector<Pixel> thePixels;
};

