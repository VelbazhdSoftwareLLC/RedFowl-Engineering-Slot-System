#ifndef GAME_CORE_PERSISTENCE_H_
#define GAME_CORE_PERSISTENCE_H_

#include <vector>
#include <string>

#include "../../persistence/WinType.h"

namespace core {

extern void connectDb(int argc, char *argv[]);
extern void disconnectDb();
extern void registerGame(const std::string &title, int &rtp);
extern void registerSession(unsigned long &session, int &credit);
extern unsigned long persistConfig(int rtp, int numberOfBettingLines,
		int singleLineBet, double denomination);
extern unsigned long persistBet(int bet, int credit, const std::string &title);
extern unsigned long persistWin(int win, int credit, persistence::WinType type,
		const std::string &title);
extern void persistHistory(unsigned long idBet, unsigned long idWin,
		unsigned long idConfig, const std::vector<std::vector<int> > &view,
		const std::string symbolsNames[], const std::string &title);
extern void persistSession(int credit, unsigned long id);
extern void adjustDenomination(double &denomination);

}

#endif
