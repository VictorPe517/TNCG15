#pragma once
class ColorDBL {
public:
    ColorDBL(double _r, double _g, double _b) :r{ _r }, g{ _g }, b{ _b } {}

    ColorDBL operator+=(ColorDBL rhs) {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;

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
   
    double r, g, b;
};

