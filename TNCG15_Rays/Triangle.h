#pragma once
#include "Polygon.h"
#include "glm/glm.hpp"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"

class Triangle : public Polygon {
public:
    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, Color{ _Color } {
        theTriangles.push_back(*this);
        thePolygons.push_back(this);
    }

    virtual glm::dvec3 normal() override {
        glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
        //std::cout << "	Triangle normal is: " << glm::to_string(direction) << ", with length: " << glm::length(direction) << ", with midpoint at position: " << glm::to_string((v1+v2+v3)/3.0) << "\n\n";
        return direction / glm::length(direction);
    }

    virtual glm::dvec3 getIntersection(Ray theRay) override {
        glm::dvec3 vertex = v3;

        glm::dvec3 c1 = v1-vertex;
        glm::dvec3 c2 = v2-vertex;

        //std::cout << glm::to_string(this->normal()) << std::endl;

        double t = glm::dot((vertex - theRay.startPosition), glm::normalize(glm::cross(c2,c1))) / (glm::dot((theRay.direction),glm::normalize(glm::cross(c2, c1))));
        
        glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

        double a = glm::dot((possibleIntersection - vertex), c1) / (glm::dot(c1, c1));
        double b = glm::dot((possibleIntersection - vertex), c2) / (glm::dot(c2, c2));

        //std::cout << glm::to_string(this->normal()) << std::endl;
        //std::cout << "     " << glm::to_string(v1) << ", " << glm::to_string(v2) << ", " << glm::to_string(v3) << std::endl;

        if (a > 0.0 && b > 0.0 && (a + b) < 1.0) {
            //std::cout << "      a = " << a << ", b = " << b << std::endl;
            //std::cout << "         " << glm::to_string(possibleIntersection) << std::endl;

            return(possibleIntersection);
        }
        else {
            //std::cout << "    Non-intersecting triangle met \n\n";
            return glm::dvec3(-9999, -9999, -9999); //If vector is 999,999,999 then no intersection
        }
    };

    std::vector<Triangle> get_vec() {
        return theTriangles;
    }

    glm::dvec3 v1, v2, v3;
    ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
    static std::vector<Triangle> theTriangles;
};

