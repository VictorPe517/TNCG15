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
};

