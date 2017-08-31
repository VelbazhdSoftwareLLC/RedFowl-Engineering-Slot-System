#ifndef PERSISTENCE_EVENT_H_
#define PERSISTENCE_EVENT_H_

#include <ctime>
#include <string>

#include <odb/core.hxx>

namespace persistence {

using namespace std;

#pragma db object
class Event {
private:

	time_t timestamp;
	string description;
	string data;

#pragma db id auto
	unsigned long id;

	Event() {
	}

	friend class odb::access;

public:

	virtual ~Event() {
	}

	unsigned long getId() const {
		return id;
	}

	const string& getData() const {
		return data;
	}

	void setData(const string& data) {
		this->data = data;
	}

	const string& getDescription() const {
		return description;
	}

	void setDescription(const string& description) {
		this->description = description;
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
