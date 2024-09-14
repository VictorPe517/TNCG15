#pragma once

#include "glm/glm.hpp"
#include "Polygon.h"
#include "ColorDBL.h"
#include "Ray.h"
#include "glm/gtx/string_cast.hpp"
#include "Material.h"
#include "Rectangle.h"

class Cube : public Object {
public:

	// Create (uniform) cube by defining a midpoint and desired sidelength
	Cube(glm::dvec3 midpoint, double sideLength, ColorDBL _Color = ColorDBL(1, 1, 1));

	// Create cube by defining its rectangles manually
	Cube(Rectangle _r1, Rectangle _r2, Rectangle _r3, Rectangle _r4, Rectangle _r5, Rectangle _r6, ColorDBL _Color);


	virtual glm::dvec3 CalculateNormal(const Ray& theRay) override;

	virtual glm::dvec3 GetIntersection(const Ray& theRay) override;


	virtual ColorDBL GetColor() override;

	virtual Material GetMaterial() override;

	virtual const glm::dvec3 GetLength() override;

	virtual const glm::dvec3 GetCenterPoint() override;

	void setMirror(bool boolean);

	std::vector<Rectangle*> theRects;
	Material theMaterial = Material(1, 1, 1, false, ColorDBL(1, 1, 1));
	static std::vector<Cube> theCubes;


};

