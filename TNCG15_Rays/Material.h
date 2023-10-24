#pragma once
#include "ColorDBL.h"

class Material
{
public:
	Material() = default;

	Material(double _refl, double _rough, double _transm, bool _mirror, ColorDBL _color) 
		: reflectance{_refl}, roughness{_rough}, transmission{_transm}, isMirror{_mirror}, MatColor{_color}{}

	double reflectance = 0.0;
	double roughness = 1.0;
	double transmission = 0.0;
	double refractiveIndex = 1.5;
	bool isMirror = false;
	bool isTransparent = false;
	ColorDBL MatColor = ColorDBL(1,1,1);
};

