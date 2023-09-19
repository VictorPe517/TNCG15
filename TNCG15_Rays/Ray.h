#pragma once
#include "glm/glm.hpp"

class Ray {
public:
    Ray(glm::dvec3 start, glm::dvec3 dir) : startPosition{ start }, direction{ dir } {}
    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
    glm::dvec3 direction = glm::dvec3(0, 0, 0);
    Ray* prevRay = nullptr;
    Ray* nextRay = nullptr;
    double radiance = 0;
    double importance = 0;
    double reflectivity = 1;
};

