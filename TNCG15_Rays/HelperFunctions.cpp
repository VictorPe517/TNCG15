#include "HelperFunctions.h"

// Displays a Loading bar and its current progress
void HelperFunctions::DisplayLoadingBar(int& rowsDone, const int& x_res) {

	rowsDone++;

	if ((int)floor((((double)x_res) / 50.0)) != 0 && rowsDone % (int)floor((((double)x_res) / 50.0)) == 0) {
		int percentDone = (int)floor(((double)rowsDone / (double)x_res) * 100.0);
		std::cout << "\r [";

		for (int i = 0; i < (int)(floor((float)percentDone / 4.0)); i++) {
			std::cout << "I";
		}

		for (int i = 0; i < 50 - (int)(floor((float)percentDone / 4.0)); i++) {
			std::cout << "-";
		}

		std::cout << "] : ";

		std::cout << (int)floor(((double)rowsDone / (double)x_res) * 50.0) << "% \r";

		std::cout.flush();
	}
}