#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Ray.h"

class Polygon {
public:
    Polygon() = default;

    virtual glm::dvec3 normal() = 0;

    virtual ColorDBL getColor() = 0;

    virtual glm::dvec3 getIntersection(Ray theRay) = 0;

    static std::vector<Polygon*> thePolygons;
};

