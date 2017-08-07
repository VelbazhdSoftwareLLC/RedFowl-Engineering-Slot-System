#ifndef PERSISTENCE_ACCOUNT_H_
#define PERSISTENCE_ACCOUNT_H_

#include <ctime>

#include <odb/core.hxx>

#include "TransactionType.h"

namespace persistence {

#pragma db object
class Account {
private:

	int value;
	TransactionType type;
	time_t timestamp;

#pragma db id auto
	unsigned long id;

	Account() {
	}

	friend class odb::access;

public:

	Account(int value, TransactionType type, time_t timestamp) {
		this->value = value;
		this->type = type;
		this->timestamp = timestamp;
	}

	virtual ~Account() {
	}

	unsigned long getId() const {
		return id;
	}

	time_t getTimestamp() const {
		return timestamp;
	}

	void setTimestamp(time_t timestamp) {
		this->timestamp = timestamp;
	}

	TransactionType getType() const {
		return type;
	}

	void setType(TransactionType type) {
		this->type = type;
	}

	int getValue() const {
		return value;
	}

	void setValue(int value) {
		this->value = value;
	}
};

}

#endif
