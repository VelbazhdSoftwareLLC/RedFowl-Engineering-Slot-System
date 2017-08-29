#ifndef GAME_TROPICALHOT_TROPICALHOTWINCOMBINATION_H_
#define GAME_TROPICALHOT_TROPICALHOTWINCOMBINATION_H_

#include "../core/WinCombination.h"

namespace tropicalhot {

class TropicalHotWinCombination: public core::WinCombination {
public:

	int lineIndex;
	int numberOfSymbols;
	int lineWin;
	bool baseGameWin;
	bool freeGameWin;
	bool bonusGameWin;

	TropicalHotWinCombination(int lineIndex, int numberOfSymbols, int lineWin,
			bool baseGameWin, bool bingGameWin) {
		this->lineIndex = lineIndex;
		this->numberOfSymbols = numberOfSymbols;
		this->lineWin = lineWin;
		this->baseGameWin = baseGameWin;
		this->freeGameWin = freeGameWin;
		this->bonusGameWin = bonusGameWin;
	}

	virtual ~TropicalHotWinCombination() {
	}
};

}

#endif

