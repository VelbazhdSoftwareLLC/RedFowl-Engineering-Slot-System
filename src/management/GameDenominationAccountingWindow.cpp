#include <cdk.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../persistence/Bet.h"
#include "../persistence/Bet-odb.hxx"
#include "../persistence/Win.h"
#include "../persistence/Win-odb.hxx"
#include "../persistence/Game.h"
#include "../persistence/Game-odb.hxx"
#include "../persistence/MachineConfiguration.h"
#include "../persistence/MachineConfiguration-odb.hxx"

#include "TuiPointers.h"

#include "GameDenominationAccountingWindow.h"

namespace management {

using namespace std;

void gameDenominationWindow(string title) {
	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	int id = 0;
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

		found = true;
		id = i->getId();
		break;
	}
	t.commit();

	double denomination = 0.0;
	string currency = "";
	/*
	 * Denomination.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::MachineConfiguration> r(
				db->query<persistence::MachineConfiguration>());
		for (odb::result<persistence::MachineConfiguration>::iterator i(
				r.begin()); i != r.end(); ++i) {
			denomination = i->getDenomination();
			currency = i->getCurrency();
			break;
		}
		t.commit();
	}

	long bet = 0;
	long totalGamesPlayed = 0;
	/* Bet report. */{
		odb::transaction t(db->begin());
		odb::result<persistence::Bet> r(
				db->query<persistence::Bet>(
						odb::query<persistence::Bet>::game == id));
		vector<string> titles;
		for (odb::result<persistence::Bet>::iterator i(r.begin()); i != r.end();
				++i) {
			bet += i->getValue();
			if (i->getValue() > 0) {
				totalGamesPlayed++;
			}
		}
		t.commit();
	}

	long win = 0;
	long totalGamesWon = 0;
	long totalGamesLost = 0;
	/* Win report. */{
		odb::transaction t(db->begin());
		odb::result<persistence::Win> r(
				db->query<persistence::Win>(
						odb::query<persistence::Win>::game == id));
		vector<string> titles;
		for (odb::result<persistence::Win>::iterator i(r.begin()); i != r.end();
				++i) {
			win += i->getValue();
			if (i->getValue() > 0) {
				totalGamesWon++;
			} else {
				totalGamesLost++;
			}
		}
		t.commit();
	}

	CDKSWINDOW *scroll = newCDKSwindow(screen, CENTER, 1, LINES - 3, COLS,
			"Total Denomination Accounting", 1000, TRUE, FALSE);

	/* Total bet. */{
		char message[100];
		sprintf(message, "  Denomination:\t\t\t\t%8.2lf %s", denomination,
				currency.c_str());
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total bet. */{
		char message[100];
		sprintf(message, "  Total Bet:\t\t\t\t%8.2lf %s", (bet * denomination),
				currency.c_str());
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total win. */{
		char message[100];
		sprintf(message, "  Total Win:\t\t\t\t%8.2lf %s", (win * denomination),
				currency.c_str());
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total game played. */{
		char message[100];
		sprintf(message, "Total Games Played:\t\t\t%8ld", totalGamesPlayed);
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total games won. */{
		char message[100];
		sprintf(message, "  Total Games Won:\t\t\t%8ld", totalGamesWon);
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total games lost. */{
		char message[100];
		sprintf(message, "  Total Games Lost:\t\t\t%8ld", totalGamesLost);
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	addCDKSwindow(scroll, "", BOTTOM);
	addCDKSwindow(scroll, "\n", BOTTOM);
	activateCDKSwindow(scroll, NULL);
	destroyCDKSwindow(scroll);

	refreshCDKScreen(screen);
}

void gameDenominationAccountingMenuOption() {
	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	odb::transaction t(db->begin());
	odb::result<persistence::Game> r(db->query<persistence::Game>());
	vector<string> titles;
	for (odb::result<persistence::Game>::iterator i(r.begin()); i != r.end();
			++i) {
		titles.push_back(i->getTitle());
	}
	t.commit();

	char **items = new char*[titles.size()];
	for (int i = 0; i < titles.size(); i++) {
		items[i] = new char[100];
		strcpy(items[i], titles[i].c_str());
	}

	/*
	 * Create a scrolling list inside the window.
	 */
	CDKSCROLL *scroll = newCDKScroll(screen, CENTER, 1, RIGHT, LINES - 1, COLS,
			"Select Game", items, titles.size(), NUMBERS, A_REVERSE, TRUE,
			FALSE);

	/*
	 * Refresh the screen.
	 */
	refreshCDKScreen(screen);

	/*
	 * User selection.
	 */
	int selection = activateCDKScroll(scroll, NULL);

	/*
	 * Clean up.
	 */
	for (int i = 0; i < titles.size(); i++) {
		delete[] items[i];
	}
	delete[] items;
	destroyCDKScroll(scroll);

	if (0 <= selection && selection < titles.size()) {
		gameDenominationWindow(titles[selection]);
	}
}

}
