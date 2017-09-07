#include <ctime>
#include <cstdlib>

#include "../core/BingoBonus.h"
#include "../core/CommonState.h"
#include "../core/Persistence.h"
#include "../core/Calculations.h"

#include "SlotState.h"
#include "SlotStatistics.h"

#include "SlotCalculations.h"

namespace bingosteamhot {

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
	win = paytable[number][symbol] * core::CommonState::singleLineBet;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	/*
	 * Keep track of the win.
	 */
	if (win > 0) {
		gameWins.push_back(
				BingoSteamHotWinCombination(index, number, win,
						core::CommonState::baseGameMode, false));
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
static int linesWin(vector<vector<int> > &view,
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
	core::Calculations::spin(core::CommonState::view, core::CommonState::baseReels, core::CommonState::reelsMinOffset,
			core::CommonState::reelsMaxOffset);

	/*
	 * Simulate bingo ball out on each reels spin.
	 */
	//TODO Some day lines and symbols can be connected with the bingo bonus.
	core::CommonState::bingoBallNumber = core::BingoBonus::markBallOut(-1, -1);

	static int win2;
	if (core::BingoBonus::checkForBingoLine() == true) {
		win2 = core::BingoBonus::bingoLineWin();
		core::CommonState::bingoLineBonusWin = win2;
#ifdef SIMULATION
		experiment->lineBonusMoney += win2;
		experiment->totalNumberOfLineBonus++;
		experiment->bonusGameHitRate++;
#endif
#ifdef OPTIMIZATION
		experiment->lineBonusMoney += win2;
#endif
	} else {
		core::CommonState::bingoLineBonusWin = 0;
	}

	static int win3;
	if (core::BingoBonus::checkForBingo() == true) {
		win3 = core::BingoBonus::bingoWin();
		core::CommonState::bingoBonusWin = win3;
#ifdef SIMULATION
		experiment->bingoBonusMoney += win3;
		experiment->totalNumberOfBingoBonus++;
		experiment->bonusGameHitRate++;
#endif
#ifdef OPTIMIZATION
		experiment->bingoBonusMoney += win3;
#endif
	} else {
		core::CommonState::bingoBonusWin = 0;
	}

	/*
	 * Win accumulated by lines and scatters.
	 */
	static int win1;
	win1 = linesWin(core::CommonState::view, (const int (*)[REELS_LENGTH]) lines,
			core::CommonState::numberOfBettingLines);
#ifdef SIMULATION
	experiment->baseGameMoney += win1;

	if((win1) > 0) {
		experiment->baseGameHitRate++;
	}
#endif

	return (win1 + core::CommonState::bingoLineBonusWin + core::CommonState::bingoBonusWin);
}

void runBaseGame(int &totalWin) {
	core::CommonState::baseGameMode = true;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	unsigned long idBet = core::persistBet(core::CommonState::totalBet, core::CommonState::credit,
			core::CommonState::title);
	gameWins.clear();
	core::CommonState::credit -= core::CommonState::totalBet;
	core::persistSession(core::CommonState::credit, core::CommonState::sessionId);
#endif
#endif
	totalWin = singleBaseGame();
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::CommonState::credit += totalWin;
	unsigned long idWin = core::persistWin(totalWin, core::CommonState::credit,
			persistence::BASE_GAME, core::CommonState::title);
	unsigned long idConfig = core::persistConfig(core::CommonState::rtp,
			core::CommonState::numberOfBettingLines, core::CommonState::singleLineBet,
			core::CommonState::denomination);
	core::persistHistory(idBet, idWin, idConfig, core::CommonState::view, symbolsNames,
			core::CommonState::title);
	core::persistSession(core::CommonState::credit, core::CommonState::sessionId);
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
