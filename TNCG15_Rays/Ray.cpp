#include "Ray.h"

int counter = 0;

void Ray::SetRayColor(ColorDBL theColor) {
	RayRadianceColor = theColor;
}

ColorDBL Ray::GetRayColor() {
	return RayRadianceColor;
}

ColorDBL Ray::CalculateIrradiance(const glm::dvec3& surfaceNormal, const glm::dvec3& intersectionPoint, const std::vector<Object*>& theObjects,LightSource& theLight,const int& hitIndex) {
	double E = 0.0;
	ColorDBL irradiance = ColorDBL(0, 0, 0);

	// Todo: iterate over all lamps here
	glm::dvec3 randomPointOnLamp = theLight.getRandomPoint();
	glm::dvec3 lampToPointVec = randomPointOnLamp - intersectionPoint;
	glm::dvec3 lightNormal = theLight.normal(*this);
	double area = theLight.area;
	ColorDBL theLightColor = theLight.Color;
	ColorDBL surfaceColor = theObjects[hitIndex]->getColor();

	double distance = glm::length(lampToPointVec);
	double cos_omega_x = glm::clamp(glm::dot(glm::normalize(surfaceNormal), lampToPointVec) / distance, 0.0, (double)INFINITY);
	double cos_omega_y = -1.0 * glm::dot(lightNormal, lampToPointVec) / distance;

	if (cos_omega_y < 0.0) cos_omega_y = 0.0;

	double G = (cos_omega_x * cos_omega_y) / (distance * distance);
	double shadowVar = IsVisibleToPoint(intersectionPoint, randomPointOnLamp, theObjects);

	E = area * G * shadowVar * theLight.Watt / std::numbers::pi;

	irradiance = surfaceColor * E * (theObjects[hitIndex]->getMaterial().radiosity / std::numbers::pi);

	//std::cout << "E is: " << E << " ; which gives an irradiance of: " << irradiance.ToString();

	return irradiance;
};

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

glm::dvec3 Ray::getRefractedDirection(const glm::dvec3& intersection, const glm::dvec3& surfaceNormal, const Object& theObject, double n1, double n2) {
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

glm::dvec3 Ray::getReflectedDirection(glm::dvec3& surfaceNormal) {
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

	return result;
}


glm::dvec3 Ray::localCartesianToWorldCartesian(const glm::dvec3& localDir, const glm::dvec3& surfaceNormal)
{
	// Local system in the local system: x = ( 1, 0, 0 ), y = ( 0, 1, 0 ), z = ( 0, 0, 1 )
	// Local system   in  global system: x = (a1,a2,a3 ), y = (b1,b2,b3 ), z = (c1,c2,c3 )

	double x_0 = localDir.x;
	double y_0 = localDir.y;
	double z_0 = localDir.z;

	glm::dvec3 z_l = surfaceNormal;

	//glm::normalize(-1.0 * localDir + glm::dot(surfaceNormal, localDir) * surfaceNormal);
	glm::dvec3 x_l = glm::dvec3(1.0, 0.0, 0.0);
	glm::dvec3 y_l = glm::cross(surfaceNormal, x_l);

	glm::dvec3 res(x_0 * x_l.x + y_0 * y_l.x + z_0 * z_l.x,
		x_0 * x_l.y + y_0 * y_l.y + z_0 * z_l.y,
		x_0 * x_l.z + y_0 * y_l.z + z_0 * z_l.z);

	return res;
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

	std::cout << glm::to_string(worldDir) << "\n";

	if (glm::dot(worldDir, surfaceNormal) < 0.0f) worldDir = -1.0 * worldDir; //feels superflous with cumulative distribution function

	return worldDir;
}

void Ray::CalculateRayPath(const std::vector<Object*>& theObjects, const LightSource& theLight)
{
	glm::dvec3 intersection = glm::dvec3(0, 0, 0);
	glm::dvec3 possibleIntersection = glm::dvec3(0.0, 0.0, 0.0);
	glm::dvec3 theObjectNormal = glm::dvec3(0.0, 0.0, 0.0);

	RayRadianceColor = ColorDBL(0, 0, 0);
	double minLength = (double)INFINITY;
	double lengthStartIntersection = (double)INFINITY;
	//std::cout << "current Dir outside func: " << glm::to_string(this->direction) << "\n";

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
		hitIndex = 0;
		//std::cout << "Negative one detected!\n";
	}


	if (minLength != INFINITY && hitIndex != -1) {
		if (theObjects[hitIndex]->getMaterial().isMirror) {
			//---------------[ MIRROR ]---------------//
			this->nextRay = new Ray(intersection, getReflectedDirection(theObjectNormal), RayRadianceColor);
			this->nextRay->startSurface = theObjects[hitIndex];
			this->nextRay->prevRay = this;
			this->nextRay->CalculateRayPath(theObjects, theLight);
		}
		else if (theObjects[hitIndex]->getMaterial().isTransparent) {
			//-------------[ TRANSPARENT ]------------//



			double n1;
			double n2;

			if (isInside) {
				n1 = (theObjects[hitIndex]->getMaterial().refractiveIndex);
				n2 = 1.0;
			}
			else {
				n1 = 1.0;
				n2 = (theObjects[hitIndex]->getMaterial().refractiveIndex);
			}

			double ior = n1 / n2;

			double R0 = pow((n1 - n2) / (n1 + n2), 2.0);
			double inclination = this->GetInclination(theObjectNormal);

			//std::cout << inclination << "\n";
			double Reflectance = (R0 + (1.0 - R0) * pow((1.0 - cos(inclination)), 5.0)) * 10.0;

			double Transmittance = 1.0 - Reflectance;

			glm::dvec3 newDirection = glm::dvec3(0, 0, 0);
			bool isReflected = false;

			double rand = DrawRandomNormalized();

			//std::cout << "Reflectance: " << Reflectance << ", rand: " << rand << "\n";

			if (rand < Reflectance) {
				newDirection = getRefractedDirection(intersection, theObjectNormal, (*theObjects[hitIndex]), n1, n2);

				isReflected = false;
			}
			else {
				newDirection = getReflectedDirection(theObjectNormal);
				isReflected = true;

			}

			if (newDirection != glm::dvec3(NAN, NAN, NAN)) {
				this->nextRay = new Ray(intersection, newDirection, RayRadianceColor);
				this->nextRay->startSurface = theObjects[hitIndex];
				if (!isReflected) this->nextRay->isInside = !this->isInside;
				this->nextRay->prevRay = this;
				this->nextRay->CalculateRayPath(theObjects, theLight);
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
				this->nextRay->CalculateRayPath(theObjects, theLight);
			}
		}
	}





}


void Ray::ToString() {
	std::cout << this->Irradiance;
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
	LocalDirection localDir = this->WorldCartesianToHemispherical();
	glm::dvec3 cartesianLocal = hemisphericalToCartesian(localDir);
	glm::dvec3 dir = localCartesianToWorldCartesian(cartesianLocal, surfaceNormal);
	localDir = WorldCartesianToHemispherical(dir);

	return localDir.inclination;
};

void Ray::CalculateRadianceFlow(std::vector<Object*>& theObjects, LightSource& theLight) {
	Ray* traversalPointer = this;

	// Traverse to end of ray list
	while (traversalPointer->nextRay != nullptr) {
		traversalPointer = traversalPointer->nextRay;
	}

	Object* hitObject = theObjects[traversalPointer->hitIndex];

	// At the endpoint:
	if (dynamic_cast<LightSource*>(theObjects[traversalPointer->hitIndex]) != nullptr) {
		// If endpoint lightsource: feed radiance into ray
		traversalPointer->RayRadianceColor = ColorDBL(1.0, 1.0, 1.0);
	}
	else if (!hitObject->getMaterial().isMirror && !hitObject->getMaterial().isTransparent) {
		// Else it's lambertian: Compute direct light and feed that radiance into ray
		ColorDBL directLightRadiance = traversalPointer->CalculateIrradiance(hitObject->normal(*traversalPointer), traversalPointer->endPosition, theObjects, theLight, traversalPointer->hitIndex);
		traversalPointer->RayRadianceColor = directLightRadiance;
	}
	else {
		if (hitObject->getMaterial().isTransparent) {
			std::cout << "ITS FUCKING FUUUCKED FUUUUUCK\n";
		}
	}

	int index = 0;

	while (traversalPointer->prevRay != nullptr) {
		traversalPointer = traversalPointer->prevRay;
		hitObject = theObjects[traversalPointer->hitIndex];

		if (dynamic_cast<LightSource*>(theObjects[traversalPointer->hitIndex]) != nullptr) {
			// If lightsource
			traversalPointer->RayRadianceColor = ColorDBL(1.0, 1.0, 1.0);
		}
		else if (hitObject->getMaterial().isMirror || hitObject->getMaterial().isTransparent) { //IF mirror // transparent : copy radiance from previous (traversalPointer->nextRay) into this ray
			traversalPointer->RayRadianceColor = traversalPointer->nextRay->RayRadianceColor;

		}
		else {
			ColorDBL directLightRadiance = traversalPointer->CalculateIrradiance(hitObject->normal(*traversalPointer), traversalPointer->endPosition, theObjects, theLight, traversalPointer->hitIndex);
			ColorDBL prevRadianceSurfaceColor = theObjects[traversalPointer->hitIndex]->getColor() * traversalPointer->nextRay->RayRadianceColor;
			//traversalPointer->nextRay->hitIndex

			directLightRadiance += prevRadianceSurfaceColor;
			traversalPointer->RayRadianceColor = directLightRadiance;
		}

		index++;
	}
}