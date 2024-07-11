#pragma once

#include <iostream>
#include <string>
#include <string.h>

class ColorDBL {
public:
	ColorDBL(double _r, double _g, double _b);

	ColorDBL operator+=(ColorDBL rhs);

	ColorDBL operator-(ColorDBL rhs);

	ColorDBL operator/(double rhs);

	ColorDBL operator*=(ColorDBL rhs);

	ColorDBL operator*(double rhs);

	ColorDBL operator*(ColorDBL rhs);

	std::string ToString();

	double r, g, b;

	static const ColorDBL Red;
	static const ColorDBL Green;
	static const ColorDBL Blue;
	static const ColorDBL Cyan;
	static const ColorDBL Magenta;
	static const ColorDBL Yellow;
	static const ColorDBL White;
	static const ColorDBL Black;


};

