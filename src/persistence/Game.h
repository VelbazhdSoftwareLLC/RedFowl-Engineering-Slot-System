#ifndef PERSISTENCE_GAME_H_
#define PERSISTENCE_GAME_H_

#include <string>

#include <odb/core.hxx>

namespace persistence {

#pragma db object
class Game {

private:

	std::string title;
	int rtp;

#pragma db id auto
	unsigned long id;

	Game() {
	}

	friend class odb::access;

public:

	Game(const std::string &title, int rtp) {
		this->title = title;
		this->rtp = rtp;
	}

	virtual ~Game() {
	}

	unsigned long getId() const {
		return id;
	}

	const std::string& getTitle() const {
		return title;
	}

	void setTitle(const std::string& title) {
		this->title = title;
	}

	int getRtp() const {
		return rtp;
	}

	void setRtp(int rtp) {
		this->rtp = rtp;
	}
};

}

#endif
