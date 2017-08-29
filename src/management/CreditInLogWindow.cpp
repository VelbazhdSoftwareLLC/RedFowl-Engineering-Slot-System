#include <cdk/cdk.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../persistence/Account.h"
#include "../persistence/Account-odb.hxx"
#include "../persistence/MachineConfiguration.h"
#include "../persistence/MachineConfiguration-odb.hxx"

#include "Util.h"
#include "TuiPointers.h"

#include "CreditInLogWindow.h"

namespace management {

void creditInLogMenuOption() {
	CDKSWINDOW *scroll = newCDKSwindow(screen, CENTER, 1, LINES - 3, COLS,
			"Credit In Log", 1000, TRUE, FALSE);

	addCDKSwindow(scroll, "\n", BOTTOM);

	int argc = 0;
	std::auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	std::auto_ptr<odb::core::database> db = connection;

	/*
	 * Machine cunrrency.
	 */
	static double denomination = 0;
	static std::string currency = "";
	{
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

	/* Head row. */{
		static char message[100];
		sprintf(message, "%-8s %20s %20s %20s", "#", "Data & Time",
				"Description", "Data");
		addCDKSwindow(scroll, message, BOTTOM);
		addCDKSwindow(scroll, "\n", BOTTOM);
	}

	/*
	 * Only last 24 hours.
	 */
	odb::transaction t(db->begin());
	odb::result<persistence::Account> r(
			db->query<persistence::Account>(
					odb::query<persistence::Account>::type
							== persistence::ATTENDANT_DEBIT
							&& odb::query<persistence::Account>::timestamp
									>= (time(NULL) - 24 * 60 * 60)));
	for (odb::result<persistence::Account>::iterator i(r.begin()); i != r.end();
			++i) {
		static char time[100];
		static char kind[100];
		static char message[100];

		if ((*i).getType() == persistence::ATTENDANT_DEBIT) {
			strcpy(kind, "Keyed in.");
		} else {
			strcpy(kind, "");
		}

		sprintf(message, "%-8ld %20s %20s %20.2lf %s", (*i).getId(),
				readbleDataTime(time, (*i).getTimestamp()), kind,
				(*i).getValue() * denomination, currency.c_str());
		addCDKSwindow(scroll, message, BOTTOM);

		addCDKSwindow(scroll, "\n", BOTTOM);
	}
	t.commit();

	activateCDKSwindow(scroll, NULL);

	destroyCDKSwindow(scroll);

	refreshCDKScreen(screen);
}

}
