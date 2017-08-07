#ifndef PERSISTENCE_POWERSTATE_H_
#define PERSISTENCE_POWERSTATE_H_

#include <ctime>

#include <odb/core.hxx>

#include "PowerAction.h"

namespace persistence {

#pragma db object
class PowerState {
private:

	PowerAction action;
	time_t timestamp;

#pragma db id auto
	unsigned long id;

	PowerState() {
	}

	friend class odb::access;

public:

	PowerState(PowerAction action, time_t timestamp) {
		this->action = action;
		this->timestamp = timestamp;
	}

	virtual ~PowerState() {
	}

	PowerAction getAction() const {
		return action;
	}

	void setAction(PowerAction action) {
		this->action = action;
	}

	time_t getTimestamp() const {
		return timestamp;
	}

	void setTimestamp(time_t timestamp) {
		this->timestamp = timestamp;
	}
};

}

#endif
