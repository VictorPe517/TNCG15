#include "Triangle.h"


Triangle::Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, ColorDBL _Color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 } {
	theMaterial.MatColor = _Color;
	theObjects.push_back(this);
	theTriangles.push_back(*this);
	thePolygons.push_back(this);

	double xMax, xMin, yMax, yMin, zMax, zMin;

	xMax = std::max({ v1.x, v2.x, v3.x});
	xMin = std::min({ v1.x, v2.x, v3.x});
	yMax = std::max({ v1.y, v2.y, v3.y});
	yMin = std::min({ v1.y, v2.y, v3.y});
	zMax = std::max({ v1.z, v2.z, v3.z});
	zMin = std::min({ v1.z, v2.z, v3.z});

	length.x = std::abs(xMax - xMin);
	length.y = std::abs(yMax - yMin);
	length.z = std::abs(zMax - zMin);

	centerPoint = glm::dvec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);
}

glm::dvec3 Triangle::CalculateNormal(const Ray& theRay) {
	glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
	//std::cout << "	Triangle normal is: " << glm::to_string(direction) << ", with length: " << glm::length(direction) << ", with midpoint at position: " << glm::to_string((v1+v2+v3)/3.0) << "\n\n";
	return direction / glm::length(direction);
}

glm::dvec3 Triangle::GetIntersection(const Ray& theRay) {
	glm::dvec3 e1 = v2 - v1;
	glm::dvec3 e2 = v3 - v1;
	glm::dvec3 h = glm::cross(theRay.direction, e2);
	double a = glm::dot(e1, h); // a = determinanten. 

	// Ray parallel till triangle
	if (a > doubThreshold && a < doubThreshold) {
		return glm::dvec3(NAN, NAN, NAN);
	}

	double f = 1.0 / a;
	glm::dvec3 s = theRay.startPosition - v1;
	double u = f * glm::dot(s, h);

	//Ray intersection utanför triangle 
	if (u < 0.0 || u > 1.0) {
		return glm::dvec3(NAN, NAN, NAN);
	}

	glm::dvec3 q = glm::cross(s, e1);
	double v = f * glm::dot(theRay.direction, q);

	// Utanför triangle 
	if (v < 0.0 || u + v > 1.0) {
		return glm::dvec3(NAN, NAN, NAN);
	}

	double t = f * glm::dot(e2, q);

	if (t > doubThreshold) {
		return theRay.startPosition + t * theRay.direction; // True ray intersection.
	}

	return glm::dvec3(NAN, NAN, NAN); // No intersection.
}

glm::dvec3 const Triangle::GetCenterPoint() {
	return centerPoint;
}

glm::dvec3 const Triangle::GetLength() {
	return length;
}

std::vector<Triangle> Triangle::get_vec() {
	return theTriangles;
}

ColorDBL Triangle::GetColor() {
	return theMaterial.MatColor;
}

Material Triangle::GetMaterial() {
	return theMaterial;
}