#pragma once
#include "glm/glm.hpp"
#include "Polygon.h"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"

class Rectangle : public Polygon {
public:
	Rectangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, ColorDBL _Color = ColorDBL(1.0, 1.0, 1.0), bool addToLists = true) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{ _v4 }{
		theMaterial.MatColor = _Color;

		if (addToLists) theObjects.push_back(this);

		thePolygons.push_back(this);
		theRectangles.push_back(*this);

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

	Rectangle(glm::dvec3 center, glm::dvec3 size, ColorDBL _Color, bool addToLists = true) {
		v1 = center + glm::dvec3(-size.x, size.y, size.z);
		v2 = center + glm::dvec3(size.x, size.y, size.z);
		v3 = center + glm::dvec3(size.x, -size.y, -size.z);
		v4 = center + glm::dvec3(-size.x, -size.y, -size.z);

		length.x = std::abs(std::max({ v1.x, v2.x, v3.x, v4.x }) - std::min({ v1.x, v2.x, v3.x, v4.x }));
		length.y = std::abs(std::max({ v1.y, v2.y, v3.y, v4.y }) - std::min({ v1.y, v2.y, v3.y, v4.y }));
		length.z = std::abs(std::max({ v1.z, v2.z, v3.z, v4.z }) - std::min({ v1.z, v2.z, v3.z, v4.z }));
		
		centerPoint = center;

		theMaterial.MatColor = _Color;

		if (addToLists) theObjects.push_back(this);

		thePolygons.push_back(this);
		theRectangles.push_back(*this);
	}

	virtual glm::dvec3 CalculateNormal(const Ray& theRay) override;

	virtual glm::dvec3 GetIntersection(const Ray& theRay) override;

	virtual ColorDBL GetColor() override;

	virtual Material GetMaterial() override;

	virtual const glm::dvec3 GetLength() override;

	virtual const glm::dvec3 GetCenterPoint() override;

	glm::dvec3 v1, v2, v3, v4;
	glm::dvec3 length;
	glm::dvec3 centerPoint;
	//ColorDBL Color = ColorDBL(0, 0, 0);
	Material theMaterial = Material(1, 1, 1, false, ColorDBL(1, 1, 1));
	static std::vector<Rectangle> theRectangles;
};

