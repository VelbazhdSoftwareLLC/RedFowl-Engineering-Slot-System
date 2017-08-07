#ifndef GAME_CORE_BINGOBONUS_H_
#define GAME_CORE_BINGOBONUS_H_

namespace core {

#define BINGO_CARDS_TALONS 6
#define BINGO_CARDS_WIDTH 9
#define BINGO_CARDS_LENGTH 18

#define BINGO_BONUS_DISTRIBUTION_LENGTH 63

extern int bingoLineBonusDistribution[BINGO_BONUS_DISTRIBUTION_LENGTH];
extern int bingoBonusDistribution[BINGO_BONUS_DISTRIBUTION_LENGTH];

extern int bingoCards[BINGO_CARDS_WIDTH][BINGO_CARDS_LENGTH];
extern bool bingoNumbersOut[BINGO_CARDS_WIDTH][BINGO_CARDS_LENGTH];

extern int bingoLineIndex;
extern int bingoCardIndex;

extern void generateRandomBingoCard();
extern int markBallOut(int line, int symbol);
extern bool checkForBingoLine();
extern bool checkForBingo();
extern int bingoLineWin();
extern int bingoWin();

}

#endif
