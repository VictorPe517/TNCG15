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

		double xMax, xMin, yMax, yMin, zMax, zMin;

		xMax = std::max({ v1.x, v2.x, v3.x, v4.x });
		xMin = std::min({ v1.x, v2.x, v3.x, v4.x });
		yMax = std::max({ v1.y, v2.y, v3.y, v4.y });
		yMin = std::min({ v1.y, v2.y, v3.y, v4.y });
		zMax = std::max({ v1.z, v2.z, v3.z, v4.z });
		zMin = std::min({ v1.z, v2.z, v3.z, v4.z });

		length.x = std::abs(xMax - xMin);
		length.y = std::abs(yMax - yMin);
		length.z = std::abs(zMax - zMin);

		centerPoint = glm::dvec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);
	}

	glm::dvec3 v1, v2, v3, v4;
	double Watt = 100.0;
	double area = 1.0;

	double calculateArea();

	virtual glm::dvec3 CalculateNormal(const Ray& theRay) override;

	glm::dvec3 const getRandomPoint();

	virtual glm::dvec3 GetIntersection(const Ray& theRay) override;

	virtual const glm::dvec3 GetLength() override;

	virtual const glm::dvec3 GetCenterPoint() override;

	virtual ColorDBL GetColor() override {
		return Color;
	}

	virtual Material GetMaterial() override {
		return Material(1, 1, 1, 0, ColorDBL(0, 1, 1));
	}

	void Translate(glm::dvec3 coordinates);

	ColorDBL Color = ColorDBL(1.0, 1.0, 1.0);

	static std::vector<LightSource*> theLightSources;

	glm::dvec3 length;
	glm::dvec3 centerPoint;
};

