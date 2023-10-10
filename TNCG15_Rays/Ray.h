#pragma once
#include "glm/glm.hpp"
#include "ColorDBL.h"
#include <iostream>
#include <cmath>
#include <numbers>

#define _USE_MATH_DEFINES

class Ray {
public:
    Ray() = default;

    Ray(glm::dvec3 start, glm::dvec3 dir, ColorDBL _RayColor, double _radiance) : startPosition{ start }, direction{ glm::normalize(dir) }, RayColor { _RayColor }, radiance{_radiance} {    }
    
    static double maxE;
    
    glm::dvec3 startPosition = glm::dvec3(0, 0, 0);
    glm::dvec3 direction = glm::dvec3(0, 0, 0);
    Ray* prevRay = nullptr;
    Ray* nextRay = nullptr;
    //Polygon* Surface = nullptr;
    ColorDBL RayColor = ColorDBL(0.0,0.0,0.0);
    double radiance = 0.0;
    double importance = 0.0;
    double reflectivity = 1.0;

    ColorDBL calcIrradiance(glm::dvec3 x_normal, glm::dvec3 y_normal, double Lradiance, double area) {
        double irradiance = 0.0;

        glm::dvec3 y = startPosition;
        glm::dvec3 x = direction+ startPosition;

        glm::dvec3 d = glm::normalize(y-x);
        double distance = glm::distance(y, x);


        double cos_omega_x = glm::dot(x_normal, d) / (distance * distance);
        double cos_omega_y = glm::dot(-1.0*y_normal, d) / (distance * distance);

        double G = cos_omega_x * cos_omega_y / (distance * distance);

        double E = area*3200 * G;

        //std::cout << "Irradiance value is: " << E << "\n\n";

        if (E > maxE) {
            maxE = E;
        }

        if (E < 0) {
            return ColorDBL(0, 0, 0);
        }
        else {
            return ColorDBL(E, E, E);
        }
        
    };

};

