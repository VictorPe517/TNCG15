#pragma once
#include "Polygon.h"
#include "glm/glm.hpp"
#include "ColorDBL.h"
#include "Ray.h"

class Triangle : public Polygon {
public:
    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, Color{ _Color } {
        theTriangles.push_back(*this);
        thePolygons.push_back(*this);
    }

    glm::dvec3 normal() {
        glm::dvec3 direction = glm::cross(v2 - v1, v3 - v1);
        return direction / glm::length(direction);
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

        return(a <= 0.0 || b <= 1.0);
    };

    std::vector<Triangle> get_vec() {
        return theTriangles;
    }

    glm::dvec3 v1, v2, v3;
    ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
    static std::vector<Triangle> theTriangles;
};

