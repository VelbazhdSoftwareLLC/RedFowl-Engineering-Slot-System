#include <cstdlib>
#include <iostream>

#include "../core/CommonState.h"
#include "../core/Persistence.h"
#include "../core/Calculations.h"
#include "../core/SelectThreeBonusGame.h"

#include "SlotState.h"
#include "SlotDefinition.h"

#include "SlotCalculations.h"

namespace sunsethot {

/**
 * Calculate win in particular line.
 *
 * @param line Single line.
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
	 * Wild wymbol passing to find first regular symbol.
	 */
	for (i = 0; i < REELS_LENGTH; i++) {
		/*
		 * First no wild symbol found.
		 */
		if (symbol != 1 && symbol != 2) {
			break;
		}

		symbol = line[i];
	}

	/*
	 * If all symbols are wild we need to take the most wining symbol.
	 */
	if (symbol == 1 || symbol == 2) {
		symbol = SYM03;
	}

	/*
	 * Wild symbol substitution. Other wild are artificial they are not part of the paytable.
	 */
	for (i = 0; i < REELS_LENGTH; i++) {
		if (line[i] == 1 || line[i] == 2) {
			/*
			 * Substitute wild with regular symbol.
			 */
			line[i] = symbol;
		}
	}

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

	int win = paytable[number][symbol] * core::CommonState::singleLineBet;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	/*
	 * Keep track of the win.
	 */
	if (win > 0) {
		gameWins.push_back(
				SunsetHotWinCombination(index, number, win, core::CommonState::baseGameMode,
						core::CommonState::freeGamesMode));
	}
#endif
#endif

	/*
	 * There is multiplier in free games mode.
	 */
	if (core::CommonState::freeGamesMode == true) {
		win *= core::CommonState::freeGamesMultiplier;
	}

#ifdef SIMULATION
	if(baseGameMode == true) {
		experiment->baseSymbolMoney[number][symbol] += win;
		if(win > 0) {
			experiment->baseGameSymbolsHitRate[number][symbol]++;
		}
	}
	if(freeGamesMode == true) {
		experiment->freeSymbolMoney[number][symbol] += win;
		if(win > 0) {
			experiment->freeGamesSymbolsHitRate[number][symbol]++;
		}
	}
#endif
	return (win);
}

/**
 * Calculate win in all possible lines.
 *
 * @param view Symbols visible in screen view.
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
		int line[5] = { -1, -1, -1, -1, -1 };

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
 * Setup parameters for bonus game mode.
 */
static void bonusGameSetup() {
	/*
	 * In free games there is no bonus game.
	 */
	if (core::CommonState::freeGamesMode == true) {
		return;
	}

	/*
	 * Count the number of bonus symbols on the screen.
	 */
	int nubmerOfBonuses = 0;
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int j = 0; j < ROWS_LENGTH; j++) {
			if (core::CommonState::view[i][j] == 15) {
				nubmerOfBonuses++;
			}
		}
	}
#ifdef SIMULATION
	if(baseGameMode == true && nubmerOfBonuses>0) {
		experiment->baseGameSymbolsHitRate[nubmerOfBonuses][BONUS]++;
	}
	if(freeGamesMode == true && nubmerOfBonuses>0) {
		experiment->freeGamesSymbolsHitRate[nubmerOfBonuses][BONUS]++;
	}
#endif

	if (nubmerOfBonuses < 3) {
		return;
	} else if (3 <= nubmerOfBonuses && nubmerOfBonuses <= 5) {
		core::CommonState::bonusGameMode = true;
		for (int i = 0; i < core::SelectThreeBonusGame::BONUS_SELECT_NUBMER; i++) {
			core::SelectThreeBonusGame::bonusOptions[i] =
					baseScatterDistritution[BONUS_SCATTER_INDEX][rand()
							% (BASE_SCATTER_DISTRIBUTION_LENGTH)];
			core::SelectThreeBonusGame::bonusOptions[i] *=
					scatterMultipliers[BONUS_SCATTER_INDEX][nubmerOfBonuses];
#ifdef SIMULATION
			break;
#endif
#ifdef OPTIMIZATION
			break;
#endif
			/*
			 * Values should be different when they are shown to the player.
			 */
			for (int j = 0; j < i; j++) {
				if (core::SelectThreeBonusGame::bonusOptions[i] == core::SelectThreeBonusGame::bonusOptions[j]) {
					i--;
					break;
				}
			}
		}
		core::SelectThreeBonusGame::reelBonusIndex = -1;
		core::CommonState::bonusSelected = false;
		core::CommonState::bonusWin = core::SelectThreeBonusGame::bonusOptions[0];
#ifdef SIMULATION
		if(baseGameMode == true) {
			experiment->baseBonusMoney += bonusWin;
		}
		if(freeGamesMode == true) {
			experiment->freeBonusMoney += bonusWin;
		}
#endif
	} else if (nubmerOfBonuses > 5) {
		cerr << "More than 5 bonus symbols ..." << endl;
	}
}

/**
 * Setup parameters for free games mode.
 */
static void freeGamesSetup() {
	/*
	 * Count the number of scatter symbols on the screen.
	 */
	int numberOfScatters = 0;
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int j = 0; j < ROWS_LENGTH; j++) {
			if (core::CommonState::view[i][j] == 16) {
				numberOfScatters++;
			}
		}
	}
#ifdef SIMULATION
	if(baseGameMode == true && numberOfScatters>0) {
		experiment->baseGameSymbolsHitRate[numberOfScatters][SCATT]++;
	}
	if(freeGamesMode == true && numberOfScatters>0) {
		experiment->freeGamesSymbolsHitRate[numberOfScatters][SCATT]++;
	}
#endif

	core::CommonState::scatterMultiplier =
			scatterMultipliers[FREE_GAMES_SCATTER_INDEX][numberOfScatters];

	/*
	 * In base game 3+ scatters turn into free spins.
	 */
	if (numberOfScatters < 3 && core::CommonState::freeGamesNumber == 0) {
		return;
	} else if (3 <= numberOfScatters && numberOfScatters <= 5
			&& core::CommonState::freeGamesNumber == 0) {
#ifdef SIMULATION
		if(baseGameMode == true) {
			experiment->baseSymbolMoney[numberOfScatters][SCATT] += scatterMultiplier*totalBet;
			experiment->totalNumberOfFreeGameStarts++;
		}
#endif
		core::CommonState::freeGamesMode = true;
		core::CommonState::freeGamesNumber =
				baseScatterDistritution[FREE_GAMES_SCATTER_INDEX][rand()
						% (BASE_SCATTER_DISTRIBUTION_LENGTH)];
		core::CommonState::freeGamesMultiplier = freeMultiplierDistribution[rand()
				% (FREE_MULTIPLIER_DISTRIBUTION_LENGTH)];
	} else if (3 <= numberOfScatters && numberOfScatters <= 5
			&& core::CommonState::freeGamesNumber > 0) {
#ifdef SIMULATION
		if(freeGamesMode == true) {
			experiment->freeSymbolMoney[numberOfScatters][SCATT] += scatterMultiplier*totalBet;
			experiment->totalNumberOfFreeGameRestarts++;
		}
#endif
		core::CommonState::freeGamesMode = true;
		core::CommonState::freeGamesNumber +=
				freeScatterDistritution[FREE_GAMES_SCATTER_INDEX][rand()
						% (FREE_SCATTER_DISTRIBUTION_LENGTH)];
	} else if (numberOfScatters > 5) {
		cerr << "More than 5 scatter symbols ..." << endl;
	}
}

/**
 * Play single bonus game.
 *
 * @return Win in a single bonus game.
 */
int singleBonusGame(int index) {
#ifdef SIMULATION
	experiment->totalNumberOfBonusGames++;
#endif

	//TODO Move this swap out of the GUI functions.
	int swap = core::SelectThreeBonusGame::bonusOptions[0];
	core::SelectThreeBonusGame::bonusOptions[0] = core::SelectThreeBonusGame::bonusOptions[index];
	core::SelectThreeBonusGame::bonusOptions[index] = swap;

	return (core::CommonState::bonusWin);
}

/**
 * Play single free spin game.
 *
 * @return Win in a single free game.
 */
int singleFreeGame() {
#ifdef SIMULATION
	experiment->totalNumberOfFreeGames++;
#endif

	/*
	 * Spin reels.
	 * In retriggered games from FS1 to FS2 and from FS2 to FS3. FS3 can not rettriger FS.
	 */
	core::Calculations::spin(core::CommonState::view, core::CommonState::freeReels, core::CommonState::reelsMinOffset,
			core::CommonState::reelsMaxOffset);

	/*
	 * There is no bonus game in free games mode.
	 */
	freeGamesSetup();

	/*
	 * Win accumulated by lines and scatters.
	 */
	int win1 = linesWin(core::CommonState::view, (const int (*)[REELS_LENGTH]) lines,
			core::CommonState::numberOfBettingLines);
	int win2 = (core::CommonState::scatterMultiplier * core::CommonState::totalBet);
#ifdef SIMULATION
	experiment->freeGamesMoney += win1;
	experiment->freeScatterMoney += win2;
#endif

	return (win1 + win2);
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
	 * Check is there conditions for bonus game.
	 */
	bonusGameSetup();

	/*
	 * Check is there conditions for free games.
	 */
	core::CommonState::freeGamesMultiplier = 1;
	freeGamesSetup();

	/*
	 * Win accumulated by lines and scatters.
	 */
	int win1 = linesWin(core::CommonState::view, (const int (*)[REELS_LENGTH]) lines,
			core::CommonState::numberOfBettingLines);
	int win2 = (core::CommonState::scatterMultiplier * core::CommonState::totalBet);
#ifdef SIMULATION
	experiment->baseGameMoney += win1;

	experiment->baseScatterMoney += win2;

	if((win1+win2) > 0) {
		experiment->baseGameHitRate++;
	}
#endif

	return (win1 + win2);
}

/**
 * It is used as workaround for play history persistence.
 */
static unsigned long idBet = 0;

void runBonusGame(int &totalWin, int index) {
	core::SelectThreeBonusGame::reelBonusIndex = index;

	/*
	 * User will select one of the bonus options.
	 */
	totalWin = singleBonusGame(index - 1);
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::CommonState::credit += totalWin;
	unsigned long idWin = core::Persistence::persistWin(totalWin, core::CommonState::credit,
			persistence::BONUS, core::CommonState::title);
	core::Persistence::persistSession(core::CommonState::credit, core::CommonState::sessionId);
#endif
#endif
}

void runFreeGame(int &totalWin) {
	core::CommonState::baseGameMode = false;
	core::CommonState::freeGamesMode = true;
	core::CommonState::bonusGameMode = false;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	gameWins.clear();
#endif
#endif

	if (core::CommonState::freeGamesNumber <= 0) {
		return;
	}

	totalWin = singleFreeGame();
	core::CommonState::freeGamesNumber--;
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::CommonState::credit += totalWin;
	unsigned long idWin = core::Persistence::Persistence::persistWin(totalWin, core::CommonState::credit,
			persistence::FREE_GAME, core::CommonState::title);
	unsigned long idConfig = core::Persistence::persistConfig(core::CommonState::rtp,
			core::CommonState::numberOfBettingLines, core::CommonState::singleLineBet,
			core::CommonState::denomination);
	core::Persistence::Persistence::persistHistory(idBet, idWin, idConfig, core::CommonState::view, symbolsNames,
			core::CommonState::title);
	core::Persistence::persistSession(core::CommonState::credit, core::CommonState::sessionId);
#endif
#endif

#ifdef SIMULATION
	//TODO May be it should be in freeGamesMoney.
	experiment->wonMoney += totalWin;
	if(totalWin > 0) {
		experiment->freeGamesHitRate++;
	}
#endif
#ifdef OPTIMIZATION
	experiment->wonMoney += totalWin;
#endif

	/*
	 * If this is the last free game switch off free games mode.
	 */
	if (core::CommonState::freeGamesNumber == 0) {
		core::CommonState::freeGamesMode = false;
	}
}

void runBaseGame(int &totalWin) {
	core::CommonState::baseGameMode = true;
	core::CommonState::freeGamesMode = false;
	core::CommonState::bonusGameMode = false;

#ifndef SIMULATION

#ifndef OPTIMIZATION
	idBet = core::Persistence::persistBet(core::CommonState::totalBet, core::CommonState::credit, core::CommonState::title);
	gameWins.clear();
	core::CommonState::credit -= core::CommonState::totalBet;
	core::Persistence::persistSession(core::CommonState::credit, core::CommonState::sessionId);
#endif
#endif
	totalWin = singleBaseGame();
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::CommonState::credit += totalWin;
	unsigned long idWin = core::Persistence::persistWin(totalWin, core::CommonState::credit,
			persistence::BASE_GAME, core::CommonState::title);
	unsigned long idConfig = core::Persistence::persistConfig(core::CommonState::rtp,
			core::CommonState::numberOfBettingLines, core::CommonState::singleLineBet,
			core::CommonState::denomination);
	core::Persistence::persistHistory(idBet, idWin, idConfig, core::CommonState::view, symbolsNames,
			core::CommonState::title);
	core::Persistence::persistSession(core::CommonState::credit, core::CommonState::sessionId);
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
