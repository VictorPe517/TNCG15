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
	double inclination = 0.0;
};

class LightSource;
#define _USE_MATH_DEFINES

class Ray {
public:
	Ray() = default;

	Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor) : startPosition{ start }, direction{ glm::normalize(dir) }, RayRadianceColor{ _RayColor = ColorDBL(1,1,1) } {    }

	void SetRayColor(ColorDBL theColor);
	ColorDBL GetRayColor();

	// Get the point of intersection between an object and a light-source
	glm::dvec3 getPointOfIntersection(const std::vector<Object*>& theObjects, const LightSource& theLight);
	glm::dvec3 getRefractedDirection(const glm::dvec3& intersection, const glm::dvec3& surfaceNormal, const Object& theObject, double n1, double n2);
	glm::dvec3 getReflectedDirection(glm::dvec3& surfaceNormal);



	glm::dvec3 localCartesianToWorldCartesian(const glm::dvec3& localDir, const glm::dvec3& surfaceNormal);
	glm::dvec3 hemisphericalToCartesian(const LocalDirection& dir);
	glm::dvec3 getRandomDirection(const glm::dvec3& surfaceNormal);
	double GetInclination(const glm::dvec3& surfaceNormal);

	LocalDirection WorldCartesianToHemispherical();
	LocalDirection WorldCartesianToHemispherical(glm::dvec3& direction);
	LocalDirection getRandomLocalDirection();

	ColorDBL CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects,LightSource& theLight,const int& hitIndex);
	void CalculateRayPath(const std::vector<Object*>& theObjects, const LightSource& theLight);
	double IsVisibleToPoint(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightPoint, const std::vector<Object*>& theObjects);

	double CalculateBRDF(glm::dvec3 thePoint, double direction, double inclination);

	bool DrawRandom();
	double DrawRandomNormalized();

	void CalculateRadianceFlow(std::vector<Object*>& theObjects, LightSource& theLight);

	void ToString();

	void DeallocateRaypath();

	~Ray();

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

	Object* startSurface = nullptr;
	Object* hitObject = nullptr;
	size_t hitIndex = -1;

	bool isInside = false;
};