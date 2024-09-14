#pragma once
#include "glm/glm.hpp"
#include "Polygon.h"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"
#include "glm/gtx/component_wise.hpp"

class Sphere : public Object
{
public:
	Sphere(glm::dvec3 _position, double rad, ColorDBL _Color = ColorDBL(1.0, 1.0, 1.0));

	virtual glm::dvec3 GetIntersection(const Ray& theRay) override;

	//virtual glm::dvec3 normal(Ray theRay) override{
	//	std::cout << "   normal() called for sphere - Invalid operation!";
	//	return glm::dvec3(0.0, 0.0, 0.0);
	//}

	virtual glm::dvec3 CalculateNormal(const Ray& theRay);

	virtual ColorDBL GetColor() override;

	virtual Material GetMaterial() override;

	virtual const glm::dvec3 GetLength() override;

	virtual const glm::dvec3 GetCenterPoint() override;

	glm::dvec3 position = glm::dvec3(0, 0, 0);
	double radius = 0;
	//ColorDBL Color = ColorDBL(0,0,0);

	Material theMaterial = Material(1, 1, 1, false, ColorDBL(1.0, 1.0, 1.0));

	static std::vector<Sphere*> theSpheres;

	glm::dvec3 length;
	glm::dvec3 centerPoint;
};

