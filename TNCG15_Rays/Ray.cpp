#include "Ray.h"

ColorDBL Ray::calcIrradiance(glm::dvec3 x_normal, glm::dvec3 y_normal, double area, glm::dvec3 intersectionPoint, glm::dvec3 areaLightPoint, std::vector<Object*>& theObjects,LightSource theLight) {
    glm::dvec3 y = areaLightPoint; //Random punkt på lampan
    glm::dvec3 x = intersectionPoint;  //Intersection from eye->object

    glm::dvec3 d = x - y;
    double distance = glm::distance(x, y);

    double cos_omega_x = glm::dot(x_normal, d) / (distance);
    double cos_omega_y = -1.0 * glm::dot(y_normal, d) / (distance);

    double G = cos_omega_x * cos_omega_y / (distance * distance);

    double shadowVar = calculateShadowRay(intersectionPoint, areaLightPoint, theObjects, theLight);

    double E = 3200 * G * shadowVar * area;

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

glm::dvec3 Ray::getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight) {
    glm::dvec3 intersection = glm::dvec3(-1, -1, -1);
    double minLength = 9999999.0;

    RayColor = ColorDBL(0, 0, 0);

    for (size_t l = 0; l < theObjects.size(); l++) {
        if (glm::dot((*theObjects[l]).normal(*this), direction) < 0.0) {
            glm::dvec3 possibleIntersection = (theObjects[l])->getIntersection(*this);

            if (possibleIntersection != glm::dvec3(-9999, -9999, -9999) && minLength >= glm::length(this->startPosition - possibleIntersection)) { //We hit the closest object
                this->hitObject = theObjects[l];
                minLength = glm::length(this->startPosition - possibleIntersection);
                intersection = possibleIntersection;
                hitIndex = l;

                if (!do_not_reflect && theObjects[l]->getMaterial().isMirror && bounces_left > 0) {
                    //Bounce ray

                    this->nextRay = new Ray(intersection, glm::reflect(direction, (theObjects[l])->normal(*this)), RayColor, 1.0, bounces_left - 1);
                    intersection = (* nextRay).getPointOfIntersection(theObjects, theLight);

                    RayColor = (*nextRay).RayColor;
                    
                }
                else {
                    RayColor = (theObjects[l]->getColor());
                    calculateLighting(intersection, Object::theObjects, theLight, 900);
                }
                //std::cout << "  Hit! r:" << RayColor.r << ", g: " << RayColor.g << ", b: " << RayColor.b << "\n";
            }
        }
    }
    return intersection; //Return closest collision coordinates
}

glm::dvec3 Ray::getReflectedDirection(glm::dvec3 surfaceNormal) {
    glm::dvec3 dir;

    dir = direction - 2.0 * (glm::dot(direction, glm::normalize(surfaceNormal)) * glm::normalize(surfaceNormal));

    return glm::normalize(dir);
}

void Ray::calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt) {
    ColorDBL finalPixelColor(0,0,0);

    if (dynamic_cast<LightSource*>((Object::theObjects[hitIndex])) != nullptr) { //If we hit a lamp just return the lamp color
        RayColor = ColorDBL(1, 1, 1);
        return;
    }

    for (size_t iter = 0; iter < iterationAmt; iter++) {
        glm::dvec3 thePoint = (theLight).getRandomPoint();

        Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 0, 1), (theLight).radiance, 0);
       
        //double shadowVar = calculateShadowRay(hitPoint, thePoint, theObjects, theLight);

        finalPixelColor += (RayColor *  newRay.calcIrradiance(((*Object::theObjects[hitIndex]).normal(newRay)),
                (theLight).normal(newRay), theLight.area, hitPoint, thePoint, theObjects, theLight)) / (iterationAmt * 3200);

        
        //RayColor *= newRay.calcIrradiance(((*Object::theObjects[hitIndex]).normal(newRay)), 
        //                                  (theLight).normal(newRay), 6.0, hitPoint, thePoint)*0.005 / iterationAmt; //Give color of rectangle or triangle to pixel
    }

    RayColor = finalPixelColor;
}

double Ray::calculateShadowRay(glm::dvec3& hitPoint, glm::dvec3& randomLightSourcePoint, std::vector<Object*>& theObjects, LightSource& theLight) {
    shadowCalculated = true;
    Ray shadowRay(hitPoint, randomLightSourcePoint - hitPoint, ColorDBL(1, 1, 1), 50, 0);
    shadowRay.do_not_reflect = true;

    double testLength = glm::distance(hitPoint, randomLightSourcePoint);

    glm::dvec3 prospectiveShadowImpact;
    glm::dvec3 intersection;
    double shadowLength = 999999.0;
    int index = -1;

    for (size_t l = 0; l < theObjects.size(); l++) {
        prospectiveShadowImpact = (theObjects[l])->getIntersection(shadowRay);

        if (shadowRay.hitObject != theObjects[l] && glm::length(prospectiveShadowImpact-randomLightSourcePoint) < testLength) {
            shadowLength = glm::length(prospectiveShadowImpact);
            intersection = (theObjects[l])->getIntersection(shadowRay);
            index = l;
        }
    }

    //std::cout << "ShadowImpact: " << glm::to_string(prospectiveShadowImpact) << "\n randomLightSource: " << glm::to_string(randomLightSourcePoint) << "\n\n";

    if(index == 12) {
        //std::cout << "hit the light!\n";
        return 1.0;
    }
    
     //std::cout << "didnt hit\n";
     /*std::cout << "There's an object in the way!: " << index << "\n";
     std::cout << "Ray start position = " << glm::to_string(shadowRay.startPosition) << ", in direction: " << glm::to_string(shadowRay.direction) << "\n\n";
     std::cout << "Ray from: " << glm::to_string(hitPoint) << ", blocked by object l: " << index << ", at position: "  << glm::to_string(intersection) << ", to light position: " << glm::to_string(randomLightSourcePoint) << "\n\n";
  */   return 0.0;
    
}
