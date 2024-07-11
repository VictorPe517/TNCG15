#include "Ray.h"

ColorDBL Ray::calcIrradiance(const glm::dvec3& objectNormal, const glm::dvec3& lightNormal, const double& area, const glm::dvec3& intersectionPoint, const glm::dvec3& areaLightPoint, const std::vector<Object*>& theObjects, const LightSource& theLight, const bool& debug, const int& theHitIndex) {
	glm::dvec3 y = areaLightPoint; //Random punkt på lampan
	glm::dvec3 x = intersectionPoint;  //Intersection from eye->object

	glm::dvec3 d = y - x;
	glm::dvec3 d_normalized = glm::normalize(d);
	double distance = glm::length(d);

	double cos_omega_x = glm::clamp(glm::dot(glm::normalize(objectNormal), d) / distance, 0.0, 999999999.0);

	double cos_omega_y = -1.0 * glm::dot(lightNormal, d) / distance;

	if (cos_omega_y < 0.0) cos_omega_y = 0.0;

	double G = (cos_omega_x * cos_omega_y) / (distance * distance);

	double shadowVar = calculateShadowRay(intersectionPoint, areaLightPoint, theObjects, theLight, debug);


	double E = 3200 * G * shadowVar * area * theLight.Watt / 100.0;


	//if (E > maxE) {
	//	maxE = E;
	//}


	if (E < 0) {
		return ColorDBL(0, 0, 0);
	}
	else {
		return ColorDBL(E, E, E);
	}
};

glm::dvec3 Ray::getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight, int iterations) {
	glm::dvec3 intersection = glm::dvec3(-1, -1, -1);
	double minLength = 9999999.0;

	RayColor = ColorDBL(0, 0, 0);

	for (size_t l = 0; l < theObjects.size(); l++) {

		glm::dvec3 theObjectNormal = (*theObjects[l]).normal(*this);
		double theDotProduct = glm::dot(theObjectNormal, direction);

		if (theDotProduct < 0.0) {

			glm::dvec3 possibleIntersection = (theObjects[l])->getIntersection(*this);

			double lengthStartIntersection = glm::length(this->startPosition - possibleIntersection);

			if (possibleIntersection != glm::dvec3(-9999, -9999, -9999) && minLength >= lengthStartIntersection) { //We hit the closest object
				this->hitObject = theObjects[l];
				minLength = lengthStartIntersection;
				intersection = possibleIntersection;
				hitIndex = l;


				if (!do_not_reflect && theObjects[l]->getMaterial().isMirror && bounces_left > 0) {
					//-----------------MIRROR-----------------//
					this->nextRay = new Ray(intersection, glm::reflect(direction, theObjectNormal), RayColor, 1.0, bounces_left - 1);
					intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);

					RayColor = (*nextRay).RayColor;

				}
				else if (theObjects[l]->getMaterial().isTransparent && bounces_left > 0) {
					//---------------TRANSPARENT--------------//
					this->nextRay = new Ray(intersection, getRefractedDirection(intersection, theObjectNormal, (*theObjects[l])), RayColor, 1.0, bounces_left - 1);

					intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);

					RayColor = (*nextRay).RayColor;
				}
				else {
					//---------------LAMBERTIAN---------------//
					RayColor = (theObjects[l]->getColor());
					calculateLighting(intersection, Object::theObjects, theLight, iterations);
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

	return glm::dvec3(-9999, -9999, -9999);
}

glm::dvec3 Ray::getReflectedDirection(glm::dvec3 surfaceNormal) {
	glm::dvec3 dir;

	dir = direction - 2.0 * (glm::dot(direction, glm::normalize(surfaceNormal)) * glm::normalize(surfaceNormal));

	return glm::normalize(dir);
}

LocalDirection Ray::getRandomLocalDirection() // Returns random direction using the cumulative distribution function
{
	LocalDirection result;

	double azimuth = (rand()) / ((RAND_MAX + 1.0));
	double omega = (rand()) / ((RAND_MAX + 1.0));

	result.azimuth = azimuth / (2 * std::numbers::pi);
	result.omega = (1 - cos(omega) * cos(omega));

	return result;
}

//            dir + normal          dir               dir              dir+origin
// WorldCartesian --> LocalCartesian --> Hemispherical --> LocalCartesian --> WorldCartesian

glm::dvec3 Ray::localCartesianToWorldCartesian(glm::dvec3 localDir, glm::dvec3 surfaceNormal)
{
	// Local system in the local system: x = ( 1, 0, 0 ), y = ( 0, 1, 0 ), z = ( 0, 0, 1 )
	// Local system   in  global system: x = (a1,a2,a3 ), y = (b1,b2,b3 ), z = (c1,c2,c3 )

	double x = localDir.x;
	double y = localDir.y;
	double z = localDir.z;

	glm::dvec3 a = glm::normalize(-1.0 * localDir + glm::dot(surfaceNormal, localDir) * surfaceNormal);
	glm::dvec3 b = glm::cross(surfaceNormal, a);

	glm::dvec3 res(x * a.x + y * b.x + z * surfaceNormal.x,
		x * a.y + y * b.y + z * surfaceNormal.y,
		x * a.z + y * b.z + z * surfaceNormal.z);

	return res;
}

glm::dvec3 Ray::hemisphericalToLocalCartesian(LocalDirection dir)
{
	return glm::dvec3(cos(dir.azimuth) * sin(dir.omega), sin(dir.azimuth) * sin(dir.omega), cos(dir.omega));
}

glm::dvec3 Ray::getRandomDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal)
{
	double twoPi = 2 * std::numbers::pi;

	LocalDirection dir = getRandomLocalDirection();

	glm::dvec3 localDir = hemisphericalToLocalCartesian(dir);
	glm::dvec3 worldDir = glm::normalize(localCartesianToWorldCartesian(localDir, surfaceNormal));

	if (glm::dot(worldDir, surfaceNormal) < 0.0f) worldDir = -1.0 * worldDir; //feels superflous with cumulative distribution function

	return worldDir;
}

void Ray::calculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt) {
	ColorDBL finalPixelColor(0, 0, 0);
	bool debug = false;

	if (dynamic_cast<LightSource*>((Object::theObjects[hitIndex])) != nullptr) { //If we hit a lamp just return the lamp color
		RayColor = ColorDBL(1, 1, 1);
		return;
	}

	for (size_t iter = 0; iter < iterationAmt; iter++) {
		glm::dvec3 thePoint = (theLight).getRandomPoint();

		Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 0, 1), (theLight).radiance, 1);

		finalPixelColor += (RayColor * newRay.calcIrradiance(((*Object::theObjects[hitIndex]).normal(newRay)), (theLight).normal(newRay), theLight.area, hitPoint, thePoint, theObjects, theLight, debug, hitIndex)) / (iterationAmt * 3200);
	}

	RayColor = finalPixelColor;
}

double Ray::calculateShadowRay(const glm::dvec3& hitPoint, const glm::dvec3& randomLightSourcePoint, const std::vector<Object*>& theObjects, const LightSource& theLight, const bool debug) {
	shadowCalculated = true;
	Ray shadowRay(hitPoint, randomLightSourcePoint - hitPoint, ColorDBL(1, 1, 1), 0, 0);

	double testLength = glm::distance(hitPoint, randomLightSourcePoint);

	glm::dvec3 prospectiveShadowImpact;
	glm::dvec3 intersection;
	double shadowLength = 999999.0;
	int index = -1;

	for (size_t l = 0; l < theObjects.size(); l++) {
		prospectiveShadowImpact = (theObjects[l])->getIntersection(shadowRay);

		if (shadowRay.hitObject != theObjects[l] && glm::length(prospectiveShadowImpact - randomLightSourcePoint) < testLength) {
			shadowLength = glm::length(prospectiveShadowImpact);
			intersection = prospectiveShadowImpact;
			index = l;
		}
	}

	if (index == 12) {
		//std::cout << "hit the light!\n";
		return 1.0;
	}

	if (debug) {
		std::cout << "ShadowImpact: " << glm::to_string(prospectiveShadowImpact) << "\n randomLightSource: " << glm::to_string(randomLightSourcePoint) << "\n\n";
		std::cout << "There's an object in the way!: " << index << "\n";
		std::cout << "Ray start position = " << glm::to_string(shadowRay.startPosition) << ", in direction: " << glm::to_string(shadowRay.direction) << "\n\n";
		std::cout << "Ray from: " << glm::to_string(hitPoint) << ", object hitIndex" << hitIndex << ", blocked by object l: " << index << ", at position: " << glm::to_string(intersection) << ", to light position: " << glm::to_string(randomLightSourcePoint) << "\n\n";
	}

	return 0.0;

}
