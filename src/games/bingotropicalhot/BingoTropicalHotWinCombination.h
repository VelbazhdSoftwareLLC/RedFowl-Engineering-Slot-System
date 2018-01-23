#ifndef GAME_BINGOTROPICALHOT_BINGOTROPICALHOTWINCOMBINATION_H_
#define GAME_BINGOTROPICALHOT_BINGOTROPICALHOTWINCOMBINATION_H_

#include "../core/WinCombination.h"

namespace bingotropicalhot {

class BingoTropicalHotWinCombination: public core::WinCombination {
public:

	int lineIndex;
	int numberOfSymbols;
	int lineWin;
	bool baseGameWin;
	bool bingGameWin;

	BingoTropicalHotWinCombination(int lineIndex, int numberOfSymbols,
								   int lineWin, bool baseGameWin, bool bingGameWin) {
		this->lineIndex = lineIndex;
		this->numberOfSymbols = numberOfSymbols;
		this->lineWin = lineWin;
		this->baseGameWin = baseGameWin;
		this->bingGameWin = bingGameWin;
	}

	virtual ~BingoTropicalHotWinCombination() {
	}
};

}

#endif

