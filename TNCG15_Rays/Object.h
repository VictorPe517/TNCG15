#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Material.h"

class Ray;

class Object
{
public:
	Object() = default;

	virtual glm::dvec3 normal(const Ray& theRay) = 0;

	virtual ColorDBL getColor() = 0;

	virtual glm::dvec3 getIntersection(const Ray& theRay) = 0;

	virtual Material getMaterial() = 0;

	static std::vector<Object*> theObjects;
};

