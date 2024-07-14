#include "Sphere.h"


Sphere::Sphere(glm::dvec3 _position, double rad, ColorDBL _Color) : position{ _position }, radius{ rad }{
	theMaterial.MatColor = _Color;
	theSpheres.push_back(this);
	theObjects.push_back(this);
}

glm::dvec3 Sphere::getIntersection(const Ray& theRay) {
	double arg;

	double c1 = glm::dot(theRay.direction, theRay.direction);
	double c2 = glm::dot(2.0 * theRay.direction, theRay.startPosition - position);
	double c3 = glm::length(theRay.startPosition - position) * glm::length(theRay.startPosition - position) - radius * radius;

	arg = c2 * c2 - (4.0 * c1 * c3);

	if (arg < 0.0) {
		return glm::dvec3(NAN, NAN, NAN);
	}

	if (arg == 0.0) {
		//one solution
		//std::cout << " Hit the edge \n";
		return theRay.startPosition + theRay.direction * (-c2 + glm::sqrt(arg)) / (2.0 * c1);
	}

	if (arg > 0) {
		//Two Solutions
		//std::cout << " Entered and exited sphere, calculating tmin \n";

		double t1 = (-c2 + glm::sqrt(arg)) / (2.0 * c1);
		double t2 = (-c2 - glm::sqrt(arg)) / (2.0 * c1);

		if (t1 < 0.000001) t1 = -1;
		if (t2 < 0.000001) t2 = -1;

		if (t1 < 0.0 && t2 < 0.0) {
			//std::cout << "Intersection is behind the origin!\n";
			return glm::dvec3(NAN, NAN, NAN);
		}

		if (t1 < 0.0) return theRay.startPosition + theRay.direction * t2;
		if (t2 < 0.0) return theRay.startPosition + theRay.direction * t1;

		if (t1 < t2) {
			//std::cout << "    Tmin = " << t1 << " \n";
			return theRay.startPosition + theRay.direction * t1;
		}
		else {
			//std::cout << "    Tmin = " << t2 << " \n";
			return theRay.startPosition + theRay.direction * t2;
		}

	}
	else {
		return glm::dvec3(NAN, NAN, NAN);
	}
}


glm::dvec3 Sphere::normal(const Ray& theRay) {

	glm::dvec3 intersection = getIntersection(theRay);

	if (intersection != glm::dvec3(NAN, NAN, NAN)) {
		return (glm::normalize(intersection - position));
	}
	else {
		return glm::dvec3(NAN, NAN, NAN);
	}
	//std::cout << "Calculated normal: " << glm::to_string(glm::normalize(intersection - position)) << "\n";


}

ColorDBL Sphere::getColor() {
	return theMaterial.MatColor;
}

Material Sphere::getMaterial() {
	return theMaterial;
}