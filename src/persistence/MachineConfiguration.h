#ifndef PERSISTENCE_MACHINECONFIGURATION_H_
#define PERSISTENCE_MACHINECONFIGURATION_H_

#include <string>

#include <odb/core.hxx>

namespace persistence {

#pragma db object
class MachineConfiguration {

private:
	double denomination;
	std::string currency;
	double attendantLimit;

#pragma db id auto
	unsigned long id;

	MachineConfiguration() {
	}

	friend class odb::access;

public:

	MachineConfiguration(double denomination, std::string currency,
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

	const std::string& getCurrency() const {
		return currency;
	}

	void setCurrency(const std::string& currency) {
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
