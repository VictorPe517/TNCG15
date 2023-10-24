#include "Cube.h"

Cube::Cube(Rectangle _r1, Rectangle _r2, Rectangle _r3, Rectangle _r4, Rectangle _r5, Rectangle _r6, ColorDBL _Color) {
	theRects.push_back(&_r1);
	theRects.push_back(&_r2);
	theRects.push_back(&_r3);
	theRects.push_back(&_r4);
	theRects.push_back(&_r5);
	theRects.push_back(&_r6);

	theMaterial.MatColor = _Color;
	theObjects.push_back(this);
	theCubes.push_back(*this);
}

glm::dvec3 Cube::getIntersection(Ray& theRay) {
	double minLength = 99999999;
	glm::dvec3 closest = glm::dvec3(-9999,-9999,-9999);

	for (Rectangle* theRectangle : theRects) {
		glm::dvec3 intersection = theRectangle->getIntersection(theRay);

		if (glm::length(intersection) < minLength) {
			minLength = glm::length(intersection);
			closest = intersection;
			latestHitRectangle = theRectangle;
		}
	}

	return closest;
}


glm::dvec3 Cube::normal(Ray& theRay) {
	glm::dvec3 intersection = getIntersection(theRay);

	return latestHitRectangle->normal(theRay);;
}


ColorDBL Cube::getColor() {
	return theMaterial.MatColor;
}

Material Cube::getMaterial() {
	return theMaterial;
}