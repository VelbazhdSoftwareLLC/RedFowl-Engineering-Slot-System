#ifndef PLAY_SESSION_H
#define PLAY_SESSION_H

namespace persistence {

class PlaySession {
private:
	int credit;

	PlaySession() {
	}

public:
	virtual ~PlaySession() {
	}
};

}

#endif
