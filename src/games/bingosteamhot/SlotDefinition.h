#ifndef GAME_BINGOSTEAMHOT_SLOTDEFINITION_H_
#define GAME_BINGOSTEAMHOT_SLOTDEFINITION_H_

#include <string>

namespace bingosteamhot {

using namespace std;

#define EMPTY -1
#define SYM03 3
#define SYM04 4
#define SYM05 5
#define SYM06 6
#define SYM07 7
#define SYM08 8
#define SYM09 9
#define SYM10 10
#define SYM11 11
#define SYM12 12

extern const int MIN_CREDIT;
extern const int MAX_CREDIT;
extern const int CREDIT_INCREMENT;
extern const int MIN_SELECTED_LINES;
extern const int MAX_SELECTED_LINES;
extern const int MIN_SINGLE_LINE_BET;
extern const int MAX_SINGLE_LINE_BET;

const int ROWS_LENGTH = 3;

const int SYMBOLS_LENGTH = 17;
extern string symbolsNames[SYMBOLS_LENGTH];
extern string symbolsTypes[SYMBOLS_LENGTH];

const int REELS_LENGTH = 5;
const int COMBINATION_LENGTH = (REELS_LENGTH + 1);
extern int paytable[COMBINATION_LENGTH][SYMBOLS_LENGTH];

const int LINES_LENGTH = 9;
extern int lines[LINES_LENGTH][REELS_LENGTH];

const int REEL_LENGTH = 63;
const int REELS_SET_LENGTH = 10;
extern int baseGameReels[REELS_SET_LENGTH][REELS_LENGTH][REEL_LENGTH];

extern int reelsMinOffset[REELS_LENGTH];
extern int reelsMaxOffset[REELS_LENGTH];

}

#endif
