#ifndef GAME_SUNSETHOT_SUNSETHOTWINCOMBINATION_H_
#define GAME_SUNSETHOT_SUNSETHOTWINCOMBINATION_H_

#include "../core/WinCombination.h"

namespace sunsethot {

class SunsetHotWinCombination: public core::WinCombination {
public:

	int lineIndex;
	int numberOfSymbols;
	int lineWin;
	bool baseGameWin;
	bool freeGameWin;
	bool bonusGameWin;

	SunsetHotWinCombination(int lineIndex, int numberOfSymbols,
			int lineWin, bool baseGameWin, bool bingGameWin) {
		this->lineIndex = lineIndex;
		this->numberOfSymbols = numberOfSymbols;
		this->lineWin = lineWin;
		this->baseGameWin = baseGameWin;
		this->freeGameWin = freeGameWin;
		this->bonusGameWin = bonusGameWin;
	}

	virtual ~SunsetHotWinCombination() {
	}
};

}

#endif

