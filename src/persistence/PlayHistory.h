#ifndef PERSISTENCE_PLAYHISTORY_H_
#define PERSISTENCE_PLAYHISTORY_H_

#include <odb/core.hxx>

#include "Bet.h"
#include "Win.h"
#include "Game.h"
#include "GameConfiguration.h"

namespace persistence {

using namespace std;

#pragma db object
class PlayHistory {
private:

	const Game *game;
	const Bet *bet;
	const Win *win;
	const GameConfiguration *configuration;
	string view;
	time_t timestamp;

#pragma db id auto
	unsigned long id;

	PlayHistory() {
	}

	friend class odb::access;

public:

	PlayHistory(const Game *game, const Bet *bet, const Win *win,
				const GameConfiguration *configuration, string view,
				time_t timestamp) {
		this->game = game;
		this->bet = bet;
		this->win = win;
		this->configuration = configuration;
		this->view = view;
		this->timestamp = timestamp;
	}

	virtual ~PlayHistory() {
	}

	const Bet* getBet() const {
		return bet;
	}

	void setBet(const Bet* bet) {
		this->bet = bet;
	}

	const GameConfiguration* getConfiguration() const {
		return configuration;
	}

	void setConfiguration(const GameConfiguration* configuration) {
		this->configuration = configuration;
	}

	const Game* getGame() const {
		return game;
	}

	void setGame(const Game* game) {
		this->game = game;
	}

	time_t getTimestamp() const {
		return timestamp;
	}

	void setTimestamp(time_t timestamp) {
		this->timestamp = timestamp;
	}

	const Win* getWin() const {
		return win;
	}

	void setWin(const Win* win) {
		this->win = win;
	}

	const string& getView() const {
		return view;
	}

	void setView(const string& view) {
		this->view = view;
	}
};

}

#endif
