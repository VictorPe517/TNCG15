#pragma once
#include "Polygon.h"
#include "glm/glm.hpp"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"
#include "Object.h"

class Triangle : public Polygon {
public:
	Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color);

	virtual glm::dvec3 CalculateNormal(const Ray& theRay) override;

	virtual glm::dvec3 GetIntersection(const Ray& theRay) override;

	std::vector<Triangle> get_vec();

	virtual ColorDBL GetColor() override;

	virtual Material GetMaterial() override;

	virtual const glm::dvec3 GetLength() override;

	virtual const glm::dvec3 GetCenterPoint() override;

	glm::dvec3 v1, v2, v3;
	ColorDBL Color = ColorDBL(0.0, 0.0, 0.0);
	static std::vector<Triangle> theTriangles;
	Material theMaterial = Material(1.0, 1.0, 1.0, false, ColorDBL(1.0, 1.0, 1.0));

	glm::dvec3 length;
	glm::dvec3 centerPoint;

private:
	double doubThreshold = 1e-8;
};

