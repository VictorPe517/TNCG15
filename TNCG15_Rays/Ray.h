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

struct LocalDirection {
public:
    double azimuth = 0.0;
    double omega = 0.0;
};

class LightSource;
#define _USE_MATH_DEFINES

class Ray {
public:
    Ray() = default;

    Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance, int _bounces) : startPosition{start}, direction{glm::normalize(dir)}, RayColor{_RayColor = ColorDBL(1,1,1) }, radiance{_radiance = 50.0}, bounces_left{_bounces = 5} {    }
    
    ColorDBL calcIrradiance(glm::dvec3 x_normal, glm::dvec3 y_normal, double area, glm::dvec3 intersectionPoint, glm::dvec3 areaLightPoint, std::vector<Object*>& theObjects, LightSource theLight, bool debug, int theHitIndex);

    static double maxE;
    
    int bounces_left = 5;
    int bounce_mirrorToMirror = 5;
    int illuminBounces = 4;

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

    Object* hitObject = nullptr;

    int hitIndex = 0;

    void calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt);

    glm::dvec3 getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight);

    glm::dvec3 getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject);

    glm::dvec3 getReflectedDirection(glm::dvec3 surfaceNormal);

    LocalDirection getRandomDirection();

    glm::dvec3 localToWorldCoordinates(LocalDirection dir, glm::dvec3 worldpoint);

    LocalDirection worldToLocalCoordinates(glm::dvec3 dir, glm::dvec3 point);

    double calculateShadowRay(glm::dvec3& hitPoint, glm::dvec3& thePoint, std::vector<Object*>& theObjects, LightSource& theLight, bool debug);

};



