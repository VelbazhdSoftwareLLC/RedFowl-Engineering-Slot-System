#include <iostream>

#include "../core/Common.h"
#include "../core/BingoBonus.h"
#include "../core/CommonState.h"

#include "SlotState.h"
#include "SlotCalculations.h"

#include "SlotStatistics.h"

namespace bingosteamhot {

/**
 * Array with experiments' statistics.
 */
SlotStatistics experiments[NUMBER_OF_EXPERIMENTS];

/**
 * Pointer to current experiment statistics.
 */
SlotStatistics *experiment = experiments;

/**
 * Hold average results.
 */
SlotStatistics average;

/**
 * Initialize simulation counters.
 */
void initSimulation() {
	experiment->wonMoney = 0L;
	experiment->lostMoney = 0L;
	experiment->baseGameMoney = 0L;
	experiment->lineBonusMoney = 0L;
	experiment->bingoBonusMoney = 0L;
	experiment->totalNumberOfBaseGames = 0L;
	experiment->totalNumberOfLineBonus = 0L;
	experiment->totalNumberOfBingoBonus = 0L;
	experiment->baseGameHitRate = 0L;
	experiment->bonusGameHitRate = 0L;
	for (int j = 0; j < SYMBOLS_LENGTH; j++) {
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			experiment->baseSymbolMoney[i][j] = 0L;
			experiment->baseGameSymbolsHitRate[i][j] = 0L;
		}
	}
}

/**
 * Run single simulation.
 */
void runSimulation() {
	int win = 0;
	for (unsigned long g = 0L; g < NUMBER_OF_SIMULATIONS; g++) {
		core::baseGameMode = true;

		if (core::bingoLineIndex != -1 && core::bingoCardIndex != -1) {
			core::generateRandomBingoCard();
			core::bingoLineBonusWin = 0;
			core::bingoBonusWin = 0;
		}

		runBaseGame(win);
	}
}

/**
 * Calculate average values.
 */
void calculateAverage() {
	for (int e = 0; e < NUMBER_OF_EXPERIMENTS; e++) {
		average.wonMoney += experiments[e].wonMoney / NUMBER_OF_EXPERIMENTS;
		average.lostMoney += experiments[e].lostMoney / NUMBER_OF_EXPERIMENTS;
		average.baseGameMoney += experiments[e].baseGameMoney
				/ NUMBER_OF_EXPERIMENTS;
		average.lineBonusMoney += experiments[e].lineBonusMoney
				/ NUMBER_OF_EXPERIMENTS;
		average.bingoBonusMoney += experiments[e].bingoBonusMoney
				/ NUMBER_OF_EXPERIMENTS;
		average.totalNumberOfBaseGames += experiments[e].totalNumberOfBaseGames
				/ NUMBER_OF_EXPERIMENTS;
		average.totalNumberOfLineBonus += experiments[e].totalNumberOfLineBonus
				/ NUMBER_OF_EXPERIMENTS;
		average.totalNumberOfBingoBonus +=
				experiments[e].totalNumberOfBingoBonus / NUMBER_OF_EXPERIMENTS;
		average.baseGameHitRate += experiments[e].baseGameHitRate
				/ NUMBER_OF_EXPERIMENTS;
		average.bonusGameHitRate += experiments[e].bonusGameHitRate
				/ NUMBER_OF_EXPERIMENTS;
		for (int j = 0; j < SYMBOLS_LENGTH; j++) {
			for (int i = 0; i < COMBINATION_LENGTH; i++) {
				average.baseSymbolMoney[i][j] +=
						experiments[e].baseSymbolMoney[i][j]
								/ NUMBER_OF_EXPERIMENTS;
				average.baseGameSymbolsHitRate[i][j] +=
						experiments[e].baseGameSymbolsHitRate[i][j]
								/ NUMBER_OF_EXPERIMENTS;
			}
		}
	}
}

/**
 * Print results.
 */
void printSimulationResults() {
	std::cout << "Total RTP\t";
	std::cout
			<< (int) ((double) experiment->wonMoney * 10000
					/ (double) experiment->lostMoney) / 100.0 << "%"
			<< std::endl;

	std::cout << "Won Money\t";
	std::cout << experiment->wonMoney << std::endl;

	std::cout << "Lost Money\t";
	std::cout << experiment->lostMoney << std::endl;

	std::cout << "Base Game RTP\t";
	std::cout
			<< (int) ((double) (experiment->baseGameMoney) * 10000
					/ (double) experiment->lostMoney) / 100.0 << "%\t";
	std::cout << std::endl;

	std::cout << "Bonus Game RTP\t";
	std::cout
			<< (int) ((double) (experiment->lineBonusMoney
					+ experiment->bingoBonusMoney) * 10000
					/ (double) experiment->lostMoney) / 100.0 << "%\t";
	std::cout
			<< (int) ((double) (experiment->lineBonusMoney) * 10000
					/ (double) experiment->lostMoney) / 100.0 << "%\t";
	std::cout
			<< (int) ((double) (experiment->bingoBonusMoney) * 10000
					/ (double) experiment->lostMoney) / 100.0 << "%\t";
	std::cout << std::endl;

	std::cout << "Total Number of Base Games\t";
	std::cout << experiment->totalNumberOfBaseGames << std::endl;

	std::cout << "Total Number of Line Bonus\t";
	std::cout << experiment->totalNumberOfLineBonus << std::endl;

	std::cout << "Total Number of Bingo Bonus\t";
	std::cout << experiment->totalNumberOfBingoBonus << std::endl;

	std::cout << "Base Game Hit Rate\t";
	std::cout
			<< (int) ((double) experiment->baseGameHitRate * 10000
					/ (double) experiment->totalNumberOfBaseGames) / 100.0
			<< "%" << std::endl;

	std::cout << "Bonus Game Hit Rate\t";
	std::cout
			<< (int) ((double) experiment->bonusGameHitRate * 10000
					/ (double) experiment->totalNumberOfBaseGames) / 100.0
			<< "%" << std::endl;

	std::cout << std::endl;
	std::cout << "Paytable\t" << std::endl;
	for (int j = 0; j < SYMBOLS_LENGTH; j++) {
		std::cout << symbolsNames[j] << " (" << j << ")" << "\t";
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			std::cout << paytable[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Won Money by Symbol in Base Game\t" << std::endl;
	for (int j = 0; j < SYMBOLS_LENGTH; j++) {
		std::cout << symbolsNames[j] << " (" << j << ")" << "\t";
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			std::cout << experiment->baseSymbolMoney[i][j] << "\t";
		}
		std::cout << "\t";
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			std::cout
					<< (int) ((double) experiment->baseSymbolMoney[i][j] * 10000
							/ (double) experiment->wonMoney) / 100.0 << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Hit Rate by Symbol in Base Game\t" << std::endl;
	for (int j = 0; j < SYMBOLS_LENGTH; j++) {
		std::cout << symbolsNames[j] << " (" << j << ")" << "\t";
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			std::cout << experiment->baseGameSymbolsHitRate[i][j] << "\t";
		}
		std::cout << "\t";
		for (int i = 0; i < COMBINATION_LENGTH; i++) {
			std::cout
					<< (int) ((double) experiment->baseGameSymbolsHitRate[i][j]
							* 10000
							/ (double) experiment->totalNumberOfBaseGames)
							/ 100.0 << "\t";
		}
		std::cout << std::endl;
	}

	for (int p = core::rtp - 90; p <= core::rtp - 90; p++) {
		std::cout << std::endl;
		std::cout << "Base Game Reels " << (90 + p) << "% Symbols Count\t"
				<< std::endl;
		/* Count symbols in reels. */
		{
			int counters[REELS_LENGTH][SYMBOLS_LENGTH] = { { 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0 }, };
			for (int i = 0; i < REELS_LENGTH; i++) {
				for (int j = 0; j < REEL_LENGTH; j++) {
					counters[i][baseGameReels[p][i][j]]++;
				}
			}
			for (int j = 0; j < SYMBOLS_LENGTH; j++) {
				std::cout << symbolsNames[j] << " (" << j << ")" << "\t";
				for (int i = 0; i < REELS_LENGTH; i++) {
					std::cout << counters[i][j] << "\t";
				}
				std::cout << std::endl;
			}
			std::cout << "---------------------------------------------"
					<< std::endl;
			std::cout << "Total:\t";
			unsigned long combinations = 1L;
			for (int i = 0; i < REELS_LENGTH; i++) {
				int sum = 0;
				for (int j = 0; j < SYMBOLS_LENGTH; j++) {
					sum += counters[i][j];
				}
				std::cout << sum << "\t";
				if (sum != 0) {
					combinations *= sum;
				}
			}
			std::cout << std::endl;
			std::cout << "---------------------------------------------"
					<< std::endl;
			std::cout << "Combinations:\t" << combinations;
			std::cout << std::endl;
		}
	}

	for (int p = core::rtp - 90; p <= core::rtp - 90; p++) {
		std::cout << std::endl;
		std::cout << "Base Game Reels " << (90 + p) << "%\t" << std::endl;
		for (int j = 0; j < REEL_LENGTH; j++) {
			for (int i = 0; i < REELS_LENGTH; i++) {
				std::cout << symbolsNames[baseGameReels[p][i][j]] << "\t";
			}
			std::cout << std::endl;
		}
	}
}

}
