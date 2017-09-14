#ifndef GAME_CORE_PERSISTENCE_H_
#define GAME_CORE_PERSISTENCE_H_

#include <vector>
#include <string>

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

#include "../../persistence/WinType.h"

namespace core {

using namespace std;
using namespace persistence;

class Persistence {

public:

	/**
	 * Database connection pointer.
	 */
	static auto_ptr<odb::core::database> db;

	/**
	 * Connect to the database.
	 *
	 * @param argc Command line arguments count.
	 * @param argv Command line arguments values.
	 */
	static void connectDb(int argc, char *argv[]) {
		auto_ptr<odb::core::database> connection(
				new odb::pgsql::database(argc, argv));
		db = connection;
	}

	/**
	 * Disconnect database.
	 */
	static void disconnectDb() {
		//TODO Check for database connection close.
	}

	/**
	 * Register game if it is not presented into the database.
	 *
	 * @param title Game title.
	 * @param rtp Game RTP.
	 */
	static void registerGame(const string &title, int &rtp) {
		bool found = false;

		odb::transaction t(db->begin());
		odb::result<Game> r(
				db->query < Game
						> (odb::query<Game>::title == title));
		for (odb::result<Game>::iterator i(r.begin());
				i != r.end(); ++i) {
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
			Game game(title, rtp);
			odb::transaction t(db->begin());
			db->persist(game);
			t.commit();
		}
	}

	/**
	 * Register new playing session if it is not presented yet.
	 *
	 * @param seesion Session object refrence.
	 * @param credit Credit variable reference.
	 */
	static void registerSession(unsigned long &id, int &credit) {
		bool found = false;

		odb::transaction t(db->begin());
		odb::result<Session> r(
				db->query < Session
						> (odb::query<Session>::id == id));
		for (odb::result<Session>::iterator i(r.begin());
				i != r.end(); ++i) {
			Session &session(*i);
			credit = session.getCredit();
			found = true;
			break;
		}
		t.commit();

		/*
		 * Register session.
		 */
		if (found == false) {
			Session session(0, time(NULL), time(NULL));
			odb::transaction t(db->begin());
			id = db->persist(session);
			t.commit();
		}
	}

	/**
	 * Store current game configurationl.
	 *
	 * @param rtp Game RTP.
	 * @param numberOfBettingLines Number of lines used in total bet calculation.
	 * @param singleLineBet Bet for a single line used in total bet calculation.
	 * @param denomination Currency denomination of the virtual coins.
	 */
	static unsigned long persistConfig(int rtp, int numberOfBettingLines,
			int singleLineBet, double denomination) {
		unsigned long id;
		bool found = false;

		odb::transaction t(db->begin());
		odb::result<GameConfiguration> r(
				db->query < GameConfiguration
						> (odb::query<GameConfiguration>::rtp
								== rtp
								&& odb::query<GameConfiguration>::activeLines
										== numberOfBettingLines
								&& odb::query<GameConfiguration>::singleBet
										== singleLineBet
								&& odb::query<GameConfiguration>::denomination
										== denomination));
		for (odb::result<GameConfiguration>::iterator i(r.begin());
				i != r.end(); ++i) {
			GameConfiguration &config(*i);
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
			GameConfiguration config(rtp, numberOfBettingLines,
					singleLineBet, denomination);
			odb::transaction t(db->begin());
			id = db->persist(config);
			t.commit();
		}

		return (id);
	}

	/**
	 * Store bet.
	 *
	 * @param bet Bet value.
	 * @param credit Current credit value.
	 * @param title Title of the game where the bet is done.
	 */
	static unsigned long persistBet(int bet, int credit, const string &title) {
		unsigned long id;

		odb::transaction t(db->begin());
		odb::result<Game> r(
				db->query < Game
						> (odb::query<Game>::title == title));
		for (odb::result<Game>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (title.compare(i->getTitle()) != 0) {
				continue;
			}

			Game &game(*i);
			t.commit();

			Bet store(bet, credit, CASHABLE,
					time(NULL), &game);
			odb::transaction t(db->begin());
			id = db->persist(store);
			t.commit();

			break;
		}

		return (id);
	}

	/**
	 * Store win.
	 *
	 * @param bet Win value.
	 * @param credit Current credit value.
	 * @param type Kind of the win.
	 * @param title Title of the game where the win appeared.
	 */
	static unsigned long persistWin(int win, int credit,
			WinType type, const string &title) {
		unsigned long id;

		odb::transaction t(db->begin());
		odb::result<Game> r(
				db->query < Game
						> (odb::query<Game>::title == title));
		for (odb::result<Game>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (title.compare(i->getTitle()) != 0) {
				continue;
			}

			Game &game(*i);
			t.commit();

			Win store(win, credit, type, time(NULL), &game);
			odb::transaction t(db->begin());
			id = db->persist(store);
			t.commit();

			break;
		}

		return (id);
	}

	/**
	 * Store history.
	 *
	 * @param idBet Bet identifier.
	 * @param idWin Win identifier.
	 * @param idConfig Game configuration identifier.
	 * @param view Game screen.
	 * @param symbolName Names of the game symbols.
	 * @param title Title of the game for history save.
	 */
	static void persistHistory(unsigned long idBet, unsigned long idWin,
			unsigned long idConfig, const vector<vector<int> > &view,
			const string symbolsNames[], const string &title) {
		odb::transaction t(db->begin());
		odb::result<Game> r(
				db->query < Game
						> (odb::query<Game>::title == title));
		for (odb::result<Game>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (title.compare(i->getTitle()) != 0) {
				continue;
			}

			Game &game(*i);
			t.commit();

			odb::transaction t(db->begin());
			odb::result<Bet> r(
					db->query < Bet
							> (odb::query<Bet>::id == idBet));
			for (odb::result<Bet>::iterator i(r.begin());
					i != r.end(); ++i) {
				Bet &bet(*i);
				t.commit();

				odb::transaction t(db->begin());
				odb::result<Win> r(
						db->query < Win
								> (odb::query<Win>::id == idWin));
				for (odb::result<Win>::iterator i(r.begin());
						i != r.end(); ++i) {
					Win &win(*i);
					t.commit();

					odb::transaction t(db->begin());
					odb::result<GameConfiguration> r(
							db->query < GameConfiguration
									> (odb::query<GameConfiguration>::id
											== idConfig));
					for (odb::result<GameConfiguration>::iterator i(
							r.begin()); i != r.end(); ++i) {
						GameConfiguration &config(*i);
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

						PlayHistory store(&game, &bet, &win,
								&config, text, time(NULL));
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

	/**
	 * Store session information.
	 *
	 * @param credit Credit value.
	 * @param id Session identifier.
	 */
	static void persistSession(int credit, unsigned long id) {
		odb::transaction t(db->begin());
		odb::result<Session> r(
				db->query < Session
						> (odb::query<Session>::id == id));
		for (odb::result<Session>::iterator i(r.begin());
				i != r.end(); ++i) {
			Session &session(*i);
			session.setCredit(credit);
			session.setEndTime(time(NULL));
			db->update(session);
		}
		t.commit();
	}

	/**
	 * Adjust denomination.
	 *
	 * @param denomination Denomination value.
	 */
	static void adjustDenomination(double &denomination) {
		bool found = false;

		odb::transaction t(db->begin());
		odb::result<MachineConfiguration> r(
				db->query<MachineConfiguration>());
		for (odb::result<MachineConfiguration>::iterator i(
				r.begin()); i != r.end(); ++i) {
			MachineConfiguration &machine(*i);
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
			MachineConfiguration machine(denomination, "", 0);
			odb::transaction t(db->begin());
			db->persist(machine);
			t.commit();
		}
	}

};

}

#endif
