#include "Ray.h"

int counter = 0;

void Ray::SetRayColor(ColorDBL theColor) {
	RayRadianceColor = theColor;
}

ColorDBL Ray::GetRayColor() {
	return RayRadianceColor;
}

double Ray::CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, LightSource& theLight) {
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
	double shadowVar = IsVisibleToPoint(intersectionPoint, randomPointOnLamp, theObjects);

	E += 3200 * G * shadowVar * area * theLight.Watt / 100.0;
	//--- end loop ---//

	if (E < DBL_EPSILON) {
		this->Irradiance = 0.0;
		return 0.0;
	}
	else {
		this->Irradiance = E;
		return E;
	}
};



glm::dvec3 Ray::getPointOfIntersection(std::vector<Object*> theObjects, LightSource& theLight, int iterations) {
	glm::dvec3 intersection = glm::dvec3(0, 0, 0);
	double minLength = INFINITY;

	//std::cout << "current Dir outside func: " << glm::to_string(this->direction) << "\n";

	RayRadianceColor = ColorDBL(0, 0, 0);

	for (size_t l = 0; l < theObjects.size(); l++) {

		glm::dvec3 theObjectNormal = (*theObjects[l]).normal(*this);

		double theDotProduct = glm::dot(theObjectNormal, direction);

		if (theDotProduct < 0.0) {

			glm::dvec3 possibleIntersection = (theObjects[l])->getIntersection(*this);

			double lengthStartIntersection = glm::length(this->startPosition - possibleIntersection);

			if (possibleIntersection != glm::dvec3(NAN, NAN, NAN) && minLength >= lengthStartIntersection) { //We hit the closest object
				this->hitObject = theObjects[l];
				minLength = lengthStartIntersection;
				intersection = possibleIntersection;
				hitIndex = l;

				endPosition = intersection;

				if (theObjects[l]->getMaterial().isMirror && bounces_left > 0) {
					//---------------[ MIRROR ]---------------//

					this->nextRay = new Ray(intersection, getReflectedDirection(theObjectNormal), RayRadianceColor, 1.0, bounces_left - 1);
					this->nextRay->startSurface = theObjects[l];

					intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);

					SetRayColor((*nextRay).GetRayColor());

				}
				else if (theObjects[l]->getMaterial().isTransparent && bounces_left > 0) {
					//-------------[ TRANSPARENT ]------------//
					double n1;
					double n2;

					if (isInside) {
						n1 = (theObjects[l]->getMaterial().refractiveIndex);
						n2 = 1.0;
					}
					else {
						n1 = 1.0;
						n2 = (theObjects[l]->getMaterial().refractiveIndex);
					}

					double ior = n1 / n2;

					double R0 = pow((n1 - n2) / (n1 + n2), 2.0);
					double inclination = this->GetInclination(theObjectNormal);
					double radiance = R0 + (1.0 - R0) * pow((1.0 - cos(inclination)), 5.0);

					double transmittance = 1.0 - radiance;

					//std::cout << " Current Dir inside func: " << glm::to_string(this->direction) << "\n";

					glm::dvec3 refractedDir = getRefractedDirection(intersection, theObjectNormal, (*theObjects[l]), n1, n2);

					if (refractedDir != glm::dvec3(NAN, NAN, NAN)) {
						this->nextRay = new Ray(intersection, refractedDir, RayRadianceColor, radiance, 3);
						this->nextRay->startSurface = theObjects[l];
						this->nextRay->isInside = !this->isInside;

						intersection = (*nextRay).getPointOfIntersection(theObjects, theLight, iterations);
						SetRayColor((*nextRay).GetRayColor());
					}
				}
				else {
					//-------------[ LAMBERTIAN ]-------------//
					if (DrawRandom()) {  //todo: skapa ray path och sen calla p� calclightning och ber�kna genom hela pathen
						this->nextRay = new Ray(intersection, getRandomDirection(theObjectNormal), RayRadianceColor, 1.0, 10);
						intersection = this->nextRay->getPointOfIntersection(theObjects, theLight, iterations);
						SetRayColor(theObjects[l]->getColor());
						CalculateLighting(intersection, Object::theObjects, theLight, iterations);
					}
				}
			}
		}
	}
	return intersection; //Return closest collision coordinates
}

bool Ray::DrawRandom() {

	return rand() > RAND_MAX / 2.0;
}

glm::dvec3 Ray::getRefractedDirection(glm::dvec3 intersection, glm::dvec3 surfaceNormal, Object& theObject, double n1, double n2) {
	glm::dvec3 d0 = glm::normalize(this->direction);

	glm::dvec3 n = glm::normalize(surfaceNormal);

	double eta = n1 / n2;

	double inclination = acos(GetInclination(surfaceNormal));

	double nDot = glm::dot(d0, n);

	double k = 1.0 - pow(eta, 2.0) * (1.0 - pow(nDot, 2.0));

	if (k < 0.0) {
		return glm::dvec3(NAN, NAN, NAN);
	}

	if ((n1 * sin(inclination) / n2) < 1.0) {
		glm::dvec3 result = eta * d0 + n * (-1.0 * eta * (nDot)-sqrt(k));
		return glm::normalize(result);
	}
	else {
		return glm::dvec3(NAN, NAN, NAN);
	}

}

glm::dvec3 Ray::getReflectedDirection(glm::dvec3 surfaceNormal) {
	glm::dvec3 dir;

	dir = direction - 2.0 * (glm::dot(direction, glm::normalize(surfaceNormal)) * glm::normalize(surfaceNormal));

	return glm::normalize(dir);
}

LocalDirection Ray::getRandomLocalDirection() // Returns random direction using the cumulative distribution function
{
	LocalDirection result;

	double azimuth = ((double)rand()) / (((double)RAND_MAX + 1.0));
	double inclination = ((double)rand()) / (((double)RAND_MAX + 1.0));

	result.azimuth = azimuth / (2.0 * std::numbers::pi);
	result.inclination = (1.0 - cos(inclination) * cos(inclination));

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

glm::dvec3 Ray::hemisphericalToCartesian(LocalDirection dir)
{
	return glm::dvec3(cos(dir.azimuth) * sin(dir.inclination), sin(dir.azimuth) * sin(dir.inclination), cos(dir.inclination));
}

glm::dvec3 Ray::getRandomDirection(glm::dvec3 surfaceNormal)
{
	double twoPi = 2 * std::numbers::pi;

	LocalDirection dir = getRandomLocalDirection();

	glm::dvec3 localDir = hemisphericalToCartesian(dir);
	glm::dvec3 worldDir = glm::normalize(localCartesianToWorldCartesian(localDir, surfaceNormal));

	if (glm::dot(worldDir, surfaceNormal) < 0.0f) worldDir = -1.0 * worldDir; //feels superflous with cumulative distribution function

	return worldDir;
}

void Ray::CalculateLighting(glm::dvec3 hitPoint, std::vector<Object*> theObjects, LightSource& theLight, int iterationAmt) {
	ColorDBL directIllumination(0, 0, 0);
	ColorDBL indirectIllumination(0, 0, 0);

	if (dynamic_cast<LightSource*>((Object::theObjects[hitIndex])) != nullptr) { //If we hit a lamp return the lamp color
		RayRadianceColor = ColorDBL(1.0, 1.0, 1.0) / 8.0;
		return;
	}

	for (size_t iter = 0; iter < iterationAmt; iter++) {
		glm::dvec3 thePoint = (theLight).getRandomPoint();

		Ray newRay(thePoint, hitPoint - thePoint, ColorDBL(1, 0, 1), (theLight).radiance, 1);
		glm::dvec3 surfaceNormal = ((*Object::theObjects[hitIndex]).normal(newRay));

		double dx = (static_cast<double>(iterationAmt) * 3200.0);

		directIllumination += (RayRadianceColor * newRay.CalculateIrradiance(surfaceNormal, hitPoint, theObjects, theLight)) / dx;
		indirectIllumination = directIllumination;
	}

	if (bounces_left > 0) {
		glm::dvec3 randomDirection = getRandomDirection(Object::theObjects[hitIndex]->normal(*this));
		Ray indirectRay(hitPoint, randomDirection, RayRadianceColor, 1.0, bounces_left - 1);
		indirectRay.getPointOfIntersection(theObjects, theLight, iterationAmt);
		if (indirectRay.hitObject) {
			indirectIllumination = indirectRay.GetRayColor();
		}
	}

	RayRadianceColor.r = indirectIllumination.r;
	RayRadianceColor.g = indirectIllumination.g;
	RayRadianceColor.b = indirectIllumination.b;
}



// Calculates wether the surface is in shadow (returns 0.0) or in light (returns 1.0)
double Ray::IsVisibleToPoint(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightSourcePoint, const std::vector<Object*>& theObjects) {
	Ray shadowRay(surfaceHitPoint, randomLightSourcePoint - surfaceHitPoint, ColorDBL(1, 1, 1), 0, 0);

	double testLength = glm::distance(surfaceHitPoint, randomLightSourcePoint);

	glm::dvec3 prospectiveShadowImpact;
	glm::dvec3 intersection;
	double shadowLength = (double)INFINITY;
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

//double Ray::CalculateBRDF(glm::dvec3 thePoint, double azimuth, double inclination) {
//	double result;
//
//	return result;
//}

LocalDirection Ray::WorldCartesianToHemispherical() {
	LocalDirection result;

	result.inclination = glm::acos(direction.z / (sqrt(pow(direction.x, 2.0) + pow(direction.y, 2.0) + pow(direction.z, 2.0))));
	result.azimuth = glm::sign(direction.y) * acos(direction.x / (sqrt(pow(direction.x, 2.0) + pow(direction.y, 2.0))));

	return result;
}

double Ray::GetInclination(glm::dvec3 surfaceNormal) {
	glm::dvec3 vector = startPosition - endPosition;

	return dot(-surfaceNormal, vector) / glm::length(vector);
};