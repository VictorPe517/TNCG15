#include "Ray.h"

int counter = 0;

void Ray::SetRayColor(ColorDBL theColor) {
	RayRadianceColor = theColor;
}

ColorDBL Ray::GetRayColor() {
	return RayRadianceColor;
}

ColorDBL Ray::CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLights,const int& hitIndex) {
	double E = 0.0;
	ColorDBL irradiance = ColorDBL(0, 0, 0);

	// Todo: iterate over all lamps here
	for (LightSource* theLight : theLights) {
		glm::dvec3 randomPointOnLamp = theLight->getRandomPoint();
		glm::dvec3 lampToPointVec = randomPointOnLamp - intersectionPoint;
		glm::dvec3 lightNormal = theLight->normal(*this);
		double area = theLight->area;
		ColorDBL theLightColor = theLight->Color;
		ColorDBL surfaceColor = theObjects[hitIndex]->getColor();

		double distance = glm::length(lampToPointVec);
		double cos_omega_x = glm::clamp(glm::dot(glm::normalize(surfaceNormal), lampToPointVec) / distance, 0.0, (double)INFINITY);
		double cos_omega_y = -1.0 * glm::dot(lightNormal, lampToPointVec) / distance;

		if (cos_omega_y < 0.0) cos_omega_y = 0.0;

		double G = (cos_omega_x * cos_omega_y) / (distance * distance);
		double shadowVar = IsVisibleToPoint(intersectionPoint, randomPointOnLamp, theObjects);

		E = area * G * shadowVar * theLight->Watt / std::numbers::pi;

		irradiance += surfaceColor * theLightColor * E * (theObjects[hitIndex]->getMaterial().radiosity / std::numbers::pi);
	}

	//std::cout << "E is: " << E << " ; which gives an irradiance of: " << irradiance.ToString();

	return irradiance;
};

std::string Ray::GetAllNonHittersFormatted()
{
	return std::string();
}

Ray::~Ray() {
	delete nextRay;
	//Ray* current = this;  // Start from the current Ray (this)
	//while (current != nullptr) {
	//	Ray* next = current->nextRay;  // Store the next Ray before deleting
	//	delete current;                // Delete the current Ray
	//	current = next;                // Move to the next Ray
	//}
}


bool Ray::DrawRandom() {

	return (double)rand() > (double)RAND_MAX / 2.0;
}

double Ray::DrawRandomNormalized() {
	return (double)rand() / (double)RAND_MAX;
}

glm::dvec3 Ray::getRefractedDirection(const glm::dvec3& intersection, const glm::dvec3& surfaceNormal, const Object& theObject, double ior) {
	glm::dvec3 d0 = glm::normalize(this->direction);
	glm::dvec3 n = glm::normalize(surfaceNormal);

	double eta = ior;
	double nDot = glm::dot(-d0, n);

	double k = 1.0 - eta * eta * (1.0 - nDot * nDot);

	if (k < 0.0) {
		std::cout << "Total Internal Reflection occured. \n";
		return glm::dvec3(NAN,NAN,NAN);
	}

	// Calculate the refracted direction using Snell's law
	glm::dvec3 refractedDir = eta * d0 + (eta * nDot - sqrt(k)) * n;

	return glm::normalize(refractedDir);

}

glm::dvec3 Ray::getReflectedDirection(const glm::dvec3& surfaceNormal) {
	glm::dvec3 dir;

	dir = direction - 2.0 * (glm::dot(direction, glm::normalize(surfaceNormal)) * glm::normalize(surfaceNormal));

	return glm::normalize(dir);
}

LocalDirection Ray::getRandomLocalDirection() // Returns random direction using the cumulative distribution function
{
	LocalDirection result;

	double azimuth = (2.0 * std::numbers::pi) * ((double)rand()) / (((double)RAND_MAX));
	double inclination = ((double)rand()) / (((double)RAND_MAX));

	

	result.azimuth = azimuth;
	result.inclination = acos(sqrt(1.0 - inclination));

	//std::cout << "  Azimuth: " << result.azimuth * 57.2957795 << "\n  Inclination: " << result.inclination*57.2957795 << "\n\n";

	return result;
}


glm::dvec3 Ray::localCartesianToWorldCartesian(const glm::dvec3& localDir, const glm::dvec3& surfaceNormal)
{
	// Local system in the local system: x = ( 1, 0, 0 ), y = ( 0, 1, 0 ), z = ( 0, 0, 1 )
	// Local system   in  global system: x = (a1,a2,a3 ), y = (b1,b2,b3 ), z = (c1,c2,c3 )

	double x_0 = localDir.x;
	double y_0 = localDir.y;
	double z_0 = localDir.z;

	glm::dvec3 z_l = glm::normalize(surfaceNormal);

	glm::dvec3 arbitraryVec = (glm::abs(z_l.x) > 0.99) ? glm::dvec3(0.0, 1.0, 0.0) : glm::dvec3(1.0, 0.0, 0.0);

	//glm::normalize(-1.0 * localDir + glm::dot(surfaceNormal, localDir) * surfaceNormal);
	//glm::dvec3 x_l = glm::dvec3(1.0, 0.0, 0.0);
	glm::dvec3 x_l = glm::normalize(glm::cross(arbitraryVec, z_l));
	glm::dvec3 y_l = glm::cross(z_l, x_l);

	glm::dvec3 res(x_0 * x_l.x + y_0 * y_l.x + z_0 * z_l.x,
		x_0 * x_l.y + y_0 * y_l.y + z_0 * z_l.y,
		x_0 * x_l.z + y_0 * y_l.z + z_0 * z_l.z);

	return glm::normalize(res);
}

glm::dvec3 Ray::hemisphericalToCartesian(const LocalDirection& dir)
{
	return glm::dvec3(cos(dir.azimuth) * sin(dir.inclination), sin(dir.azimuth) * sin(dir.inclination), cos(dir.inclination));
}

glm::dvec3 Ray::getRandomDirection(const glm::dvec3& surfaceNormal)
{
	double twoPi = 2.0 * std::numbers::pi;

	LocalDirection dir = getRandomLocalDirection();

	glm::dvec3 localDir = hemisphericalToCartesian(dir);
	glm::dvec3 worldDir = glm::normalize(localCartesianToWorldCartesian(localDir, surfaceNormal));

	//std::cout << glm::to_string(worldDir) << "\n";

	if (glm::dot(worldDir, surfaceNormal) < 0.0f) worldDir = -1.0 * worldDir; //feels superflous with cumulative distribution function

	return worldDir;
}

void Ray::CalculateRayPath(const std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLights)
{
	glm::dvec3 intersection = glm::dvec3(0, 0, 0);
	glm::dvec3 possibleIntersection = glm::dvec3(0.0, 0.0, 0.0);
	glm::dvec3 theObjectNormal = glm::dvec3(0.0, 0.0, 0.0);

	RayRadianceColor = ColorDBL(0, 0, 0);
	double minLength = (double)INFINITY;
	double lengthStartIntersection = (double)INFINITY;
	direction = glm::normalize(direction);

	// Find the closest object in the direction of the ray
	for (int l = 0; l < theObjects.size(); l++) {
		//lengthStartIntersection = 0.0;
		possibleIntersection = (theObjects[l])->getIntersection(*this);
		lengthStartIntersection = glm::length(possibleIntersection - startPosition);

		if (lengthStartIntersection < minLength) {
			theObjectNormal = (*theObjects[l]).normal(*this);
			this->hitObject = theObjects[l];
			minLength = glm::length(possibleIntersection - startPosition);
			intersection = possibleIntersection;

			this->hitIndex = l;
			endPosition = intersection;
		}

	}

	if (hitIndex == -1) {
		nonHitters++;

		hitIndex = 0; // Why on earth do we have to do this i don't understand how we're NOT hitting something...
		//std::cout << "Ray with start: " << glm::to_string(startPosition) << 
		//		   "\n     Direction: " << glm::to_string(direction) << ", hit nothing.\n\n";
		//std::cout << "Negative one detected!\n";
	}


	if (minLength != INFINITY && hitIndex != -1 && !glm::any(glm::isnan(intersection))) {
		if (theObjects[hitIndex]->getMaterial().isMirror) {
			//---------------[ MIRROR ]---------------//
			this->nextRay = new Ray(intersection, getReflectedDirection(theObjectNormal), RayRadianceColor);
			this->nextRay->startSurface = theObjects[hitIndex];
			this->nextRay->prevRay = this;
			this->nextRay->CalculateRayPath(theObjects, theLights);
		}
		else if (theObjects[hitIndex]->getMaterial().isTransparent) {
			//-------------[ TRANSPARENT ]------------//
			double airIndex = 1.0;
			double matIndex = (theObjects[hitIndex]->getMaterial().refractiveIndex);

			glm::dvec3 geometricNormal = theObjectNormal;
			glm::dvec3 adjustedNormal = glm::dot(direction, theObjectNormal) < 0.0 ? theObjectNormal : theObjectNormal * -1.0;

			bool isEntering = glm::dot(geometricNormal, adjustedNormal) > 0.0;
			
			double refractRatio;
			double cosIncidenceAngle = glm::dot(direction, adjustedNormal);

			if (isEntering) {
				refractRatio = airIndex / matIndex;
			}
			else {
				refractRatio = matIndex / airIndex;

			}

			double refractDiff = matIndex - airIndex;
			double refractSum = matIndex + airIndex;
			double R0 = (refractDiff * refractDiff) / (refractSum * refractSum); // Reflectance at normal incidence

			glm::dvec3 refractionDir = getRefractedDirection(intersection, adjustedNormal, (*theObjects[hitIndex]), refractRatio);
			glm::dvec3 reflectionDir = getReflectedDirection(adjustedNormal);

			double cosOutgoingAngle = 0.0;
			if (isEntering) {
				cosOutgoingAngle = glm::abs(cosIncidenceAngle);
			}
			else {
				cosOutgoingAngle = glm::abs(glm::dot(refractionDir, adjustedNormal));
			}

			double Reflectance = (R0 + (1.0 - R0) * pow((1.0 - cosOutgoingAngle), 5.0));
			double Transmittance = 1.0 - Reflectance;

			glm::dvec3 newDirection = glm::dvec3(0, 0, 0);

			double rand = DrawRandomNormalized();
			if (rand < Reflectance) {
				newDirection = reflectionDir;
				//std::cout << "REFLECTED!\n";
			}
			else {
				newDirection = refractionDir;
				if (glm::any(glm::isnan(newDirection))) {
					newDirection = reflectionDir;
					std::cout << "It's NAN, setting reflection instead.\n\n";
				}
	
			}

			newDirection = glm::normalize(newDirection);
			if (!glm::any(glm::isnan(newDirection))) {
				nextRay = new Ray(intersection, newDirection, RayRadianceColor);
				nextRay->startSurface = theObjects[hitIndex];
				nextRay->prevRay = this;

				nextRay->CalculateRayPath(theObjects, theLights);
			}
			else {
				std::cout << "Encountered NAN\n";
			}
			
		}
		else {
			//-------------[ LAMBERTIAN ]-------------//

			LocalDirection newRandomDirection = getRandomLocalDirection();

			double continueNum = DrawRandom() ? 1.0 : 0.0;

			
			if (newRandomDirection.azimuth <= 2.0 * std::numbers::pi * continueNum) {
				glm::dvec3 localDir = hemisphericalToCartesian(newRandomDirection);
				glm::dvec3 theDirection = localCartesianToWorldCartesian(localDir, theObjectNormal);

				this->nextRay = new Ray(intersection, theDirection, RayRadianceColor);
				this->nextRay->prevRay = this;
				this->nextRay->CalculateRayPath(theObjects, theLights);
			}
		}
	}
}

// Calculates wether the surface is in shadow (returns 0.0) or in light (returns 1.0)
double Ray::IsVisibleToPoint(const glm::dvec3& surfaceHitPoint, const glm::dvec3& randomLightSourcePoint, const std::vector<Object*>& theObjects) {
	Ray shadowRay(surfaceHitPoint, randomLightSourcePoint - surfaceHitPoint, ColorDBL(1, 1, 1));

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

LocalDirection Ray::WorldCartesianToHemispherical(glm::dvec3& direction) {
	LocalDirection result;

	result.inclination = glm::acos(direction.z / (sqrt(pow(direction.x, 2.0) + pow(direction.y, 2.0) + pow(direction.z, 2.0))));
	result.azimuth = glm::sign(direction.y) * acos(direction.x / (sqrt(pow(direction.x, 2.0) + pow(direction.y, 2.0))));

	return result;
}

double Ray::GetInclination(const glm::dvec3& surfaceNormal) {
	//LocalDirection localDir = this->WorldCartesianToHemispherical();
	//glm::dvec3 cartesianLocal = hemisphericalToCartesian(localDir);
	//glm::dvec3 dir = localCartesianToWorldCartesian(cartesianLocal, surfaceNormal);
	//localDir = WorldCartesianToHemispherical(dir);
		// Normalize the ray direction and surface normal
	glm::dvec3 normalizedRayDir = glm::normalize(this->direction);
	glm::dvec3 normalizedSurfaceNormal = glm::normalize(surfaceNormal);

	double cosInclination = glm::dot(normalizedRayDir, normalizedSurfaceNormal);
	cosInclination = glm::clamp(cosInclination, -1.0, 1.0); //This seems off but whatever

	return acos(cosInclination);
};

void Ray::CalculateRadianceFlow(std::vector<Object*>& theObjects, const std::vector<LightSource*>& theLights) {
	bool useOldFeedRadiance = false;
	Ray* traversalPointer = this;

	// Traverse to end of ray list
	while (traversalPointer->nextRay != nullptr) {
		traversalPointer = traversalPointer->nextRay;
	}

	Object* hitObject = theObjects[traversalPointer->hitIndex];
	LightSource* potentialLight = dynamic_cast<LightSource*>(theObjects[traversalPointer->hitIndex]);

	// At the endpoint:
	if (potentialLight != nullptr) {
		// If endpoint lightsource: feed radiance into ray
		if (useOldFeedRadiance) {
			traversalPointer->RayRadianceColor = potentialLight->getColor(); //Old, dont know if this is right
		}else{
			ColorDBL tempColor = potentialLight->getColor();
			double lightRadiance = potentialLight->Watt / (potentialLight->area * std::numbers::pi);
			traversalPointer->RayRadianceColor = tempColor * lightRadiance;
		}
	}
	else if (!hitObject->getMaterial().isMirror && !hitObject->getMaterial().isTransparent) {
		// Else it's lambertian: Compute direct light and feed that radiance into ray
		ColorDBL directLightRadiance = traversalPointer->CalculateIrradiance(hitObject->normal(*traversalPointer), traversalPointer->endPosition, theObjects, theLights, traversalPointer->hitIndex);
		traversalPointer->RayRadianceColor = directLightRadiance;
	}

	while (traversalPointer->prevRay != nullptr) {
		traversalPointer = traversalPointer->prevRay;
		hitObject = theObjects[traversalPointer->hitIndex];
		LightSource* potentialLight = dynamic_cast<LightSource*>(theObjects[traversalPointer->hitIndex]);

		if (potentialLight != nullptr) {
			// If lightsource

			if (useOldFeedRadiance) {
				traversalPointer->RayRadianceColor = potentialLight->getColor(); //Old, dont know if this is right
			}
			else {
				ColorDBL tempColor = potentialLight->getColor();
				double lightRadiance = potentialLight->Watt / (potentialLight->area * std::numbers::pi);
				traversalPointer->RayRadianceColor = tempColor * lightRadiance;
			}
		}
		else if (hitObject->getMaterial().isMirror || hitObject->getMaterial().isTransparent) { //IF mirror // transparent : copy radiance from previous (traversalPointer->nextRay) into this ray
			traversalPointer->RayRadianceColor = traversalPointer->nextRay->RayRadianceColor;

		}
		else {
			ColorDBL directLightRadiance = traversalPointer->CalculateIrradiance(hitObject->normal(*traversalPointer), traversalPointer->endPosition, theObjects, theLights, traversalPointer->hitIndex);
			ColorDBL prevRadianceSurfaceColor = theObjects[traversalPointer->hitIndex]->getColor() * traversalPointer->nextRay->RayRadianceColor;
			//traversalPointer->nextRay->hitIndex

			directLightRadiance += prevRadianceSurfaceColor;
			traversalPointer->RayRadianceColor = directLightRadiance;
		}
	}
}