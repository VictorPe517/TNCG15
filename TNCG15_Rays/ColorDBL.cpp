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
    r *= rhs;
    g *= rhs;
    b *= rhs;

    return *this;
}

ColorDBL ColorDBL::operator*(ColorDBL rhs) {
    ColorDBL finalColor = *this;

    finalColor.r *= rhs.r;
    finalColor.g *= rhs.g;
    finalColor.b *= rhs.b;

    return finalColor;
}

std::string ColorDBL::ToString() {
    std::string res("r: " + std::to_string(r) + ", b:" + std::to_string(b) + ", g: " + std::to_string(g) + "\n");

    return res;
};