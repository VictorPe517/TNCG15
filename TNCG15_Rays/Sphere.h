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
	Sphere(glm::dvec3 _position, double rad, ColorDBL _Color);

	virtual glm::dvec3 getIntersection(Ray& theRay) override;

	//virtual glm::dvec3 normal(Ray theRay) override{
	//	std::cout << "   normal() called for sphere - Invalid operation!";
	//	return glm::dvec3(-9999, -9999, -9999);
	//}

	virtual glm::dvec3 normal(Ray& theRay);

	virtual ColorDBL getColor() override;

	virtual Material getMaterial() override;

	void setSize(double newSize);

	void setPosition(glm::dvec3 pos);

	glm::dvec3 position = glm::dvec3(0, 0, 0);
	double radius = 0;
	//ColorDBL Color = ColorDBL(0,0,0);

	Material theMaterial = Material(1, 0, 1, false, ColorDBL(1, 1, 1));

	static std::vector<Sphere*> theSpheres;
};

