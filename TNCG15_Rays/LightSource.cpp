#include "LightSource.h"


//Returns a normalized 3-dimensional vector pointing straight out of the surface
glm::dvec3 LightSource::normal(const Ray& theRay) {
	glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);

	//std::cout << "	Light normal is: " << glm::to_string(direction) << "\n\n";

	return glm::normalize(direction);
}


//Returns a random point in world-space anywhere on the light source
glm::dvec3 const LightSource::getRandomPoint() {
	double randY = (rand()) / ((RAND_MAX + 1.0));
	double randX = (rand()) / ((RAND_MAX + 1.0));

	glm::dvec3 verticalVec = v2 - v1;
	glm::dvec3 horizontVec = v3 - v1;

	return(v1 + randY * verticalVec + randX * horizontVec);
}


//Returns the point of intersection between a ray and the particular light source in world-space
glm::dvec3 LightSource::getIntersection(const Ray& theRay) {
	glm::dvec3 vertex = v1;

	glm::dvec3 c1 = v2 - vertex; //v3
	glm::dvec3 c2 = v3 - vertex; //v1

	glm::dvec3 _theNormal = this->normal(theRay);

	double t = glm::dot((this->v2 - (theRay).startPosition), _theNormal) / (glm::dot((theRay).direction, _theNormal));

	glm::dvec3 possibleIntersection = (theRay).startPosition + t * (theRay).direction;

	double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
	double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));


	if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0) {
		return(possibleIntersection);
	}
	else {
		return glm::dvec3(-9999, -9999, -9999); //If vector is empty then no intersection
	}
}

void LightSource::Translate(glm::dvec3 coordinates)
{
	v1 += coordinates;
	v2 += coordinates;
	v3 += coordinates;
	v4 += coordinates;
}


//Returns the area of the lightsource. Already called in constructor, so use variable "(LightSource).area" instead
double LightSource::calculateArea() {
	area = glm::distance(v2, v1) * glm::distance(v3, v1);
	return area;
}
