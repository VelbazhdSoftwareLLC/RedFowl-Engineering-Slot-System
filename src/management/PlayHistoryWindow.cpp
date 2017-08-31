#include <ctime>
#include <string>
#include <cstdio>
#include <sstream>

#include <cdk/cdk.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../persistence/GameHistoryInfo.h"
#include "../persistence/GameHistoryInfo-odb.hxx"
#include "../persistence/MachineConfiguration.h"
#include "../persistence/MachineConfiguration-odb.hxx"

#include "Util.h"
#include "TuiPointers.h"

#include "PlayHistoryWindow.h"

namespace management {

using namespace std;

static string currency = "";

static void showPlayHistoryInfo(CDKSWINDOW *scroll,
		const persistence::GameHistoryInfo &info, const int currnet,
		const int all) {
	char time[100];
	char message[100];

	sprintf(message, "Game %03d of %03d - %s", currnet, all,
			info.title.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
	addCDKSwindow(scroll, "\n", BOTTOM);

	istringstream in(info.screen.c_str());
	string line;
	while (getline(in, line, '\n')) {
		sprintf(message, "%s", line.c_str());
		addCDKSwindow(scroll, message, BOTTOM);
	}
	addCDKSwindow(scroll, "\n", BOTTOM);

	sprintf(message, "Date:\t\t\t%20s", readbleDataTime(time, info.moment));
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Start credit:\t\t%20.2lf %s",
			info.denomination * info.startCredit, currency.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Line played:\t\t%20d", info.activeLines);
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Bet per line:\t\t%20d", info.lineBet);
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Denomination:\t\t%20.2lf %s", info.denomination,
			currency.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Total bet:\t\t%20.2lf %s",
			info.denomination * info.totalBet, currency.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "Total win:\t\t%20.2lf %s",
			info.denomination * info.totalWin, currency.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, "End credit:\t\t%20.2lf %s",
			info.denomination * info.endCredit, currency.c_str());
	addCDKSwindow(scroll, message, BOTTOM);
}

void displayMenuOption() {
	CDKSWINDOW *scroll = newCDKSwindow(screen, CENTER, 1, LINES - 3, COLS,
			"Play History", 1000, TRUE, FALSE);

	addCDKSwindow(scroll, "\n", BOTTOM);

	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	/*
	 * Machine cunrrency.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::MachineConfiguration> r(
				db->query<persistence::MachineConfiguration>());
		for (odb::result<persistence::MachineConfiguration>::iterator i(
				r.begin()); i != r.end(); ++i) {
			currency = i->getCurrency();
			break;
		}
		t.commit();
	}

	odb::transaction t(db->begin());
	odb::result<persistence::GameHistoryInfo> r(
			db->query<persistence::GameHistoryInfo>());
	int number = 0;
	for (odb::result<persistence::GameHistoryInfo>::iterator i(r.begin());
			i != r.end(); ++i) {
		showPlayHistoryInfo(scroll, (*i), (number + 1), r.size());
		addCDKSwindow(scroll, "\n", BOTTOM);
		number++;
	}
	t.commit();

	activateCDKSwindow(scroll, NULL);

	destroyCDKSwindow(scroll);

	refreshCDKScreen(screen);
}

}
