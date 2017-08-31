#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../../persistence/Bet.h"
#include "../../persistence/Bet-odb.hxx"
#include "../../persistence/Win.h"
#include "../../persistence/Win-odb.hxx"
#include "../../persistence/Game.h"
#include "../../persistence/Game-odb.hxx"
#include "../../persistence/Session.h"
#include "../../persistence/Session-odb.hxx"
#include "../../persistence/PlayHistory.h"
#include "../../persistence/PlayHistory-odb.hxx"
#include "../../persistence/GameConfiguration.h"
#include "../../persistence/GameConfiguration-odb.hxx"
#include "../../persistence/MachineConfiguration.h"
#include "../../persistence/MachineConfiguration-odb.hxx"

#include "Persistence.h"

namespace core {

static auto_ptr<odb::core::database> db;

void connectDb(int argc, char *argv[]) {
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, argv));
	db = connection;
}

void disconnectDb() {
	//TODO Check for database connection close.
}

void registerGame(const string &title, int &rtp) {
	bool found = false;

	odb::transaction t(db->begin());
	odb::result<persistence::Game> r(
			db->query<persistence::Game>(
					odb::query<persistence::Game>::title == title));
	for (odb::result<persistence::Game>::iterator i(r.begin()); i != r.end();
			++i) {
		if (title.compare(i->getTitle()) != 0) {
			continue;
		}

		rtp = i->getRtp();
		found = true;
		break;
	}
	t.commit();

	/*
	 * The game is already registered.
	 */
	if (found == true) {
		return;
	}

	/* Register game. */{
		persistence::Game game(title, rtp);
		odb::transaction t(db->begin());
		db->persist(game);
		t.commit();
	}
}

void registerSession(unsigned long &id, int &credit) {
	bool found = false;

	odb::transaction t(db->begin());
	odb::result<persistence::Session> r(
			db->query<persistence::Session>(
					odb::query<persistence::Session>::id == id));
	for (odb::result<persistence::Session>::iterator i(r.begin()); i != r.end();
			++i) {
		persistence::Session &session(*i);
		credit = session.getCredit();
		found = true;
		break;
	}
	t.commit();

	/*
	 * Register session.
	 */
	if (found == false) {
		persistence::Session session(0, time(NULL), time(NULL));
		odb::transaction t(db->begin());
		id = db->persist(session);
		t.commit();
	}
}

unsigned long persistConfig(int rtp, int numberOfBettingLines,
		int singleLineBet, double denomination) {
	unsigned long id;
	bool found = false;

	odb::transaction t(db->begin());
	odb::result<persistence::GameConfiguration> r(
			db->query<persistence::GameConfiguration>(
					odb::query<persistence::GameConfiguration>::rtp == rtp
							&& odb::query<persistence::GameConfiguration>::activeLines
									== numberOfBettingLines
							&& odb::query<persistence::GameConfiguration>::singleBet
									== singleLineBet
							&& odb::query<persistence::GameConfiguration>::denomination
									== denomination));
	for (odb::result<persistence::GameConfiguration>::iterator i(r.begin());
			i != r.end(); ++i) {
		persistence::GameConfiguration &config(*i);
		id = config.getId();

		found = true;
		break;
	}
	t.commit();

	/*
	 * The game is already registered.
	 */
	if (found == true) {
		return (id);
	}

	/* Register game. */{
		persistence::GameConfiguration config(rtp, numberOfBettingLines,
				singleLineBet, denomination);
		odb::transaction t(db->begin());
		id = db->persist(config);
		t.commit();
	}

	return (id);
}

unsigned long persistBet(int bet, int credit, const string &title) {
	unsigned long id;

	odb::transaction t(db->begin());
	odb::result<persistence::Game> r(
			db->query<persistence::Game>(
					odb::query<persistence::Game>::title == title));
	for (odb::result<persistence::Game>::iterator i(r.begin()); i != r.end();
			++i) {
		if (title.compare(i->getTitle()) != 0) {
			continue;
		}

		persistence::Game &game(*i);
		t.commit();

		persistence::Bet store(bet, credit, persistence::CASHABLE, time(NULL),
				&game);
		odb::transaction t(db->begin());
		id = db->persist(store);
		t.commit();

		break;
	}

	return (id);
}

unsigned long persistWin(int win, int credit, persistence::WinType type,
		const string &title) {
	unsigned long id;

	odb::transaction t(db->begin());
	odb::result<persistence::Game> r(
			db->query<persistence::Game>(
					odb::query<persistence::Game>::title == title));
	for (odb::result<persistence::Game>::iterator i(r.begin()); i != r.end();
			++i) {
		if (title.compare(i->getTitle()) != 0) {
			continue;
		}

		persistence::Game &game(*i);
		t.commit();

		persistence::Win store(win, credit, type, time(NULL), &game);
		odb::transaction t(db->begin());
		id = db->persist(store);
		t.commit();

		break;
	}

	return (id);
}

void persistHistory(unsigned long idBet, unsigned long idWin,
		unsigned long idConfig, const vector<vector<int> > &view,
		const string symbolsNames[], const string &title) {
	odb::transaction t(db->begin());
	odb::result<persistence::Game> r(
			db->query<persistence::Game>(
					odb::query<persistence::Game>::title == title));
	for (odb::result<persistence::Game>::iterator i(r.begin()); i != r.end();
			++i) {
		if (title.compare(i->getTitle()) != 0) {
			continue;
		}

		persistence::Game &game(*i);
		t.commit();

		odb::transaction t(db->begin());
		odb::result<persistence::Bet> r(
				db->query<persistence::Bet>(
						odb::query<persistence::Bet>::id == idBet));
		for (odb::result<persistence::Bet>::iterator i(r.begin()); i != r.end();
				++i) {
			persistence::Bet &bet(*i);
			t.commit();

			odb::transaction t(db->begin());
			odb::result<persistence::Win> r(
					db->query<persistence::Win>(
							odb::query<persistence::Win>::id == idWin));
			for (odb::result<persistence::Win>::iterator i(r.begin());
					i != r.end(); ++i) {
				persistence::Win &win(*i);
				t.commit();

				odb::transaction t(db->begin());
				odb::result<persistence::GameConfiguration> r(
						db->query<persistence::GameConfiguration>(
								odb::query<persistence::GameConfiguration>::id
										== idConfig));
				for (odb::result<persistence::GameConfiguration>::iterator i(
						r.begin()); i != r.end(); ++i) {
					persistence::GameConfiguration &config(*i);
					t.commit();

					string text = "";
					for (int j = 0; j < view[0].size(); j++) {
						for (int i = 0; i < view.size(); i++) {
							text += symbolsNames[view[i][j]];

							if (i < view.size() - 1) {
								text += " ";
							}
						}

						if (j < view[0].size() - 1) {
							text += "\n";
						}
					}

					persistence::PlayHistory store(&game, &bet, &win, &config,
							text, time(NULL));
					odb::transaction t(db->begin());
					db->persist(store);
					t.commit();

					break;
				}

				break;
			}

			break;
		}

		break;
	}

}

void persistSession(int credit, unsigned long id) {
	odb::transaction t(db->begin());
	odb::result<persistence::Session> r(
			db->query<persistence::Session>(
					odb::query<persistence::Session>::id == id));
	for (odb::result<persistence::Session>::iterator i(r.begin()); i != r.end();
			++i) {
		persistence::Session &session(*i);
		session.setCredit(credit);
		session.setEndTime(time(NULL));
		db->update(session);
	}
	t.commit();
}

void adjustDenomination(double &denomination) {
	bool found = false;

	odb::transaction t(db->begin());
	odb::result<persistence::MachineConfiguration> r(
			db->query<persistence::MachineConfiguration>());
	for (odb::result<persistence::MachineConfiguration>::iterator i(r.begin());
			i != r.end(); ++i) {
		persistence::MachineConfiguration &machine(*i);
		denomination = machine.getDenomination();
		found = true;
		break;
	}
	t.commit();

	/*
	 * Register session.
	 */
	if (found == false) {
		denomination = 1;
		persistence::MachineConfiguration machine(denomination, "", 0);
		odb::transaction t(db->begin());
		db->persist(machine);
		t.commit();
	}
}

}
