#ifndef GAMES_CORE_SELECTTHREEBONUSGAME_H_
#define GAMES_CORE_SELECTTHREEBONUSGAME_H_

namespace core {

class SelectThreeBonusGame {

public:

	/**
	 * Select one of three bounus.
	 */
	static const int BONUS_SELECT_NUBMER = 3;

	/**
	 * Index of the reel with the bonus selected by the user.
	 */
	static int reelBonusIndex;

	/**
	 * User can select from three options.
	 */
	static int bonusOptions[BONUS_SELECT_NUBMER];

};

}

#endif
