#include "Rectangle.h"

glm::dvec3 Rectangle::normal(const Ray& theRay) {
	glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);

	if (direction == glm::dvec3(-9999, -9999, -9999))
	{
		std::cout << "Something is really wrong with rectangleNormal\n";
	}

	return direction / glm::length(direction);
}

glm::dvec3 Rectangle::getIntersection(const Ray& theRay) {
	glm::dvec3 vertex = v2;

	glm::dvec3 c1 = v3 - vertex;
	glm::dvec3 c2 = v1 - vertex;

	glm::dvec3 theNormal = this->normal(theRay);

	double t = glm::dot((vertex - theRay.startPosition), theNormal) / (glm::dot(theRay.direction, theNormal));

	if (t < 0.001) t = -1.0;
	if (t < 0) return glm::dvec3(-9999, -9999, -9999);

	glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

	double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
	double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));

	if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0) {
		return(possibleIntersection);
	}
	else {
		//std::cout << "    Non-intersecting rectangle met \n\n";
		return glm::dvec3(-9999, -9999, -9999); //If vector is empty then no intersection
	}
}

ColorDBL Rectangle::getColor() {
	return theMaterial.MatColor;
}

Material Rectangle::getMaterial() {
	return theMaterial;
}

