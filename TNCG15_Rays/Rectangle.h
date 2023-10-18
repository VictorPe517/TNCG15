#pragma once
#include "glm/glm.hpp"
#include "Polygon.h"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"

class Rectangle : public Polygon {
public:
    Rectangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{ _v4 }, Color{ _Color } {
        thePolygons.push_back(this);
        theRectangles.push_back(*this);
    }

    virtual glm::dvec3 normal() override {
        glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
        
        return direction / glm::length(direction);
    }

    virtual glm::dvec3 getIntersection(Ray theRay) override {
        glm::dvec3 vertex = v2;

        glm::dvec3 c1 = v3 - vertex;
        glm::dvec3 c2 = v1 - vertex;

        double t = glm::dot((vertex - theRay.startPosition), this->normal()) / (glm::dot(theRay.direction, this->normal()));

        if (t > 25.0) {
            return glm::dvec3(-9999, -9999, -9999);
        }

        glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

        double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
        double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));

        if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0) {
            return(possibleIntersection);
        }
        else {
            //std::cout << "    Non-intersecting rectangle met \n\n";
            return glm::dvec3(-9999,-9999,-9999); //If vector is empty then no intersection
        }
    };

    virtual ColorDBL getColor() override {
        return Color;
    }

    glm::dvec3 v1, v2, v3, v4;
    ColorDBL Color = ColorDBL(0, 0, 0);
    static std::vector<Rectangle> theRectangles;
};

