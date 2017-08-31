#ifndef GAME_CORE_PERSISTENCE_H_
#define GAME_CORE_PERSISTENCE_H_

#include <vector>
#include <string>

#include "../../persistence/WinType.h"

namespace core {

using namespace std;

extern void connectDb(int argc, char *argv[]);
extern void disconnectDb();
extern void registerGame(const string &title, int &rtp);
extern void registerSession(unsigned long &session, int &credit);
extern unsigned long persistConfig(int rtp, int numberOfBettingLines,
		int singleLineBet, double denomination);
extern unsigned long persistBet(int bet, int credit, const string &title);
extern unsigned long persistWin(int win, int credit, persistence::WinType type,
		const string &title);
extern void persistHistory(unsigned long idBet, unsigned long idWin,
		unsigned long idConfig, const vector<vector<int> > &view,
		const string symbolsNames[], const string &title);
extern void persistSession(int credit, unsigned long id);
extern void adjustDenomination(double &denomination);

}

#endif
