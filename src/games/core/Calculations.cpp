#include <cstdlib>

#include "CommonState.h"

#include "Calculations.h"

namespace core {

/**
 * Generate decorative view for initial screen.
 */
void generatDecorative(const std::vector<std::vector<int> > &reels) {
	static int index = 0;
	for (int i = 0; i < view.size(); i++) {
		for (int j = view[i].size() - 1; j >= 0; j--) {
			index = (reelsStops[i] + j) % reels[i].size();
			view[i][j] = reels[i][index];
		}
	}
}

/**
 * Single reels spin to fill view with symbols. Each reel stops on particular random position.
 *
 * @param view Slot screen symbols as output parameter.
 *
 * @param reels Reels strips as input parameter.
 */
void spin(std::vector<std::vector<int> > &view,
		const std::vector<std::vector<int> > &reels,
		const std::vector<int> &reelsMinOffset,
		const std::vector<int> &reelsMaxOffset) {
	/*
	 * Static variables are not allocated into stack and it is faster to be used.
	 */
	static int index;
	for (int i = 0; i < view.size(); i++) {
		for (int j = view[i].size() - 1; j >= 0; j--) {
			reelsStops[i] += reelsMinOffset[i]
					+ rand() % (reelsMaxOffset[i] - reelsMinOffset[i] + 1);

			index = (reelsStops[i] + j) % reels[i].size();
			view[i][j] = reels[i][index];
		}
	}
}

}
