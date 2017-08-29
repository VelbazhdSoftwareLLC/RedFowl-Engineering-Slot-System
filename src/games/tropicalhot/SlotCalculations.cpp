#include <cstdlib>
#include <iostream>

#include "../core/CommonState.h"
#include "../core/Persistence.h"
#include "../core/Calculations.h"
#include "../core/SelectThreeBonusGame.h"

#include "SlotState.h"
#include "SlotDefinition.h"

#include "SlotCalculations.h"

namespace tropicalhot {

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

	int win = paytable[number][symbol] * core::singleLineBet;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	/*
	 * Keep track of the win.
	 */
	if (win > 0) {
		gameWins.push_back(
				TropicalHotWinCombination(index, number, win,
						core::baseGameMode, core::freeGamesMode));
	}
#endif
#endif

	/*
	 * There is multiplier in free games mode.
	 */
	if (core::freeGamesMode == true) {
		win *= core::freeGamesMultiplier;
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
	if (core::freeGamesMode == true) {
		return;
	}

	/*
	 * Count the number of bonus symbols on the screen.
	 */
	int nubmerOfBonuses = 0;
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int j = 0; j < ROWS_LENGTH; j++) {
			if (core::view[i][j] == 15) {
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
		core::bonusGameMode = true;
		for (int i = 0; i < core::BONUS_SELECT_NUBMER; i++) {
			core::bonusOptions[i] =
					baseScatterDistritution[BONUS_SCATTER_INDEX][rand()
							% (BASE_SCATTER_DISTRIBUTION_LENGTH)];
			core::bonusOptions[i] *=
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
				if (core::bonusOptions[i] == core::bonusOptions[j]) {
					i--;
					break;
				}
			}
		}
		core::reelBonusIndex = -1;
		core::bonusSelected = false;
		core::bonusWin = core::bonusOptions[0];
#ifdef SIMULATION
		if(baseGameMode == true) {
			experiment->baseBonusMoney += bonusWin;
		}
		if(freeGamesMode == true) {
			experiment->freeBonusMoney += bonusWin;
		}
#endif
	} else if (nubmerOfBonuses > 5) {
		std::cerr << "More than 5 bonus symbols ..." << std::endl;
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
			if (core::view[i][j] == 16) {
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

	core::scatterMultiplier =
			scatterMultipliers[FREE_GAMES_SCATTER_INDEX][numberOfScatters];

	/*
	 * In base game 3+ scatters turn into free spins.
	 */
	if (numberOfScatters < 3 && core::freeGamesNumber == 0) {
		return;
	} else if (3 <= numberOfScatters && numberOfScatters <= 5
			&& core::freeGamesNumber == 0) {
#ifdef SIMULATION
		if(baseGameMode == true) {
			experiment->baseSymbolMoney[numberOfScatters][SCATT] += scatterMultiplier*totalBet;
			experiment->totalNumberOfFreeGameStarts++;
		}
#endif
		core::freeGamesMode = true;
		core::freeGamesNumber =
				baseScatterDistritution[FREE_GAMES_SCATTER_INDEX][rand()
						% (BASE_SCATTER_DISTRIBUTION_LENGTH)];
		core::freeGamesMultiplier = freeMultiplierDistribution[rand()
				% (FREE_MULTIPLIER_DISTRIBUTION_LENGTH)];
	} else if (3 <= numberOfScatters && numberOfScatters <= 5
			&& core::freeGamesNumber > 0) {
#ifdef SIMULATION
		if(freeGamesMode == true) {
			experiment->freeSymbolMoney[numberOfScatters][SCATT] += scatterMultiplier*totalBet;
			experiment->totalNumberOfFreeGameRestarts++;
		}
#endif
		core::freeGamesMode = true;
		core::freeGamesNumber +=
				freeScatterDistritution[FREE_GAMES_SCATTER_INDEX][rand()
						% (FREE_SCATTER_DISTRIBUTION_LENGTH)];
	} else if (numberOfScatters > 5) {
		std::cerr << "More than 5 scatter symbols ..." << std::endl;
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
	int swap = core::bonusOptions[0];
	core::bonusOptions[0] = core::bonusOptions[index];
	core::bonusOptions[index] = swap;

	return (core::bonusWin);
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
	core::spin(core::view, core::freeReels, core::reelsMinOffset,
			core::reelsMaxOffset);

	/*
	 * There is no bonus game in free games mode.
	 */
	freeGamesSetup();

	/*
	 * Win accumulated by lines and scatters.
	 */
	int win1 = linesWin(core::view, (const int (*)[REELS_LENGTH]) lines,
			core::numberOfBettingLines);
	int win2 = (core::scatterMultiplier * core::totalBet);
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
	core::spin(core::view, core::baseReels, core::reelsMinOffset,
			core::reelsMaxOffset);

	/*
	 * Check is there conditions for bonus game.
	 */
	bonusGameSetup();

	/*
	 * Check is there conditions for free games.
	 */
	core::freeGamesMultiplier = 1;
	freeGamesSetup();

	/*
	 * Win accumulated by lines and scatters.
	 */
	int win1 = linesWin(core::view, (const int (*)[REELS_LENGTH]) lines,
			core::numberOfBettingLines);
	int win2 = (core::scatterMultiplier * core::totalBet);
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
	core::reelBonusIndex = index;

	/*
	 * User will select one of the bonus options.
	 */
	totalWin = singleBonusGame(index - 1);
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::credit += totalWin;
	unsigned long idWin = core::persistWin(totalWin, core::credit,
			persistence::BONUS, core::title);
	core::persistSession(core::credit, core::sessionId);
#endif
#endif
}

void runFreeGame(int &totalWin) {
	core::baseGameMode = false;
	core::freeGamesMode = true;
	core::bonusGameMode = false;

#ifndef SIMULATION
#ifndef OPTIMIZATION
	gameWins.clear();
#endif
#endif

	if (core::freeGamesNumber <= 0) {
		return;
	}

	totalWin = singleFreeGame();
	core::freeGamesNumber--;
#ifndef SIMULATION
#ifndef OPTIMIZATION
	core::credit += totalWin;
	unsigned long idWin = core::persistWin(totalWin, core::credit,
			persistence::FREE_GAME, core::title);
	unsigned long idConfig = core::persistConfig(core::rtp,
			core::numberOfBettingLines, core::singleLineBet,
			core::denomination);
	core::persistHistory(idBet, idWin, idConfig, core::view, symbolsNames,
			core::title);
	core::persistSession(core::credit, core::sessionId);
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
	if (core::freeGamesNumber == 0) {
		core::freeGamesMode = false;
	}
}

void runBaseGame(int &totalWin) {
	core::baseGameMode = true;
	core::freeGamesMode = false;
	core::bonusGameMode = false;

#ifndef SIMULATION

#ifndef OPTIMIZATION
	idBet = core::persistBet(core::totalBet, core::credit, core::title);
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
