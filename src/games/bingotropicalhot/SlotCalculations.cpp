#include <ctime>
#include <cstdlib>

#include "../core/BingoBonus.h"
#include "../core/CommonState.h"
#include "../core/Persistence.h"
#include "../core/Calculations.h"

#include "SlotState.h"
#include "SlotStatistics.h"

#include "SlotCalculations.h"

namespace bingotropicalhot {

/**
 * Calculate win in particular line.
 *
 * @param line Single line as input-output parameter.
 * @param index Current line index.
 *
 * @return Calculated win.
 */
static int lineWin(int line[REELS_LENGTH], const int &index) {
	/*
	 * Static variables are not allocated into stack and it is faster to be used.
	 */
	static int i;

	/*
	 * Keep first symbol in the line.
	 */
	static int symbol;
	symbol = line[0];

	/*
	 * Count symbols in winning line.
	 */
	static int number;
	number = 0;
	for (i = 0; i < REELS_LENGTH; i++) {
		if (line[i] == symbol) {
			number++;
		} else {
			break;
		}
	}

	/*
	 * Cleare unused symbols.
	 */
	for (i = number; i < REELS_LENGTH; i++) {
		line[i] = EMPTY;
	}

	static int win;
	win = paytable[number][symbol] * core::singleLineBet;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	/*
	 * Keep track of the win.
	 */
	if (win > 0) {
		gameWins.push_back(
				BingoTropicalHotWinCombination(index, number, win,
						core::baseGameMode, false));
	}
#endif
#endif

#ifdef SIMULATION
	if(baseGameMode == true) {
		experiment->baseSymbolMoney[number][symbol] += win;
		if(win > 0) {
			experiment->baseGameSymbolsHitRate[number][symbol]++;
		}
	}
#endif
	return (win);
}

/**
 * Calculate win in all possible lines.
 *
 * @param view Symbols visible in screen view.
 * @param lines Lines with possible win combinations.
 * @param numberOfBettingLines Number of active betting lines.
 *
 * @return Calculated win.
 */
static int linesWin(std::vector<std::vector<int> > &view,
		const int lines[LINES_LENGTH][REELS_LENGTH], int numberOfBettingLines) {
	/*
	 * Static variables are not allocated into stack and it is faster to be used.
	 */
	static int i;
	static int l;

	/*
	 * Check wins in all possible lines.
	 */
	static int win;
	static int result;
	win = 0;
	for (l = 0; l < numberOfBettingLines; l++) {
		int line[REELS_LENGTH] = { -1, -1, -1, -1, -1 };

		/*
		 * Prepare line for combination check.
		 */
		for (i = 0; i < REELS_LENGTH; i++) {
			static int index;
			index = lines[l][i];
			line[i] = view[i][index];
		}

		result = lineWin(line, l);

		/*
		 * Accumulate line win.
		 */
		win += result;
	}

	return (win);
}

/**
 * Play single base game.
 *
 * @return Win in a single base game with free games win included.
 */
int singleBaseGame() {
#ifdef SIMULATION
	experiment->totalNumberOfBaseGames++;
#endif

	/*
	 * Spin reels.
	 */
	core::Calculations::spin(core::view, core::baseReels, core::reelsMinOffset,
			core::reelsMaxOffset);

	/*
	 * Simulate bingo ball out on each reels spin.
	 */
	//TODO Some day lines and symbols can be connected with the bingo bonus.
	core::bingoBallNumber = core::BingoBonus::markBallOut(-1, -1);

	static int win2;
	if (core::BingoBonus::checkForBingoLine() == true) {
		win2 = core::BingoBonus::bingoLineWin();
		core::bingoLineBonusWin = win2;
#ifdef SIMULATION
		experiment->lineBonusMoney += win2;
		experiment->totalNumberOfLineBonus++;
		experiment->bonusGameHitRate++;
#endif
#ifdef OPTIMIZATION
		experiment->lineBonusMoney += win2;
#endif
	} else {
		core::bingoLineBonusWin = 0;
	}

	static int win3;
	if (core::BingoBonus::checkForBingo() == true) {
		win3 = core::BingoBonus::bingoWin();
		core::bingoBonusWin = win3;
#ifdef SIMULATION
		experiment->bingoBonusMoney += win3;
		experiment->totalNumberOfBingoBonus++;
		experiment->bonusGameHitRate++;
#endif
#ifdef OPTIMIZATION
		experiment->bingoBonusMoney += win3;
#endif
	} else {
		core::bingoBonusWin = 0;
	}

	/*
	 * Win accumulated by lines and scatters.
	 */
	static int win1;
	win1 = linesWin(core::view, (const int (*)[REELS_LENGTH]) lines,
			core::numberOfBettingLines);
#ifdef SIMULATION
	experiment->baseGameMoney += win1;

	if((win1) > 0) {
		experiment->baseGameHitRate++;
	}
#endif

	return (win1 + core::bingoLineBonusWin + core::bingoBonusWin);
}

void runBaseGame(int &totalWin) {
	core::baseGameMode = true;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	unsigned long idBet = core::persistBet(core::totalBet, core::credit,
			core::title);
	gameWins.clear();
	core::credit -= core::totalBet;
	core::persistSession(core::credit, core::sessionId);
#endif
#endif
	totalWin = singleBaseGame();
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::credit += totalWin;
	unsigned long idWin = core::persistWin(totalWin, core::credit,
			persistence::BASE_GAME, core::title);
	unsigned long idConfig = core::persistConfig(core::rtp,
			core::numberOfBettingLines, core::singleLineBet,
			core::denomination);
	core::persistHistory(idBet, idWin, idConfig, core::view, symbolsNames,
			core::title);
	core::persistSession(core::credit, core::sessionId);
#endif
#endif

#ifdef SIMULATION
	experiment->wonMoney += totalWin;
	experiment->lostMoney += totalBet;
	if(totalWin > 0) {
		experiment->baseGameHitRate++;
	}
#endif
#ifdef OPTIMIZATION
	experiment->wonMoney += totalWin;
	experiment->lostMoney += totalBet;
#endif
}

}
