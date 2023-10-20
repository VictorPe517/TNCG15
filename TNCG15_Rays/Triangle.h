#pragma once
#include "Polygon.h"
#include "glm/glm.hpp"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"
#include "Object.h"

class Triangle : public Polygon {
public:
    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 } {
        theMaterial.MatColor = _Color;
        theObjects.push_back(this);
        theTriangles.push_back(*this);
        thePolygons.push_back(this);
    }

    virtual glm::dvec3 normal(Ray& theRay) override {
        glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
        //std::cout << "	Triangle normal is: " << glm::to_string(direction) << ", with length: " << glm::length(direction) << ", with midpoint at position: " << glm::to_string((v1+v2+v3)/3.0) << "\n\n";
        return direction / glm::length(direction);
    }

    virtual glm::dvec3 getIntersection(Ray theRay) override {
        glm::dvec3 e1 = v2 - v1;
        glm::dvec3 e2 = v3 - v1;
        glm::dvec3 h = glm::cross(theRay.direction, e2); 
        double a = glm::dot(e1, h); // a = determinanten. 

        // Ray parallel till triangle
        if (a > doubThreshold && a < doubThreshold) {
            return glm::dvec3(-9999, -9999, -9999); 
        }

        double f = 1.0 / a;
        glm::dvec3 s = theRay.startPosition - v1;
        double u = f * glm::dot(s, h);

        //Ray intersection utanför triangle 
        if (u < 0.0 || u > 1.0) {
            return glm::dvec3(-9999, -9999, -9999); 
        }

        glm::dvec3 q = glm::cross(s, e1);
        double v = f * glm::dot(theRay.direction, q);

        // Utanför triangle 
        if (v < 0.0 || u + v > 1.0) {
            return glm::dvec3(-9999, -9999, -9999); 
        }

        double t = f * glm::dot(e2, q);

        if (t > doubThreshold) {
            return theRay.startPosition + t * theRay.direction; // True ray intersection.
        }

        return glm::dvec3(-9999, -9999, -9999); // No intersection.
    }

    std::vector<Triangle> get_vec() {
        return theTriangles;
    }

    virtual ColorDBL getColor() override {
        return theMaterial.MatColor;
    }

    virtual Material getMaterial() override {
        return theMaterial;
    }

    glm::dvec3 v1, v2, v3;
    ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
    static std::vector<Triangle> theTriangles;
    Material theMaterial = Material(1, 0, 1, false, ColorDBL(1, 1, 1));

private:
    double doubThreshold = 1e-6;
};

