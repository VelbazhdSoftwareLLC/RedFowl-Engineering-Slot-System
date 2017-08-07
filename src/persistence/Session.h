#ifndef PERSISTENCE_SESSION_H_
#define PERSISTENCE_SESSION_H_

#include <ctime>

#include <odb/core.hxx>

namespace persistence {

/**
 * The attendant start session. Each player can be with more than one session.
 */
#pragma db object
class Session {
private:
	int credit;
	time_t startTime;
	/**
	 * End time should be constantly updated on each session operation.
	 */
	time_t endTime;

#pragma db id auto
	unsigned long id;

	friend class odb::access;

private:
	Session() {
	}

public:
	Session(int credit, time_t startTime, time_t endTime) {
		this->credit = credit;
		this->startTime = startTime;
		this->endTime = endTime;
	}

	virtual ~Session() {
	}

	int getCredit() const {
		return credit;
	}

	void setCredit(int credit) {
		this->credit = credit;
	}

	time_t getEndTime() const {
		return endTime;
	}

	void setEndTime(time_t endTime) {
		this->endTime = endTime;
	}

	time_t getStartTime() const {
		return startTime;
	}

	void setStartTime(time_t startTime) {
		this->startTime = startTime;
	}
};

}

#endif
