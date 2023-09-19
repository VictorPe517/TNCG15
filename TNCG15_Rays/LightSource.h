#pragma once

#define _USE_MATH_DEFINES

#include "glm/glm.hpp"
#include "ColorDBL.h"
#include <cmath>
#include <numbers>
#include "Ray.h"
#include <iostream>
#include <stdlib.h>

class LightSource
{
public:
	LightSource(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, glm::dvec3 _v4, double _watt, ColorDBL _color) : v1{ _v1 }, v2{ _v2 }, v3{ _v3 }, v4{ _v4 }, Watt{ _watt }, Color{_color} {
		area = calculateArea();
		std::cout << area << std::endl;
	}

	glm::dvec3 v1, v2, v3, v4;
	double Watt = 100.0;
	double area = 1.0;
	double radiosity = 10000;
	double radiance = radiosity;

	double calculateArea() {
		area = glm::length(v2 - v1) * glm::length(v3 - v1);
		return area;
	}

	glm::dvec3 normal() {
		glm::dvec3 direction = glm::cross(this->v2 - this->v1, this->v3 - this->v1);
		return direction / glm::length(direction);
	}

	glm::dvec3 getRandomPoint() {
		double rand1 = (rand()) / ((RAND_MAX + 1.0));


		double rand2 = (rand()) / ((RAND_MAX + 1.0));
		
		glm::dvec3 e1 = v2 - v1;
		glm::dvec3 e2 = v3 - v1;

		return(v1 + rand1 * e1 + rand2 * e2);
	}

	glm::vec3 isIntersection(Ray theRay) {
		glm::dvec3 vertex = v2;

		glm::dvec3 c1 = v3 - vertex;
		glm::dvec3 c2 = v1 - vertex;

		double t = glm::dot((this->v2 - theRay.startPosition), this->normal()) / (glm::dot(theRay.direction, this->normal()));

		glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

		double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
		double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));

		if (a <= 0.0 || b <= 1.0) {
			return(possibleIntersection);
		}
	};

	glm::dvec3 getIntersection(Ray theRay) {
		glm::dvec3 vertex = v2;

		glm::dvec3 c1 = v3 - vertex;
		glm::dvec3 c2 = v1 - vertex;

		double t = glm::dot((this->v2 - theRay.startPosition), this->normal()) / (glm::dot(theRay.direction, this->normal()));

		glm::dvec3 possibleIntersection = theRay.startPosition + t * theRay.direction;

		double a = glm::dot((possibleIntersection - vertex), c1) / (dot(c1, c1));
		double b = glm::dot((possibleIntersection - vertex), c2) / (dot(c2, c2));


		if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0) {
			return(possibleIntersection);
		}
		else {
			return glm::dvec3(0, 0, 0); //If vector is empty then no intersection
		}
	};

	ColorDBL Color = ColorDBL(1.0, 1.0, 1.0);
};

