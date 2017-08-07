#include "FiniteStateMachine.h"

namespace core {

Uint32 lastSpinTick = SDL_GetTicks();

std::vector<bool> reelsSpinFlag;

void reset() {
	for (int i = 0; i < reelsSpinFlag.size(); i++) {
		reelsSpinFlag[i] = false;
	}
}

void spinAllReels() {
	for (int i = 0; i < reelsSpinFlag.size(); i++) {
		reelsSpinFlag[i] = true;
	}
}

bool areAllReelsStoped() {
	for (int i = 0; i < reelsSpinFlag.size(); i++) {
		if (reelsSpinFlag[i] == true) {
			return (false);
		}
	}

	return (true);
}

}
