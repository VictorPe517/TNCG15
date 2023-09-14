#pragma once
#include "glm/glm.hpp"
#include "ColorDBL.h"

class Pixel {
public:
    Pixel();

    Pixel(glm::dvec3 pos) : position{ pos } {}

    ColorDBL pixelColor = ColorDBL(0.0, 0.0, 0.0);
    glm::dvec3 position = glm::dvec3(0, 0, 0);

    void SetPosition(glm::dvec3 pos);
};

