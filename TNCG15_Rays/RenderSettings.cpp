#include "RenderSettings.h"
#include "Camera.h"


void RenderSettings::UserInputAndSettings() {
	int res = 0;
	int scale = 0;

	std::cout << "Desired resolution:\n"
		<< "   1: 640 x 360 \n"
		<< "   2: 1280 x 720 \n"
		<< "   3: 1920 x 1080 \n"
		<< "   4: 2560 x 1440 \n"
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
	}

	std::cout << "\nDesired resolution multiplier:\n"
		<< "   1: 0.20 \n"
		<< "   2: 0.5 \n"
		<< "   3: 0.75 \n"
		<< "   4: 1.0 \n"
		<< "   5: 1.5 \n"
		<< "   6: 2.0 \n"
		<< "   7: 4.0 \n"
		<< "[Input:] ";

	std::cin >> scale;

	switch (scale) {
	case 1:
		s_resolutionScale = 0.20;
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