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


class LightSource;
#define _USE_MATH_DEFINES

class Ray {
public:
    Ray() = default;

    Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance, int _bounces) : startPosition{ start }, direction{ glm::normalize(dir) }, RayColor{ _RayColor }, radiance{ _radiance }, bounces_left{_bounces} {    }
    
    ColorDBL calcIrradiance(glm::dvec3 x_normal, glm::dvec3 y_normal, double area, glm::dvec3 intersectionPoint, glm::dvec3 areaLightPoint);

    static double maxE;
    
    int bounces_left = 5;

    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);

    glm::dvec3 direction = glm::dvec3(0, 0, 0);


    Ray* prevRay = nullptr;
    Ray* nextRay = nullptr;

    ColorDBL RayColor = ColorDBL(0.0,0.0,0.0);
    double radiance = 0.0;
    double importance = 0.0;
    double reflectivity = 1.0;

    bool do_not_reflect = false;
    bool shadowCalculated = false;

    Object* hitObject;

    bool debug = true;

    int hitIndex = 0;

    void calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt);

    glm::dvec3 getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight);

    glm::dvec3 getReflectedDirection(glm::dvec3 surfaceNormal);

    double calculateShadowRay(glm::dvec3 hitPoint, glm::dvec3 thePoint, std::vector<Object*> theObjects, LightSource& theLight);

};



