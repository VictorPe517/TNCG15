#pragma once

#include "glm/glm.hpp"
#include <iostream>

class Camera;


class RenderSettings {
public:
	void UserInputAndSettings();
	void WriteSettingsToScreen(Camera theCamera);
	int GetTotalIterations();

	size_t GetAAIterations();

	glm::ivec2 s_renderingResolution = glm::ivec2(2560, 1440);
	double s_resolutionScale = 1.33; // Resolution scale multiplier

	size_t s_GlobalIterations = 2; // off = 1;

	bool s_useMulticore = true;
	bool s_useAntiAliasing = true;
	bool s_verboseDebugging = false;
};

