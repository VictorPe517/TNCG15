#include "Cube.h"

Cube::Cube(glm::dvec3 midpoint, double sideLength, ColorDBL _Color)
{
	double l = sideLength / 2.0;
	//Define 8 vertices that all the rects will share
	glm::dvec3 v1, v2, v3, v4, v5, v6, v7, v8;

	v1 = glm::dvec3(midpoint.x + l, midpoint.y + l, midpoint.z + l);
	v2 = glm::dvec3(midpoint.x - l, midpoint.y + l, midpoint.z + l);
	v3 = glm::dvec3(midpoint.x + l, midpoint.y - l, midpoint.z + l);
	v4 = glm::dvec3(midpoint.x + l, midpoint.y + l, midpoint.z - l);
	v5 = glm::dvec3(midpoint.x - l, midpoint.y - l, midpoint.z + l);
	v6 = glm::dvec3(midpoint.x - l, midpoint.y + l, midpoint.z - l);
	v7 = glm::dvec3(midpoint.x + l, midpoint.y - l, midpoint.z - l);
	v8 = glm::dvec3(midpoint.x - l, midpoint.y - l, midpoint.z - l);

	theRects.push_back(new Rectangle(v1, v2, v5, v3, _Color, false)); //Up
	theRects.push_back(new Rectangle(v1, v4, v6, v2, _Color, false)); //Right
	theRects.push_back(new Rectangle(v1, v3, v7, v4, _Color, false)); //Forward
	theRects.push_back(new Rectangle(v3, v5, v8, v7, _Color, false)); //Left
	theRects.push_back(new Rectangle(v5, v2, v6, v8, _Color, false)); //Back
	theRects.push_back(new Rectangle(v7, v8, v6, v4, _Color, false)); //Down

	theMaterial.MatColor = _Color;

	theObjects.push_back(this);
	theCubes.push_back(*this);
}

Cube::Cube(Rectangle _r1, Rectangle _r2, Rectangle _r3, Rectangle _r4, Rectangle _r5, Rectangle _r6, ColorDBL _Color) {
	theRects.push_back(&_r1);
	theRects.push_back(&_r2);
	theRects.push_back(&_r3);
	theRects.push_back(&_r4);
	theRects.push_back(&_r5);
	theRects.push_back(&_r6);

	theMaterial.MatColor = _Color;
	//theObjects.push_back(this);
	theCubes.push_back(*this);
}

glm::dvec3 Cube::GetIntersection(const Ray& theRay) {
	double minLength = (double)INFINITY;
	glm::dvec3 closest = glm::dvec3(NAN, NAN, NAN);

	for (Rectangle* theRectangle : theRects) {
		glm::dvec3 intersection = theRectangle->GetIntersection(theRay);

		if (!glm::all(glm::isnan(intersection)) && glm::length(intersection - theRay.startPosition) < minLength) {
			minLength = glm::length(intersection - theRay.startPosition);
			closest = intersection;
		}
	}
	//if (minLength != glm::length(glm::dvec3(0.0, 0.0, 0.0)))std::cout << "Minlength: " << minLength << "\n";

	return closest;
}


glm::dvec3 Cube::CalculateNormal(const Ray& theRay) {
	Rectangle* latestHitRectangle = nullptr;

	double minLength = (double)INFINITY;
	glm::dvec3 closest = glm::dvec3(NAN, NAN, NAN);

	for (Rectangle* theRectangle : theRects) {
		glm::dvec3 intersection = theRectangle->GetIntersection(theRay);

		if (intersection != glm::dvec3(NAN, NAN, NAN) && glm::length(intersection - theRay.startPosition) < minLength) {
			minLength = glm::length(intersection - theRay.startPosition);
			closest = intersection;
			latestHitRectangle = theRectangle;
		}
	}
	//std::cout << "Normal: " << glm::to_string(latestHitRectangle->normal(theRay)) << "\n";
	if (latestHitRectangle != nullptr) return latestHitRectangle->CalculateNormal(theRay);

	return glm::dvec3(NAN, NAN, NAN);
}


ColorDBL Cube::GetColor() {
	return theMaterial.MatColor;
}

Material Cube::GetMaterial() {
	return theMaterial;
}

void Cube::setMirror(bool boolean)
{
	theMaterial.isMirror = boolean;

	//for (Rectangle* theRectangle : theRects) {
	//	theRectangle->theMaterial.isMirror = boolean; //Redundancy measure
	//}
}

glm::dvec3 const Cube::GetLength() {
	return glm::dvec3(0, 0, 0);
}

glm::dvec3 const Cube::GetCenterPoint() {
	return glm::dvec3(0, 0, 0);
}
