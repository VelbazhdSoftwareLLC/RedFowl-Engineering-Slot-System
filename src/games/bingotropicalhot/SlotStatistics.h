#ifndef GAME_BINGOTROPICALHOT_SLOTSTATISTICS_H_
#define GAME_BINGOTROPICALHOT_SLOTSTATISTICS_H_

#include "../core/Common.h"

namespace bingotropicalhot {

class SlotStatistics {
public:
	/**
	 * Total amount of won money.
	 */
	unsigned long wonMoney;

	/**
	 * Total amount of lost money.
	 */
	unsigned long lostMoney;

	/**
	 * Amount of won money in base game.
	 */
	unsigned long baseGameMoney;

	/**
	 * Bonus amount of won money by lines.
	 */
	unsigned long lineBonusMoney;

	/**
	 * Bonus amount of won money by bingo.
	 */
	unsigned long bingoBonusMoney;

	/**
	 * Total number of base games played.
	 */
	unsigned long totalNumberOfBaseGames;

	/**
	 * Total number of line bonus.
	 */
	unsigned long totalNumberOfLineBonus;

	/**
	 * Total number of bingo bonus.
	 */
	unsigned long totalNumberOfBingoBonus;

	/**
	 * Hit rate of wins in base game. How often there is a win.
	 */
	unsigned long baseGameHitRate;

	/**
	 * Hit rate of wins in bonus game. How often there is a win.
	 */
	unsigned long bonusGameHitRate;

	/**
	 * Symbols win hit rate in base game.
	 */
	unsigned long baseSymbolMoney[COMBINATION_LENGTH][SYMBOLS_LENGTH];

	/**
	 * Symbols hit rate in base game.
	 */
	unsigned long baseGameSymbolsHitRate[COMBINATION_LENGTH][SYMBOLS_LENGTH];
};

extern SlotStatistics experiments[NUMBER_OF_EXPERIMENTS];
extern SlotStatistics *experiment;
extern SlotStatistics average;

extern void initSimulation();
extern void runSimulation();
extern void calculateAverage();
extern void printSimulationResults();

}

#endif
