#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Ray.h"
#include "Material.h"
#include "Object.h"

class Polygon : public Object{
public:
    Polygon() = default;

    //virtual glm::dvec3 normal(Ray theRay) = 0;

    //virtual ColorDBL getColor() = 0;

    //virtual glm::dvec3 getIntersection(Ray theRay) = 0;

    //virtual Material getMaterial() = 0;

    static std::vector<Polygon*> thePolygons;
};

