#ifndef GAMES_CORE_CALCULATIONS_H_
#define GAMES_CORE_CALCULATIONS_H_

#include <cstdlib>

#include <vector>

#include "CommonState.h"

namespace core {

using namespace std;

/**
 * Class for common calculations.
 */
class Calculations {

public:

	/**
	 * Generate decorative view for initial screen.
	 */
	static void generatDecorative(const vector<vector<int> > &reels) {
		static int index = 0;
		for (int i = 0; i < CommonState::view.size(); i++) {
			for (int j = CommonState::view[i].size() - 1; j >= 0; j--) {
				index = (CommonState::reelsStops[i] + j) % reels[i].size();
				CommonState::view[i][j] = reels[i][index];
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
	static void spin(vector<vector<int> > &view, const vector<vector<int> > &reels, const vector<int> &reelsMinOffset, const vector<int> &reelsMaxOffset) {
		/*
		 * Static variables are not allocated into stack and it is faster to be used.
		 */
		static int index;
		for (int i = 0; i < view.size(); i++) {
			for (int j = view[i].size() - 1; j >= 0; j--) {
				CommonState::reelsStops[i] += reelsMinOffset[i]
											  + rand() % (reelsMaxOffset[i] - reelsMinOffset[i] + 1);

				index = (CommonState::reelsStops[i] + j) % reels[i].size();
				view[i][j] = reels[i][index];
			}
		}
	}

};

}

#endif
