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


    glm::dvec3 getIntersection(Ray theRay) {
        glm::dvec3 vertex = v2;

        glm::dvec3 c1 = v3 - vertex;
        glm::dvec3 c2 = v1 - vertex;

        double t = glm::dot((this->v2 - theRay.startPosition), this->normal()) / (glm::dot(theRay.direction, this->normal()));

        glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

        double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
        double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));

        if (a <= 0.0 || b <= 1.0) {
            return(possibleIntersection);
        }
        else {
            return glm::dvec3(0, 0, 0); //If vector is empty then no intersection
        }
    };

    std::vector<Triangle> get_vec() {
        return theTriangles;
    }

    glm::dvec3 v1, v2, v3;
    ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
    static std::vector<Triangle> theTriangles;
};

