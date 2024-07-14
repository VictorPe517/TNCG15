#include "RenderSettings.h"
#include "Camera.h"


void RenderSettings::UserInputAndSettings() {
	int res = 0;
	int scale = 0;
	int shadowIter = 0;
	int SSAAIter = 0;

	std::cout << "Desired resolution:\n"
		<< " Fast - 1: 640 x 360 \n"
		<< "        2: 1280 x 720 \n"
		<< "        3: 1920 x 1080 \n"
		<< "        4: 2560 x 1440 \n"
		<< " Slow - 5: 3840 x 2160 \n"
		<< "[Input:] ";

	std::cin >> res;

	switch (res) {
	case 1:
		s_renderingResolution = glm::ivec2(640, 360);
		break;
	case 2:
		s_renderingResolution = glm::ivec2(1280, 720);
		break;
	case 3:
		s_renderingResolution = glm::ivec2(1920, 1080);
		break;
	case 4:
		s_renderingResolution = glm::ivec2(2560, 1440);
		break;
	case 5:
		s_renderingResolution = glm::ivec2(3840, 2160);
		break;
	}

	std::cout << "\nDesired resolution multiplier:\n"
		<< " Fast - 1: 0.25x \n"
		<< "        2: 0.50x \n"
		<< "        3: 0.75x \n"
		<< "        4: 1.0x \n"
		<< "        5: 1.5x \n"
		<< "        6: 2.0x \n"
		<< " Slow - 7: 4.0x \n"
		<< "[Input:] ";

	std::cin >> scale;

	switch (scale) {
	case 1:
		s_resolutionScale = 0.25;
		break;
	case 2:
		s_resolutionScale = 0.50;
		break;
	case 3:
		s_resolutionScale = 0.75;
		break;
	case 4:
		s_resolutionScale = 1.0;
		break;
	case 5:
		s_resolutionScale = 1.5;
		break;
	case 6:
		s_resolutionScale = 2.0;
		break;
	case 7:
		s_resolutionScale = 4.0;
		break;
	}

	std::cout << "\nDesired shadow ray samples per pixel:\n"
		<< " Fast - 1: 8 \n"
		<< "        2: 16 \n"
		<< "        3: 64 \n"
		<< "        4: 256 \n"
		<< "        5: 512 \n"
		<< "        6: 1024 \n"
		<< " Slow - 7: 2048 \n"
		<< "[Input:] ";

	std::cin >> shadowIter;

	switch (shadowIter) {
	case 1:
		s_shadowrayIterations = 8;
		break;
	case 2:
		s_shadowrayIterations = 16;
		break;
	case 3:
		s_shadowrayIterations = 64;
		break;
	case 4:
		s_shadowrayIterations = 256;
		break;
	case 5:
		s_shadowrayIterations = 512;
		break;
	case 6:
		s_shadowrayIterations = 1024;
		break;
	case 7:
		s_shadowrayIterations = 2048;
		break;
	}

	std::cout << "\nDesired Super-Sampling Anti-Aliasing amples:\n"
		<< " Fast - 1: Off \n"
		<< "        2: 2 \n"
		<< "        3: 4 \n"
		<< "        4: 8 \n"
		<< "        5: 16 \n"
		<< " Slow - 6: 32 \n"
		<< "[Input:] ";

	std::cin >> SSAAIter;

	switch (SSAAIter) {
	case 1:
		s_SSAAiterations = 1;
		break;
	case 2:
		s_SSAAiterations = 2;
		break;
	case 3:
		s_SSAAiterations = 4;
		break;
	case 4:
		s_SSAAiterations = 8;
		break;
	case 5:
		s_SSAAiterations = 16;
		break;
	case 6:
		s_SSAAiterations = 32;
		break;
	}
}

size_t RenderSettings::GetAAIterations() {
	return s_SSAAiterations;
}

void RenderSettings::WriteSettingsToScreen(Camera theCamera) {
	std::cout << "/=========[ Current Settings ]=========\\" << std::endl
		<< "  Resolution:" << std::endl
		<< "    " << s_renderingResolution.x << " x " << s_renderingResolution.y <<
		" [x" << s_resolutionScale << "] --> ( " << theCamera.GetResX() << " x " << theCamera.GetResY() << " ) " << std::endl

		<< "  Shadowray Iterations per pixel:" << std::endl
		<< "    " << s_shadowrayIterations << std::endl
		<< "  Max recursive mirror bounces:" << std::endl
		<< "    " << s_maxMirrorBounces << std::endl
		<< "\\======================================/" << std::endl;
}

int RenderSettings::GetTotalIterations()
{
	return (int)round(s_shadowrayIterations / s_SSAAiterations);
}
