#include "RenderSettings.h"
#include "Camera.h"


void RenderSettings::UserInputAndSettings() {
	int res = 0;
	int scale = 0;
	int shadowIter = 0;
	int SSAAIter = 0;

	int customX = 0;
	int customY = 0;

	std::cout << "Desired resolution:\n"
		<< " Fast - 1: 640 x 360 \n"
		<< "        2: 1280 x 720 \n"
		<< "        3: 1920 x 1080 \n"
		<< "        4: 2560 x 1440 \n"
		<< " Slow - 5: 3840 x 2160 \n"
		<< "        6: Type Custom Resolution \n"
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
	case 6:
		std::cout << "\n[Enter width in pixels]: ";
		std::cin >> customX;
		std::cout << "\n[Enter height in pixels]: ";
		std::cin >> customY;

		s_renderingResolution = glm::ivec2(customX, customY);
	}

	std::cout << "\nDesired Resolution Multiplier:\n"
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


	std::cout << "\nDesired Samples per Pixel:\n"
		<< " Fast - 1: 1 \n"
		<< "        2: 8 \n"
		<< "        3: 16 \n"
		<< "        4: 32 \n"
		<< "        5: 128 \n"
		<< "        6: 256 \n"
		<< " Slow - 7: 512 \n"
		<< "[Input:] ";

	std::cin >> SSAAIter;

	switch (SSAAIter) {
	case 1:
		s_GlobalIterations = 2;
		break;
	case 2:
		s_GlobalIterations = 16;
		break;
	case 3:
		s_GlobalIterations = 32;
		break;
	case 4:
		s_GlobalIterations = 64;
		break;
	case 5:
		s_GlobalIterations = 128;
		break;
	case 6:
		s_GlobalIterations = 256;
		break;
	case 7:
		s_GlobalIterations = 512;
		break;
	}
}

size_t RenderSettings::GetAAIterations() {
	return s_GlobalIterations;
}

void RenderSettings::WriteSettingsToScreen(Camera theCamera) {
	std::cout << "/=========[ Current Settings ]=========\\" << std::endl
		<< "  Resolution:" << std::endl
		<< "    " << s_renderingResolution.x << " x " << s_renderingResolution.y <<
		" [x" << s_resolutionScale << "] --> ( " << theCamera.GetResX() << " x " << theCamera.GetResY() << " ) " << std::endl

		<< "  Iterations Per Pixel:" << std::endl
		<< "    " << s_GlobalIterations << std::endl
		<< "\\======================================/" << std::endl;
}

int RenderSettings::GetTotalIterations()
{
	return (int)round(s_GlobalIterations);
}
