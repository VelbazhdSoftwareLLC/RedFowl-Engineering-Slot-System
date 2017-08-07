#ifndef GAME_BINGOSTEAMHOT_BINGOSTEAMHOTWINCOMBINATION_H_
#define GAME_BINGOSTEAMHOT_BINGOSTEAMHOTWINCOMBINATION_H_

#include "../core/WinCombination.h"

namespace bingosteamhot {

class BingoSteamHotWinCombination: public core::WinCombination {
public:

	int lineIndex;
	int numberOfSymbols;
	int lineWin;
	bool baseGameWin;
	bool bingGameWin;

	BingoSteamHotWinCombination(int lineIndex, int numberOfSymbols,
			int lineWin, bool baseGameWin, bool bingGameWin) {
		this->lineIndex = lineIndex;
		this->numberOfSymbols = numberOfSymbols;
		this->lineWin = lineWin;
		this->baseGameWin = baseGameWin;
		this->bingGameWin = bingGameWin;
	}

	virtual ~BingoSteamHotWinCombination() {
	}
};

}

#endif

