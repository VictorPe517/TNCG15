#include "Ray.h"

ColorDBL Ray::calcIrradiance(glm::dvec3 objectNormal, glm::dvec3 lightNormal, double area, glm::dvec3 intersectionPoint, glm::dvec3 areaLightPoint, std::vector<Object*>& theObjects,LightSource theLight, bool debug, int theHitIndex) {
    glm::dvec3 y = areaLightPoint; //Random punkt på lampan
    glm::dvec3 x = intersectionPoint;  //Intersection from eye->object

    glm::dvec3 d = y - x;
    glm::dvec3 d_normalized = glm::normalize(d);
    double distance = glm::length(d);

    double cos_omega_x = glm::clamp(glm::dot(glm::normalize(objectNormal), d)/distance,0.0,999999999.0);

    double cos_omega_y = -1.0 * glm::dot(lightNormal, d)/distance;

    if (cos_omega_y < 0.0) cos_omega_y = 0.0;

    double G = (cos_omega_x * cos_omega_y) / (distance * distance);

    //double shadowVar = calculateShadowRay(intersectionPoint, areaLightPoint, theObjects, theLight, debug);


    double E = 3200 * G * 1.0 * area * theLight.Watt/100.0;


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
                    //-----------------MIRROR-----------------//
                    this->nextRay = new Ray(intersection, glm::reflect(direction, (theObjects[l])->normal(*this)), RayColor, 1.0, bounces_left - 1);
                    intersection = (* nextRay).getPointOfIntersection(theObjects, theLight);

                    RayColor = (*nextRay).RayColor;
                    
                }
                else if (theObjects[l]->getMaterial().isTransparent && bounces_left > 0) {
                    //---------------TRANSPARENT--------------//
                    this->nextRay = new Ray(intersection, getRefractedDirection(intersection, (theObjects[l])->normal(*this), (*theObjects[l])), RayColor, 1.0, bounces_left - 1);
                    intersection = (*nextRay).getPointOfIntersection(theObjects, theLight);

                    RayColor = (*nextRay).RayColor;
                }
                else {
                    //---------------LAMBERTIAN---------------//
                    RayColor = (theObjects[l]->getColor());
                    calculateLighting(intersection, Object::theObjects, theLight, 50);
                }
                //std::cout << "  Hit! r:" << RayColor.r << ", g: " << RayColor.g << ", b: " << RayColor.b << "\n";
            }
        }
    }
    return intersection; //Return closest collision coordinates
}

glm::dvec3 Ray::getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject) {
    glm::dvec3 d0 = glm::normalize(direction);
    glm::dvec3 direction;

    double n1 = theObject.getMaterial().refractiveIndex;
    double n2 = 1.0;

    double R = n2 / n1;

    double Nd0 = glm::dot(surfaceNormal, d0);

    direction = R * d0 + surfaceNormal * (-R * (Nd0)-glm::sqrt(1 - R * R * (1 - (Nd0))));
    
    if (n1 * n2 / n2 < 1) {
        return glm::normalize(direction);
    }

    return glm::dvec3(-9999,-9999,-9999);
}

glm::dvec3 Ray::getReflectedDirection(glm::dvec3 surfaceNormal) {
    glm::dvec3 dir;

    dir = direction - 2.0 * (glm::dot(direction, glm::normalize(surfaceNormal)) * glm::normalize(surfaceNormal));

    return glm::normalize(dir);
}

LocalDirection Ray::getRandomDirection() //Returns random direction using the cumulative distribution function
{
    LocalDirection result;

    double azimuth = (rand()) / ((RAND_MAX + 1.0));
    double omega = (rand()) / ((RAND_MAX + 1.0));

    result.azimuth = azimuth / (2 * std::numbers::pi);
    result.omega = (1 - cos(omega) * cos(omega));

    return result;
}

glm::dvec3 Ray::localToWorldCoordinates(LocalDirection dir, glm::dvec3 worldpoint)
{
    return glm::dvec3();
}

LocalDirection Ray::worldToLocalCoordinates(glm::dvec3 dir, glm::dvec3 point)
{
    return LocalDirection();
}

void Ray::calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt) {
    ColorDBL finalPixelColor(0,0,0);
    bool debug = false;

    if (dynamic_cast<LightSource*>((Object::theObjects[hitIndex])) != nullptr) { //If we hit a lamp just return the lamp color
        RayColor = ColorDBL(1, 1, 1);
        return;
    }

    for (size_t iter = 0; iter < iterationAmt; iter++) {
        glm::dvec3 thePoint = (theLight).getRandomPoint();

        Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 0, 1), (theLight).radiance, 1);
            
        finalPixelColor += (RayColor *  newRay.calcIrradiance(((*Object::theObjects[hitIndex]).normal(newRay)), (theLight).normal(newRay), theLight.area, hitPoint, thePoint, theObjects, theLight, debug, hitIndex)) / (iterationAmt * 3200);
    }

    RayColor = finalPixelColor;
}

double Ray::calculateShadowRay(glm::dvec3& hitPoint, glm::dvec3& randomLightSourcePoint, std::vector<Object*>& theObjects, LightSource& theLight, bool debug) {
    shadowCalculated = true;
    Ray shadowRay(hitPoint, randomLightSourcePoint - hitPoint, ColorDBL(1, 1, 1), 50, 5);


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

    if(debug)std::cout << "ShadowImpact: " << glm::to_string(prospectiveShadowImpact) << "\n randomLightSource: " << glm::to_string(randomLightSourcePoint) << "\n\n";

    if(index == 12) {
        //std::cout << "hit the light!\n";
        return 1.0;
    }

    if (debug) {
        std::cout << "There's an object in the way!: " << index << "\n";
        std::cout << "Ray start position = " << glm::to_string(shadowRay.startPosition) << ", in direction: " << glm::to_string(shadowRay.direction) << "\n\n";
        std::cout << "Ray from: " << glm::to_string(hitPoint) <<", object hitIndex" << hitIndex << ", blocked by object l: " << index << ", at position: " << glm::to_string(intersection) << ", to light position: " << glm::to_string(randomLightSourcePoint) << "\n\n";
    }

     return 0.0;
    
}
