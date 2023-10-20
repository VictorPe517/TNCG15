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
	Sphere(glm::dvec3 _position, double rad, ColorDBL _Color) : position{ _position }, radius{ rad }{
		theMaterial.MatColor = _Color;
		theSpheres.push_back(this);
		theObjects.push_back(this);
	}

	virtual glm::dvec3 getIntersection(Ray theRay) override{
		double arg;

		double c1 = glm::dot(theRay.direction,theRay.direction);
		double c2 = 2 * glm::dot(theRay.direction , (theRay.startPosition - position));
		double c3 = glm::dot((theRay.startPosition - position), (theRay.startPosition - position)) - radius * radius;

		arg = c2 * c2 - 4 * c1 * c3;

		if (arg < 0) {
			return glm::dvec3(-9999, -9999, -9999);
		}
		
		if (arg == 0) {
			//one solution
			//std::cout << " Hit the edge \n";
			return theRay.startPosition + theRay.direction * (-1.0 * c2 + glm::sqrt(arg)) / (2 * c1);
		}
		
		if (arg > 0) {
			//Two Solutions
			//std::cout << " Entered and exited sphere, calculating tmin \n";

			double t1 = (-1.0*c2 + glm::sqrt(arg))/(2*c1);
			double t2 = (-1.0 * c2 - glm::sqrt(arg)) / (2 * c1);

			if (t1 < t2) {
				//std::cout << "    Tmin = " << t1 << " \n";
				return theRay.startPosition + theRay.direction*t1;
			}
			else {
				return theRay.startPosition + theRay.direction * t2;
			}

		}
		else {
			return glm::dvec3(-9999, -9999, -9999);
		}
	}

	//virtual glm::dvec3 normal(Ray theRay) override{
	//	std::cout << "   normal() called for sphere - Invalid operation!";
	//	return glm::dvec3(-9999, -9999, -9999);
	//}

	virtual glm::dvec3 normal(Ray& theRay) {
		double arg;

		double c1 = glm::dot(theRay.direction, theRay.direction);
		double c2 = 2 * glm::dot(theRay.direction, (theRay.startPosition - position));
		double c3 = glm::dot((theRay.startPosition - position), (theRay.startPosition - position)) - radius * radius;

		arg = c2 * c2 - 4 * c1 * c3;

		if (arg < 0) {
			//std::cout << " Missed the sphere \n";
			return glm::dvec3(-9999, -9999, -9999);
		}
		
		if (arg == 0) {
			//one solution
			//std::cout << " NormalCheck: Hit the edge \n";
			double t1 = (-1.0 * c2 + glm::sqrt(arg)) / (2 * c1);
			return glm::normalize((theRay.startPosition + theRay.direction * t1) - position);
		}
		
		if (arg > 0) {
			//Two Solutions
			//std::cout << " NormalCheck: Entered and exited sphere, calculating tmin \n";

			double t1 = (-1.0 * c2 + glm::sqrt(arg)) / (2 * c1);
			double t2 = (-1.0 * c2 - glm::sqrt(arg)) / (2 * c1);

			if (t1 < t2) {
				std::cout << " First intersection on sphere found!\n";
				//std::cout << "   The normal is: " << glm::to_string(glm::normalize((theRay.startPosition + theRay.direction * t1) - position)) << "\n";
				return glm::normalize((theRay.startPosition + theRay.direction * t1) - position);
			}
			else {
				//std::cout << "   The normal is: " << glm::to_string(glm::normalize((theRay.startPosition + theRay.direction * t2) - position)) << "\n";
				return glm::normalize((theRay.startPosition + theRay.direction * t2) - position);
			}

		}
		else {
			return glm::dvec3(-9999, -9999, -9999);
		}
	}

	virtual ColorDBL getColor() override{
		return theMaterial.MatColor;
	}

	virtual Material getMaterial() override{
		return theMaterial;
	}

	glm::dvec3 position = glm::dvec3(0, 0, 0);
	double radius = 0;
	//ColorDBL Color = ColorDBL(0,0,0);

	Material theMaterial = Material(1, 0, 1, true, ColorDBL(1, 1, 1));

	static std::vector<Sphere*> theSpheres;
};
