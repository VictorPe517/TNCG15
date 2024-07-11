#pragma once

#define _USE_MATH_DEFINES

#include "glm/glm.hpp"
#include "ColorDBL.h"
#include <cmath>
#include <numbers>
#include <iostream>
#include <stdlib.h>
#include "glm/gtx/string_cast.hpp"
#include "Object.h"
#include "Ray.h"
#include "Material.h"
#include <cassert>

class Ray;

class LightSource : public Object
{
public:
	LightSource(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, double _watt, ColorDBL _color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{ _v4 }, Watt{ _watt }, Color{ _color } {
		area = calculateArea();
		theObjects.push_back(this);
		theLightSources.push_back(this);
	}

	glm::dvec3 v1, v2, v3, v4;
	double Watt = 100.0;
	double area = 1.0;
	double radiosity = 10000;
	double radiance = radiosity;

	double calculateArea();

	virtual glm::dvec3 normal(Ray& theRay) override;

	glm::dvec3 getRandomPoint();

	virtual glm::dvec3 getIntersection(Ray& theRay) override;

	virtual ColorDBL getColor() override {
		return Color;
	}

	virtual Material getMaterial() override {
		return Material(1, 1, 1, 0, ColorDBL(0, 1, 1));
	}

	void Translate(glm::dvec3 coordinates);

	ColorDBL Color = ColorDBL(1.0, 1.0, 1.0);

	static std::vector<LightSource*> theLightSources;
};

