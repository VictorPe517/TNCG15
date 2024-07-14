#include "Ray.h"

void Ray::SetRayColor(ColorDBL theColor) {
	RayColor = theColor;
}

ColorDBL Ray::GetRayColor() {
	return RayColor;
}

double Ray::calcIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, LightSource& theLight) {
	double E = 0.0;

	// Todo: iterate over all lamps here
	//--- loop loop ---//
	glm::dvec3 randomPointOnLamp = theLight.getRandomPoint();
	glm::dvec3 lampToPointVec = randomPointOnLamp - intersectionPoint;
	glm::dvec3 lightNormal = theLight.normal(*this);
	double area = theLight.area;

	double distance = glm::length(lampToPointVec);
	double cos_omega_x = glm::clamp(glm::dot(glm::normalize(surfaceNormal), lampToPointVec) / distance, 0.0, (double)INFINITY);
	double cos_omega_y = -1.0 * glm::dot(lightNormal, lampToPointVec) / distance;

	if (cos_omega_y < 0.0) cos_omega_y = 0.0;

	double G = (cos_omega_x * cos_omega_y) / (distance * distance);
	double shadowVar = calculateShadowRay(intersectionPoint, randomPointOnLamp, theObjects);

	E += 3200 * G * shadowVar * area * theLight.Watt / 100.0;
	//--- end loop ---//

	if (E < DBL_EPSILON) {
		this->irradiance = 0.0;
		return 0.0;
	}
	else {
		this->irradiance = E;
		return E;
	}
};

glm::dvec3 Ray::getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight, int iterations) {
	glm::dvec3 intersection = glm::dvec3(-1, -1, -1);
	double minLength = INFINITY;

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


				if (theObjects[l]->getMaterial().isMirror && bounces_left > 0) {
					//---------------[ MIRROR ]---------------//
					this->nextRay = new Ray(intersection, glm::reflect(direction, theObjectNormal), RayColor, 1.0, bounces_left - 1);
					intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);

					SetRayColor((*nextRay).GetRayColor());

				}
				else if (theObjects[l]->getMaterial().isTransparent && bounces_left > 0) {
					//-------------[ TRANSPARENT ]------------//
					this->nextRay = new Ray(intersection, getRefractedDirection(intersection, theObjectNormal, (*theObjects[l])), RayColor, 1.0, 3);

					intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);

					SetRayColor((*nextRay).GetRayColor());
				}
				else {
					//-------------[ LAMBERTIAN ]-------------//
					SetRayColor((theObjects[l]->getColor()));
					calculateLighting(intersection, Object::theObjects, theLight, iterations);
				}
				//std::cout << "  Hit! r:" << RayColor.r << ", g: " << RayColor.g << ", b: " << RayColor.b << "\n";
			}
		}
	}
	return intersection; //Return closest collision coordinates
}

glm::dvec3 Ray::getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject) {
	glm::dvec3 d0 = glm::normalize(this->direction);

	double eta = theObject.getMaterial().refractiveIndex;

	double nDot = glm::dot(surfaceNormal, d0);

	double k = 1.0 - pow(eta, 2.0) * (1.0 - pow(nDot, 2.0));

	if (k < 0.0000000001) {
		//return getReflectedDirection(surfaceNormal);
		return glm::dvec3(0, 0, 0);
	}

	return glm::normalize(eta * d0 + surfaceNormal * (-1.0 * eta * (nDot)-sqrt(k)));
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

	if (dynamic_cast<LightSource*>((Object::theObjects[hitIndex])) != nullptr) { //If we hit a lamp return the lamp color
		RayColor = ColorDBL(1.0, 1.0, 1.0) / 8.0;
		return;
	}

	for (size_t iter = 0; iter < iterationAmt; iter++) {
		glm::dvec3 thePoint = (theLight).getRandomPoint();

		Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 0, 1), (theLight).radiance, 1);
		glm::dvec3 surfaceNormal = ((*Object::theObjects[hitIndex]).normal(newRay));

		double dx = ((float)iterationAmt * 3200.0);

		finalPixelColor += (RayColor * newRay.calcIrradiance(surfaceNormal, hitPoint, theObjects, theLight)) / dx;
	}

	RayColor = finalPixelColor;
}

// Calculates wether the surface is in shadow (returns 0.0) or in light (returns 1.0)
double Ray::calculateShadowRay(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightSourcePoint, const std::vector<Object*>& theObjects) {
	Ray shadowRay(surfaceHitPoint, randomLightSourcePoint - surfaceHitPoint, ColorDBL(1, 1, 1), 0, 0);

	double testLength = glm::distance(surfaceHitPoint, randomLightSourcePoint);

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

	if (index != -1 && dynamic_cast<LightSource*>((Object::theObjects[index])) != nullptr) {
		// If we can cast the hit object to a LightSource, it IS a lightsource, and we should return fullbright
		return 1.0;
	}


	return 0.0;

}
