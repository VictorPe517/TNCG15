#pragma once
#include "ColorDBL.h"

class Material
{
public:
	Material() : radiosity{ 1.0 }, roughness{ 1.0 }, transmission{ 0.0 }, isMirror{ false }, MatColor{ ColorDBL(1, 1, 1) }{}

	Material(double _refl, double _rough, double _transm, bool _mirror, ColorDBL _color)
		: radiosity{ _refl }, roughness{ _rough }, transmission{ _transm }, isMirror{ _mirror }, MatColor{ _color }{}

	double radiosity = 1.0;
	double roughness = 1.0;
	double transmission = 0.0;
	double refractiveIndex = 1.3;
	bool isMirror = false;
	bool isTransparent = false;
	ColorDBL MatColor = ColorDBL(1, 1, 1);
};

