#ifndef PERSISTENCE_BET_H_
#define PERSISTENCE_BET_H_

#include <ctime>

#include <odb/core.hxx>

#include "Game.h"
#include "BetType.h"

namespace persistence {

#pragma db object
class Bet {

private:

	int value;
	int credit;
	BetType type;
	time_t timestamp;
	const Game *game;

#pragma db id auto
	unsigned long id;

	Bet() {
	}

	friend class odb::access;

public:

	Bet(int value, int credit, BetType type, time_t timestamp,
			const Game* game) {
		this->value = value;
		this->credit = credit;
		this->type = type;
		this->timestamp = timestamp;
		this->game = game;
	}

	virtual ~Bet() {
	}

	int getValue() const {
		return value;
	}

	void setValue(int value) {
		this->value = value;
	}

	int getCredit() const {
		return credit;
	}

	void setCredit(int credit) {
		this->credit = credit;
	}

	BetType getType() const {
		return type;
	}

	void setType(BetType type) {
		this->type = type;
	}

	time_t getTimestamp() const {
		return timestamp;
	}

	void setTimestamp(time_t timestamp) {
		this->timestamp = timestamp;
	}

	const Game* getGame() const {
		return game;
	}

	void setGame(const Game* game) {
		this->game = game;
	}
};

}

#endif
