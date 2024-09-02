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
#include <Random>

struct LocalDirection {
public:
	double azimuth = 0.0;
	double inclination = 0.0;
};

class LightSource;
#define _USE_MATH_DEFINES

class Ray {
public:
	Ray() = default;

	Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance, int _bounces) : startPosition{ start }, direction{ glm::normalize(dir) }, RayRadianceColor{ _RayColor = ColorDBL(1,1,1) }, L{ _radiance = 50.0 }, bounces_left{ _bounces = 5 } {    }

	void SetRayColor(ColorDBL theColor);
	ColorDBL GetRayColor();

	bool DrawRandom();
	// Get the point of intersection between an object and a light-source
	glm::dvec3 getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight, int iterations);
	glm::dvec3 getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject, double n1, double n2);
	glm::dvec3 getReflectedDirection(glm::dvec3 surfaceNormal);

	glm::dvec3 localCartesianToWorldCartesian(glm::dvec3 localDir, glm::dvec3 surfaceNormal);
	glm::dvec3 hemisphericalToCartesian(LocalDirection dir);
	glm::dvec3 getRandomDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal);
	double GetInclination(glm::dvec3 surfaceNormal);

	LocalDirection WorldCartesianToHemispherical();
	LocalDirection getRandomLocalDirection();

	double CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, LightSource& theLight);
	void CalculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt);
	ColorDBL CalculateFinalIrradiance(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt);
	double IsVisibleToPoint(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightPoint, const std::vector<Object*>& theObjects);

	double CalculateBRDF(glm::dvec3 thePoint, double direction, double inclination);

	glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
	glm::dvec3 endPosition = glm::dvec3(0, 0, 0);
	glm::dvec3 direction = glm::dvec3(0, 0, 0);

	Ray* prevRay = nullptr;
	Ray* nextRay = nullptr;

private:
	ColorDBL RayRadianceColor = ColorDBL(0.0, 0.0, 0.0);

	// W: Importance
	double W = 1.0;

	// L: Radiance
	double L = 0.0;

	// E: Irradiance, the light arriving from the scene to the point
	double Irradiance = 0.0;


	int bounces_left = 5;

	Object* startSurface = nullptr;
	Object* hitObject = nullptr;
	size_t hitIndex = 0;

	bool isInside = false;
};



