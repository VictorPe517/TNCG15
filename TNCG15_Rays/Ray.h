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

	Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor) : startPosition{ start }, direction{ glm::normalize(dir) }, RayRadianceColor{ _RayColor = ColorDBL(1,1,1) } {
		rayAmount++;
	}

	void SetRayColor(ColorDBL theColor);
	ColorDBL GetRayColor();

	// Get the point of intersection between an object and a light-source
	glm::dvec3 getPointOfIntersection(const std::vector<Object*>& theObjects, const LightSource& theLight);
	glm::dvec3 GetRefractedDirection(const glm::dvec3& intersection, const glm::dvec3& surfaceNormal, const Object& theObject, double ior);
	glm::dvec3 GetReflectedDirection(const glm::dvec3& surfaceNormal);

	glm::dvec3 LocalCartesianToWorldCartesian(const glm::dvec3& localDir, const glm::dvec3& surfaceNormal);
	glm::dvec3 HemisphericalToCartesian(const LocalDirection& dir);
	glm::dvec3 getRandomDirection(const glm::dvec3& surfaceNormal);
	double GetInclination(const glm::dvec3& surfaceNormal);

	LocalDirection WorldCartesianToHemispherical();
	LocalDirection WorldCartesianToHemispherical(glm::dvec3& direction);
	LocalDirection GetRandomLocalDirection();

	ColorDBL CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLight,const int& hitIndex);
	void CalculateRayPath(const std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLights);
	double IsVisibleToPoint(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightPoint, const std::vector<Object*>& theObjects, const LightSource* theLight);
	
	bool BoundingBoxesIntersect(const glm::dvec3& rayCenter, const glm::dvec3& rayLength, const glm::dvec3& objectCenter, const glm::dvec3& objectLength);
	glm::dvec3 CreateLength(const glm::dvec3& minPoint, const glm::dvec3& maxPoint);
	glm::dvec3 CreateCenterPoint(const glm::dvec3& minPoint, const glm::dvec3& maxPoint);

	double CalculateBRDF(glm::dvec3 thePoint, double direction, double inclination);

	bool DrawRandom();
	double DrawRandomNormalized();

	void CalculateRadianceFlow(std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLights);

	void ToString();

	void DeallocateRaypath();

	std::string GetAllNonHittersFormatted();

	~Ray();

	glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
	glm::dvec3 endPosition = glm::dvec3(0, 0, 0);
	glm::dvec3 direction = glm::dvec3(0, 0, 0);

	Ray* prevRay = nullptr;
	Ray* nextRay = nullptr;
	bool isInside = false;

	static unsigned int nonHitters;
	static unsigned int rayAmount;

private:
	ColorDBL RayRadianceColor = ColorDBL(0.0, 0.0, 0.0);
	Object* startSurface = nullptr;
	Object* hitObject = nullptr;
	size_t hitIndex = -1;
};