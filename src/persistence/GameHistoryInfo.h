#ifndef PERSISTENCE_GAMEHISTORYINFO_H_
#define PERSISTENCE_GAMEHISTORYINFO_H_

#include <string>

#include <odb/core.hxx>

#include "Bet.h"
#include "Win.h"
#include "Game.h"
#include "PlayHistory.h"
#include "GameConfiguration.h"

namespace persistence {

using namespace std;

#define NUMBER_OF_GAMES_IN_HISTORY 50

#pragma db view object(PlayHistory) object(Bet:PlayHistory::bet) object(Win:PlayHistory::win) object(GameConfiguration:PlayHistory::configuration) object(Game=g1:PlayHistory::game) object(Game=g2:Bet::game) object(Game=g3:Win::game) query ("order by" + PlayHistory::timestamp + "desc limit 50")
class GameHistoryInfo {
public:

#pragma db column(g1::title)
	string title;

#pragma db column(PlayHistory::view)
	string screen;

#pragma db column(PlayHistory::timestamp)
	time_t moment;

#pragma db column(Bet::credit)
	int startCredit;

#pragma db column(GameConfiguration::activeLines)
	int activeLines;

#pragma db column(GameConfiguration::singleBet)
	int lineBet;

#pragma db column(GameConfiguration::denomination)
	double denomination;

#pragma db column(Bet::value)
	int totalBet;

#pragma db column(Win::value)
	int totalWin;

#pragma db column(Win::credit)
	int endCredit;
};

//TODO Native SQL view.
//
//#pragma db view query("select \"Game\".title, \"PlayHistory\".view, \"PlayHistory\".timestamp, \"Bet\".credit, \"GameConfiguration\".\"activeLines\", \"GameConfiguration\".\"singleBet\", \"GameConfiguration\".denomination, \"Bet\".value, \"Win\".value, \"Win\".credit from \"PlayHistory\", \"Game\", \"GameConfiguration\", \"Bet\", \"Win\" where \"Game\".id = \"PlayHistory\".game AND \"Bet\".id = \"PlayHistory\".bet AND \"Win\".id = \"PlayHistory\".win AND \"GameConfiguration\".id = \"PlayHistory\".configuration order by \"PlayHistory\".timestamp desc limit 50")
//class GameHistoryInfo {
//public:
//#pragma db type("text")
//	string title;
//
//#pragma db type("text")
//	string screen;
//
//#pragma db type("bigint")
//	time_t moment;
//
//#pragma db type("integer")
//	int startCredit;
//
//#pragma db type("integer")
//	int activeLines;
//
//#pragma db type("integer")
//	int lineBet;
//
//#pragma db type("double precision")
//	double denomination;
//
//#pragma db type("integer")
//	int totalBet;
//
//#pragma db type("integer")
//	int totalWin;
//
//#pragma db type("integer")
//	int endCredit;
//};

}

#endif
