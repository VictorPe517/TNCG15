#pragma once

#include <iostream>
#include <string>
#include <string.h>

class ColorDBL {
public:
    ColorDBL(double _r, double _g, double _b) :r{ _r }, g{ _g }, b{ _b } {}

    ColorDBL operator+=(ColorDBL rhs) {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;

        return *this;
    }

    ColorDBL operator-(ColorDBL rhs) {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;

        return *this;
    }

    ColorDBL operator/(double rhs) {
        r /= rhs;
        g /= rhs;
        b /= rhs;

        return *this;
    }

    ColorDBL operator*=(ColorDBL rhs) {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;

        return *this;
    }

    ColorDBL operator*(double rhs) {
        r *= rhs;
        g *= rhs;
        b *= rhs;

        return *this;
    }

    std::string ToString() {
        std::string res("r: " + std::to_string(r) + ", b:" + std::to_string(b) + ", g: " + std::to_string(g)+ "\n");

        return res;
    };
   
    double r, g, b;
};

