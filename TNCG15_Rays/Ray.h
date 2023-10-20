#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "ColorDBL.h"
#include <iostream>
#include <cmath>
#include <numbers>
#include "Object.h"
#include "glm/gtx/string_cast.hpp"
#include "LightSource.h"

#define _USE_MATH_DEFINES

class Ray {
public:
    Ray() = default;

    Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance, int _bounces) : startPosition{ start }, direction{ glm::normalize(dir) }, RayColor{ _RayColor }, radiance{ _radiance }, bounces_left{_bounces} {    }
    
    static double maxE;
    
    int bounces_left = 5;
    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
    glm::dvec3 direction = glm::dvec3(0, 0, 0);
    Ray* prevRay = nullptr;
    Ray* nextRay = nullptr;
    //Polygon* Surface = nullptr;
    ColorDBL RayColor = ColorDBL(0.0,0.0,0.0);
    double radiance = 0.0;
    double importance = 0.0;
    double reflectivity = 1.0;

    int hitIndex = 0;

    ColorDBL calcIrradiance(glm::dvec3 x_normal, glm::dvec3 y_normal, double area, glm::dvec3 intersectionPoint, glm::dvec3 areaLightPoint) {
        glm::dvec3 y = areaLightPoint; //Random punkt på lampan
        glm::dvec3 x = intersectionPoint;  //Intersection from eye->object
        //glm::dvec3 x = intersectionPoint;

        glm::dvec3 d = x-y;
        double distance = glm::distance(x, y);

        double cos_omega_x = glm::dot(x_normal, d) / (distance);
        double cos_omega_y = -1.0 * glm::dot(y_normal, d) / (distance);

        double G = cos_omega_x * cos_omega_y / (distance*distance);

        //Ray shadowRay = Ray(areaLightPoint, intersectionPoint - areaLightPoint, RayColor, 1.0);

        double E = 3200 * G * area;

        //if (theSphere.getIntersection(shadowRay) != glm::dvec3(-9999, -9999, -9999)) {
        //     E = 0.0;
        //}

        //std::cout << "Irradiance value is: " << E << "\n\n";

        if (E > maxE) {
            maxE = E;
        }

        if (E < 0) {
            return ColorDBL(0, 0, 0);
        }
        else {
            return ColorDBL(E, E, E);
        }
        
    };

    void calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource* theLight, int iterationAmt);

    glm::dvec3 getPointOfIntersection(std::vector<Object*> theObjects) {
        glm::dvec3 intersection = glm::dvec3(-9999,-9999,-9999);
        double minLength = 9999999.0;

        RayColor = ColorDBL(0,0,0);

        for (size_t l = 0; l < theObjects.size(); l++) {
            //std::cout << "  l:" << l << " \n";

            if (glm::dot((*theObjects[l]).normal(*this), direction) < 0.0) {

                glm::dvec3 possibleIntersection = (theObjects[l])->getIntersection(*this);

                if (possibleIntersection != glm::dvec3(-9999, -9999, -9999) && minLength >= glm::length(this->startPosition - possibleIntersection)) { //We hit the closest object
                    minLength = glm::length(this->startPosition-possibleIntersection);
                    intersection = possibleIntersection;
                    hitIndex = l;

                    if (theObjects[l]->getMaterial().isMirror && bounces_left > 0) {
                        //Bounce ray
                        //std::cout << "  Hit Mirror!!\n";

                        Ray bouncedRay(intersection, (*this).getReflectedDirection((theObjects[l])->normal(*this)), RayColor, 1.0, bounces_left - 1);

                        //std::cout << glm::to_string((*this).getReflectedDirection(theObjects[l]->normal(*this))) << "\n";
                        //std::cout << "  Reflected dir: \n";

                        intersection = bouncedRay.getPointOfIntersection(theObjects);

                        RayColor = bouncedRay.RayColor*0.95;

                        return intersection;
                    }
                    else {
                        RayColor = (theObjects[l]->getColor());
                        
                        
                    }
                    //Light calculations
                    // 
                    // 
                    //std::cout << "  Hit! r:" << RayColor.r << ", g: " << RayColor.g << ", b: " << RayColor.b << "\n";
                }
            }
        }

        return intersection; //Return closest collision coordinates
    }

    glm::dvec3 getReflectedDirection(glm::dvec3 surfaceNormal) {
        glm::dvec3 dir;

        dir = direction - 2.0 * (glm::dot(direction, surfaceNormal) * surfaceNormal);
        
        return glm::normalize(dir);
    }

};

void Ray::calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource* theLight, int iterationAmt) {
    for (size_t iter = 0; iter < iterationAmt; iter++) {
        glm::dvec3 thePoint = (*theLight).getRandomPoint();
        Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 1, 1), (*theLight).radiance, 0);
        RayColor += newRay.calcIrradiance(((*Object::theObjects[hitIndex]).normal(newRay)), (*theLight).normal(), 6.0, hitPoint, thePoint) / iterationAmt; //Give color of rectangle or triangle to pixel
    }
}

