#pragma once
#include "glm/glm.hpp"
#include "Polygon.h"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"

class Rectangle : public Polygon {
public:
    Rectangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, ColorDBL _Color, bool addToLists = true) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{ _v4 }{
        theMaterial.MatColor = _Color;

        if(addToLists) theObjects.push_back(this);

        thePolygons.push_back(this);
        theRectangles.push_back(*this);
    }

    virtual glm::dvec3 normal(Ray& theRay) override;

    virtual glm::dvec3 getIntersection(Ray& theRay) override;

    virtual ColorDBL getColor() override;

    virtual Material getMaterial() override;

    glm::dvec3 v1, v2, v3, v4;
    //ColorDBL Color = ColorDBL(0, 0, 0);
    Material theMaterial = Material(1,0,0,false,ColorDBL(0,0,0));
    static std::vector<Rectangle> theRectangles;
};

