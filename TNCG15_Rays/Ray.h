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

	Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance, int _bounces) : startPosition{ start }, direction{ glm::normalize(dir) }, RayColor{ _RayColor = ColorDBL(1,1,1) }, radiance{ _radiance = 50.0 }, bounces_left{ _bounces = 5 } {    }

	// Get the point of intersection between an object and a light-source
	glm::dvec3 getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight, int iterations);

	glm::dvec3 getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject);

	glm::dvec3 getReflectedDirection(glm::dvec3 surfaceNormal);

	glm::dvec3 localCartesianToWorldCartesian(glm::dvec3 localDir, glm::dvec3 surfaceNormal);

	glm::dvec3 hemisphericalToLocalCartesian(LocalDirection dir);

	glm::dvec3 getRandomDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal);

	LocalDirection getRandomLocalDirection();

	double calcIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, LightSource& theLight);

	void calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt);

	double calculateShadowRay(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightPoint, const std::vector<Object*>& theObjects);

	int bounces_left = 5;

	glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
	glm::dvec3 direction = glm::dvec3(0, 0, 0);

	Ray* prevRay = nullptr;
	Ray* nextRay = nullptr;
	ColorDBL RayColor = ColorDBL(0.0, 0.0, 0.0);

	double radiance = 0.0;
	double importance = 0.0;
	double reflectivity = 1.0;
	double irradiance = 0.0;

	bool isInside = false;
	bool do_not_reflect = false;
	bool shadowCalculated = false;

	Object* hitObject = nullptr;

	size_t hitIndex = 0;

};



