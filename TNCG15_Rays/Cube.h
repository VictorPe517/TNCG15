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

	Cube(Rectangle _r1, Rectangle _r2, Rectangle _r3, Rectangle _r4, Rectangle _r5, Rectangle _r6, ColorDBL _Color);

	Rectangle* latestHitRectangle = nullptr;

	virtual glm::dvec3 normal(Ray& theRay) override;

	virtual glm::dvec3 getIntersection(Ray& theRay) override;

	virtual ColorDBL getColor() override;

	virtual Material getMaterial() override;

	std::vector<Rectangle*> theRects;
	Material theMaterial = Material(0, 0, 0, false, ColorDBL(1, 1, 1));
	static std::vector<Cube> theCubes;


};

