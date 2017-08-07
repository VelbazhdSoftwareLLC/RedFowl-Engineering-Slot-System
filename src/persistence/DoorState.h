#ifndef PERSISTENCE_DOORSTATE_H_
#define PERSISTENCE_DOORSTATE_H_

#include <ctime>

#include <odb/core.hxx>

#include "DoorType.h"
#include "DoorAction.h"

namespace persistence {

#pragma db object
class DoorState {
private:

	DoorType type;
	DoorAction action;
	time_t timestamp;

#pragma db id auto
	unsigned long id;

	DoorState() {};

	friend class odb::access;

public:

	DoorState(DoorType type, DoorAction action, time_t timestamp) {
		this->type = type;
		this->action = action;
		this->timestamp = timestamp;
	};

	virtual ~DoorState() {
	}

	DoorAction getAction() const {
		return action;
	}

	void setAction(DoorAction action) {
		this->action = action;
	}

	time_t getTimestamp() const {
		return timestamp;
	}

	void setTimestamp(time_t timestamp) {
		this->timestamp = timestamp;
	}

	DoorType getType() const {
		return type;
	}

	void setType(DoorType type) {
		this->type = type;
	}
};

}

#endif
