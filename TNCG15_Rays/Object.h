#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Material.h"

class Ray;

class Object
{
public:
	Object() = default;

	virtual glm::dvec3 CalculateNormal(const Ray& theRay) = 0;

	virtual ColorDBL GetColor() = 0;

	virtual glm::dvec3 GetIntersection(const Ray& theRay) = 0;

	virtual Material GetMaterial() = 0;

	virtual const glm::dvec3 GetLength() = 0;

	virtual const glm::dvec3 GetCenterPoint() = 0;

	static std::vector<Object*> theObjects;
};

