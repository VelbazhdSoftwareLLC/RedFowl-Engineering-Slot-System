#include "BingoBonus.h"

#include <cstdlib>

namespace core {

#define NUMBER_OF_SHAKES 30

/**
 * Bingo line bonus distribution.
 */
int bingoLineBonusDistribution[BINGO_BONUS_DISTRIBUTION_LENGTH] = { 10, 15, 20,
																	10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20,
																	10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20,
																	10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20,
																	10, 15, 20, 10, 15, 20
																  };

/**
 * Bingo line bonus distribution.
 */
int bingoBonusDistribution[BINGO_BONUS_DISTRIBUTION_LENGTH] = { 90, 100, 110,
																90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110,
																90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110,
																90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110,
																90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110
															  };


/**
 * Bingo cards for the bonus game.
 */
int bingoCards[BINGO_CARDS_WIDTH][BINGO_CARDS_LENGTH] = { {
		1, 2, 3, 4, 5, 6, 7,
		8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}, {
		10, 11, 12, 13, 14, 15, 16, 17, 18,
		19, 0, 0, 0, 0, 0, 0, 0, 0
	}, {
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		0, 0, 0, 0, 0, 0, 0, 0
	}, {
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0,
		0, 0, 0, 0, 0, 0, 0
	}, {
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 0, 0,
		0, 0, 0, 0, 0, 0
	}, {
		50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 0, 0, 0,
		0, 0, 0, 0, 0
	}, {
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 0, 0, 0, 0,
		0, 0, 0, 0
	}, {
		70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0, 0, 0, 0, 0,
		0, 0, 0
	}, {
		80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 0, 0, 0, 0, 0,
		0, 0
	}
};

/**
 * Numbers out in the bingo game (only flags).
 */
bool bingoNumbersOut[BINGO_CARDS_WIDTH][BINGO_CARDS_LENGTH] = {
	{
		false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false
	},
	{
		false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false
	}, {
		false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false
	}, {
		false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false
	}, {
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false
	}, {
		false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false
	},
	{
		false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false
	}, {
		false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false
	}, {
		false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false
	}
};

/**
 * Index of the bingo line in the bingo card.
 */
int bingoLineIndex = -1;

/**
 * Index of the card with the bingo in it;
 */
int bingoCardIndex = -1;

/**
 * Counting numbers in rows in order to keep track of the balance.
 */
static int numbersInRow[BINGO_CARDS_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0
											  };

/**
 * Fix by columns not to have 0 or 3 numbers in column.
 *
 * @return
 */
static bool fixThreeRows() {
	bool wasItChanged = false;

	for (int i = 0; i < 9; i++) {
		int a = -1;
		int b = -1;

		for (int j = 0; j < 18; j += 3) {
			if (0
					== (bingoCards[i][j + 0] != 0 ? 1 : 0)
					+ (bingoCards[i][j + 1] != 0 ? 1 : 0)
					+ (bingoCards[i][j + 2] != 0 ? 1 : 0)) {
				a = j + rand() % (3);
			}
			if (3
					== (bingoCards[i][j + 0] != 0 ? 1 : 0)
					+ (bingoCards[i][j + 1] != 0 ? 1 : 0)
					+ (bingoCards[i][j + 2] != 0 ? 1 : 0)) {
				b = j + rand() % (3);
			}
		}

		if (a == -1 && b == -1) {
			continue;
		}
		if (a == -1) {
			do {
				a = rand() % (18);
			} while (bingoCards[i][a] != 0);
		}
		if (b == -1) {
			do {
				b = rand() % (18);
			} while (bingoCards[i][b] == 0);
		}

		int swap = bingoCards[i][a];
		bingoCards[i][a] = bingoCards[i][b];
		bingoCards[i][b] = swap;
		numbersInRow[a]++;
		numbersInRow[b]--;
		wasItChanged = true;
	}

	return (wasItChanged);
}

/**
 * Fix all rows to have only 5 numbers.
 *
 * @return True if fix was done, false otherwise.
 */
static bool fixRows() {
	bool wasItChanged = false;

	bool done = false;
	do {
		done = false;

		int a = -1;
		int b = -1;

		for (int j = 0; j < 18; j++) {
			if (numbersInRow[j] < 5) {
				a = j;
			}
			if (numbersInRow[j] > 5) {
				b = j;
			}
		}
		if (a == -1 || b == -1) {
			done = true;
			break;
		}

		int x = -1;
		for (int i = 0; i < 9; i++) {
			if (bingoCards[i][a] == 0 && bingoCards[i][b] != 0) {
				x = i;
				break;
			}
		}

		if (x == -1) {
			done = false;
			continue;
		}

		int swap = bingoCards[x][a];
		bingoCards[x][a] = bingoCards[x][b];
		bingoCards[x][b] = swap;
		numbersInRow[a]++;
		numbersInRow[b]--;
		wasItChanged = true;
	} while (done == false);

	return (wasItChanged);
}

/**
 * Shuffle the numbers in single bingo card.
 */
static void shuffleBingoCards() {
	for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
		for (int last = BINGO_CARDS_LENGTH - 1, r = -1, swap = -1; last > 0;
				last--) {
			r = rand() % (last + 1);
			swap = bingoCards[i][last];
			bingoCards[i][last] = bingoCards[i][r];
			bingoCards[i][r] = swap;
		}
	}

	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		numbersInRow[j] = 0;
	}

	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
			numbersInRow[j] += (bingoCards[i][j] != 0 ? 1 : 0);
		}
	}
}

/**
 * Generate random bingo card with 6 talons in it. Also mark the card as empty.
 */
void generateRandomBingoCard() {
	int shakes = 0;
	bool goOn = false;
	do {
		if (shakes <= 0) {
			shuffleBingoCards();
			shakes = NUMBER_OF_SHAKES;
		}

		goOn = fixRows();
		goOn = fixThreeRows() || goOn;
		shakes--;
	} while (goOn == true);

	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
			bingoNumbersOut[i][j] = false;
		}
	}

	bingoLineIndex = -1;
	bingoCardIndex = -1;
}

/**
 * Mark bingo number and return it to the caller.
 *
 * @param line Line with a win in current spin.
 * @param symbol Symbol of the win.
 *
 * @return The number marked.
 */
int markBallOut(int line, int symbol) {
	bool canBeFound = false;

	/*
	 * Check for available numbers.
	 */
	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
			if (bingoNumbersOut[i][j] == false && bingoCards[i][j] != 0) {
				canBeFound = true;
			}
		}
	}

	/*
	 * It should not be possible to search for numbers when there is no any.
	 */
	if (canBeFound == false) {
		return (-1);
	}

	int i = -1;
	int j = -1;
	do {
		i = rand() % BINGO_CARDS_WIDTH;
		j = rand() % BINGO_CARDS_LENGTH;
	} while (bingoNumbersOut[i][j] == true || bingoCards[i][j] == 0);

	bingoNumbersOut[i][j] = true;

	return (bingoCards[i][j]);
}

/**
 * Check is there a bingo line combination.
 *
 * @return True if there is a bingo line, false otherwise.
 */
bool checkForBingoLine() {
	if (bingoLineIndex != -1) {
		return (false);
	}

	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		int count = 0;
		for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
			if (bingoNumbersOut[i][j] == true && bingoCards[i][j] != 0) {
				count++;
			}
		}

		if (count > 5) {
			//TODO It should not be possible.
		} else if (count == 5) {
			bingoLineIndex = j;
			return (true);
		}
	}

	return (false);
}

/**
 * Check is there a bingo combination.
 *
 * @return True if there is a bingo, false otherwise.
 */
bool checkForBingo() {
	if (bingoCardIndex != -1) {
		return (false);
	}

	int count = 0;
	for (int j = 0; j < BINGO_CARDS_LENGTH; j++) {
		if (j % 3 == 0) {
			count = 0;
		}

		for (int i = 0; i < BINGO_CARDS_WIDTH; i++) {
			if (bingoNumbersOut[i][j] == true && bingoCards[i][j] != 0) {
				count++;
			}
		}

		if (count > 15) {
			//TODO It should not be possible.
		} else if (count == 15) {
			bingoCardIndex = j / 3;
			return (true);
		}
	}

	return (false);
}

/**
 * Bingo line bonus win.
 *
 * @return Amount of the win according discrete distribution.
 */
int bingoLineWin() {
	return (bingoLineBonusDistribution[rand() % BINGO_BONUS_DISTRIBUTION_LENGTH]);
}

/**
 * Bingo bonus win.
 *
 * @return Amount of the win according discrete distribution.
 */
int bingoWin() {
	return (bingoBonusDistribution[rand() % BINGO_BONUS_DISTRIBUTION_LENGTH]);
}

}
