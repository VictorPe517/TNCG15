#pragma once

#include "glm/glm.hpp"
#include <iostream>

class Camera;


class RenderSettings {
public:
	void UserInputAndSettings();
	void WriteSettingsToScreen(Camera theCamera);

	glm::ivec2 s_renderingResolution = glm::ivec2(2560, 1440);
	double s_resolutionScale = 1.33; // Resolution scale multiplier

	size_t s_maxMirrorBounces = 16;
	size_t s_shadowrayIterations = 16;

	bool s_useMulticore = true;
	bool s_verboseDebugging = false;
};

