#pragma once
#include "glm/glm.hpp"
#include "ColorDBL.h"

class Ray {
public:
    Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor) : startPosition{ start }, direction{ dir }, RayColor { _RayColor } {}

   
    
    
    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
    glm::dvec3 direction = glm::dvec3(0, 0, 0);
    Ray* prevRay = nullptr;
    Ray* nextRay = nullptr;
    Polygon* Surface = nullptr;
    ColorDBL RayColor;
    double radiance = 0;
    double importance = 0;
    double reflectivity = 1;

};

