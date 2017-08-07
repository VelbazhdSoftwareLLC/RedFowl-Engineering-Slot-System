#ifndef PERSISTENCE_BILLSTACKERACCESS_H_
#define PERSISTENCE_BILLSTACKERACCESS_H_

#include <ctime>

#include <odb/core.hxx>

#include "BillStackerAction.h"

namespace persistence {

#pragma db object
class BillStackerAccess {
private:

	BillStackerAction action;
	time_t timestamp;

#pragma db id auto
	unsigned long id;

	BillStackerAccess() {
	}

	friend class odb::access;

public:

	BillStackerAccess(BillStackerAction action, time_t timestamp) {
		this->action = action;
		this->timestamp = timestamp;
	}

	virtual ~BillStackerAccess() {
	}

	BillStackerAction getAction() const {
		return action;
	}

	void setAction(BillStackerAction action) {
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
