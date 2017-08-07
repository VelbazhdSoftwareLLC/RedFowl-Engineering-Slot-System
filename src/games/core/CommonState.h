#ifndef GAMES_CORE_COMMONSTATE_H_
#define GAMES_CORE_COMMONSTATE_H_

#include <vector>
#include <string>

namespace core {

extern int rtp;
extern std::string title;
extern unsigned long sessionId;
extern bool autoPlayMode;
extern bool baseGameMode;
extern bool freeGamesMode;
extern bool bonusGameMode;
extern bool bonusSelected;
extern double denomination;
extern int credit;
extern int totalBet;
extern int totalWin;
extern int bonusWin;
extern int bingoLineBonusWin;
extern int bingoBonusWin;
extern int bingoBallNumber;
extern int numberOfBettingLines;
extern int singleLineBet;
extern int freeGamesNumber;
extern int freeGamesMultiplier;
extern int scatterMultiplier;
extern std::vector<std::vector<int> > reels;
extern std::vector<std::vector<int> > baseReels;
extern std::vector<std::vector<int> > freeReels;
extern std::vector<std::vector<int> > view;
extern std::vector<int> reelsMinOffset;
extern std::vector<int> reelsMaxOffset;
extern std::vector<int> reelsStops;

}

#endif
