#ifndef GAMES_CORE_COMMONSTATE_H_
#define GAMES_CORE_COMMONSTATE_H_

#include <vector>
#include <string>

namespace core {

using namespace std;

extern int rtp;
extern string title;
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
extern vector<vector<int> > reels;
extern vector<vector<int> > baseReels;
extern vector<vector<int> > freeReels;
extern vector<vector<int> > view;
extern vector<int> reelsMinOffset;
extern vector<int> reelsMaxOffset;
extern vector<int> reelsStops;

}

#endif
