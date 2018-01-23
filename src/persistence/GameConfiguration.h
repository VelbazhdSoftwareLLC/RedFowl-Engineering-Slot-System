#ifndef PERSISTENCE_GAMECONFIGURATION_H_
#define PERSISTENCE_GAMECONFIGURATION_H_

#include <odb/core.hxx>

namespace persistence {

#pragma db object
class GameConfiguration {
private:

	int rtp;
	int activeLines;
	int singleBet;
	double denomination;

#pragma db id auto
	unsigned long id;

	GameConfiguration() {
	}

	friend class odb::access;

public:
	GameConfiguration(int rtp, int activeLines, int singleBet,
					  double denomination) {
		this->rtp = rtp;
		this->activeLines = activeLines;
		this->singleBet = singleBet;
		this->denomination = denomination;
	}

	virtual ~GameConfiguration() {
	}

	unsigned long getId() const {
		return id;
	}

	int getRtp() const {
		return rtp;
	}

	void setRtp(int rtp) {
		this->rtp = rtp;
	}

	int getActiveLines() const {
		return activeLines;
	}

	void setActiveLines(int activeLines) {
		this->activeLines = activeLines;
	}

	double getDenomination() const {
		return denomination;
	}

	void setDenomination(double denomination) {
		this->denomination = denomination;
	}

	int getSingleBet() const {
		return singleBet;
	}

	void setSingleBet(int singleBet) {
		this->singleBet = singleBet;
	}
};

}

#endif
