#ifndef PERSISTENCE_MACHINECONFIGURATION_H_
#define PERSISTENCE_MACHINECONFIGURATION_H_

#include <string>

#include <odb/core.hxx>

namespace persistence {

using namespace std;

#pragma db object
class MachineConfiguration {

private:
	double denomination;
	string currency;
	double attendantLimit;

#pragma db id auto
	unsigned long id;

	MachineConfiguration() {
	}

	friend class odb::access;

public:

	MachineConfiguration(double denomination, string currency,
						 double attendantLimit) {
		this->denomination = denomination;
		this->currency = currency;
		this->attendantLimit = attendantLimit;
	}

	virtual ~MachineConfiguration() {
	}

	double getDenomination() const {
		return denomination;
	}

	void setDenomination(double denomination) {
		this->denomination = denomination;
	}

	const string& getCurrency() const {
		return currency;
	}

	void setCurrency(const string& currency) {
		this->currency = currency;
	}

	double getAttendantLimit() const {
		return attendantLimit;
	}

	void setAttendantLimit(double attendantLimit) {
		this->attendantLimit = attendantLimit;
	}
};

}

#endif
