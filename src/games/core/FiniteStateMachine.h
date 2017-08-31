#ifndef GAME_CORE_SLOTFINITESTATEMACHINE_H_
#define GAME_CORE_SLOTFINITESTATEMACHINE_H_

#include <vector>

#include <SDL2/SDL.h>

namespace core {

using namespace std;

/**
 * Finte state machine for the game engine.
 */
class FiniteStateMachine {

public:

	/**
	 * Last time for spin tick.
	 */
	static Uint32 lastSpinTick;

	/**
	 * Reels spinning mode flag.
	 */
	static vector<bool> reelsSpinFlag;

public:

	/**
	 * Spint reset.
	 */
	static void reset() {
		for (int i = 0; i < reelsSpinFlag.size(); i++) {
			reelsSpinFlag[i] = false;
		}
	}

	/**
	 * Put all reels in spining mode.
	 */
	static void spinAllReels() {
		for (int i = 0; i < reelsSpinFlag.size(); i++) {
			reelsSpinFlag[i] = true;
		}
	}

	/**
	 * Put all reels in stop mode.
	 */
	static bool areAllReelsStoped() {
		for (int i = 0; i < reelsSpinFlag.size(); i++) {
			if (reelsSpinFlag[i] == true) {
				return (false);
			}
		}

		return (true);
	}

};

}

#endif
