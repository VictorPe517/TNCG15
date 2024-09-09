#include "ColorDBL.h"

ColorDBL::ColorDBL(double _r, double _g, double _b) :r{ _r }, g{ _g }, b{ _b } {}

ColorDBL ColorDBL::operator+=(ColorDBL rhs) {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;

	return *this;
}

ColorDBL ColorDBL::operator-(ColorDBL rhs) {
	ColorDBL res(0, 0, 0);

	res.r = r - rhs.r;
	res.g = g - rhs.g;
	res.b = b - rhs.b;

	return res;
}

ColorDBL ColorDBL::operator/(double rhs) {
	r /= rhs;
	g /= rhs;
	b /= rhs;

	return *this;
}

ColorDBL ColorDBL::operator*=(ColorDBL rhs) {
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;

	return *this;
}

ColorDBL ColorDBL::operator*(double rhs) {
	ColorDBL finalColor = *this;

	finalColor.r *= rhs;
	finalColor.g *= rhs;
	finalColor.b *= rhs;

	return finalColor;
}

ColorDBL ColorDBL::operator*(ColorDBL rhs) {
	ColorDBL finalColor = *this;

	finalColor.r *= rhs.r;
	finalColor.g *= rhs.g;
	finalColor.b *= rhs.b;

	return finalColor;
}

double ColorDBL::distance() {
	return sqrt(r * r + g * g + b * b);
}

std::string ColorDBL::ToString() {
	std::string res("r: " + std::to_string(r) + ", b:" + std::to_string(b) + ", g: " + std::to_string(g) + "\n");

	return res;
};

const ColorDBL ColorDBL::Red = ColorDBL(0.8, 0.2, 0.2);
const ColorDBL ColorDBL::Green = ColorDBL(0.2, 0.8, 0.2);
const ColorDBL ColorDBL::Blue = ColorDBL(0.2, 0.2, 0.8);
const ColorDBL ColorDBL::Cyan = ColorDBL(0.2, 0.8, 0.8);
const ColorDBL ColorDBL::Magenta = ColorDBL(0.8, 0.2, 0.8);
const ColorDBL ColorDBL::Yellow = ColorDBL(0.8, 0.8, 0.2);
const ColorDBL ColorDBL::Orange = ColorDBL(0.9, 0.4, 0.0);

const ColorDBL ColorDBL::PureRed = ColorDBL(1.0, 0.0, 0.0);
const ColorDBL ColorDBL::PureGreen = ColorDBL(0.0, 1.0, 0.0);
const ColorDBL ColorDBL::PureBlue = ColorDBL(0.0, 0.0, 1.0);

const ColorDBL ColorDBL::White = ColorDBL(1.0, 1.0, 1.0);
const ColorDBL ColorDBL::Black = ColorDBL(0.05, 0.05, 0.05);