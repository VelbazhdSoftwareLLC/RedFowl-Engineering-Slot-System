#include "BingoBonus.h"

namespace core {

const int BingoBonus::bingoLineBonusDistribution[BingoBonus::BINGO_BONUS_DISTRIBUTION_LENGTH] =
		{ 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15,
				20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20,
				10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10,
				15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20, 10, 15, 20 };

const int BingoBonus::bingoBonusDistribution[BingoBonus::BINGO_BONUS_DISTRIBUTION_LENGTH] =
		{ 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110,
				90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100,
				110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90,
				100, 110, 90, 100, 110, 90, 100, 110, 90, 100, 110, 90, 100,
				110, 90, 100, 110, 90, 100, 110 };

int BingoBonus::bingoCards[BingoBonus::BINGO_CARDS_WIDTH][BingoBonus::BINGO_CARDS_LENGTH] =
		{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 10, 11,
				12, 13, 14, 15, 16, 17, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0 }, { 20,
				21, 22, 23, 24, 25, 26, 27, 28, 29, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 0, 0, 0, 0, 0, 0,
						0 } };

bool BingoBonus::bingoNumbersOut[BingoBonus::BINGO_CARDS_WIDTH][BingoBonus::BINGO_CARDS_LENGTH] =
		{ { false, false, false, false, false, false, false, false, false,
				false, false, false, false, false, false, false, false, false },
				{ false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false }, { false, false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false, false, false, false }, { false, false, false,
						false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false }, {
						false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false, false }, { false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false, false, false, false, false },
				{ false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false }, { false, false, false, false, false, false,
						false, false, false, false, false, false, false, false,
						false, false, false, false }, { false, false, false,
						false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false } };

int BingoBonus::numbersInRow[BingoBonus::BINGO_CARDS_LENGTH] = { 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int BingoBonus::bingoLineIndex = -1;

int BingoBonus::bingoCardIndex = -1;

}
