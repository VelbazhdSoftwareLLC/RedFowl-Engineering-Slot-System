#include <cdk/cdk.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../persistence/Bet.h"
#include "../persistence/Bet-odb.hxx"
#include "../persistence/Win.h"
#include "../persistence/Win-odb.hxx"
#include "../persistence/Account.h"
#include "../persistence/Account-odb.hxx"
#include "../persistence/MachineConfiguration.h"
#include "../persistence/MachineConfiguration-odb.hxx"

#include "TuiPointers.h"

#include "MachineAccountingWindow.h"

namespace management {

void machineAccountingMenuOption() {
	int argc = 0;
	std::auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	std::auto_ptr<odb::core::database> db = connection;

	long bet = 0;
	/*
	 * Bet report.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::Bet> r(db->query<persistence::Bet>());
		for (odb::result<persistence::Bet>::iterator i(r.begin()); i != r.end();
				++i) {
			bet += i->getValue();
		}
		t.commit();
	}

	long win = 0;
	/*
	 * Win report.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::Win> r(db->query<persistence::Win>());
		for (odb::result<persistence::Win>::iterator i(r.begin()); i != r.end();
				++i) {
			win += i->getValue();
		}
		t.commit();
	}

	long debit = 0;
	/*
	 * Bet report.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::Account> r(db->query<persistence::Account>());
		for (odb::result<persistence::Account>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (i->getType() == persistence::ATTENDANT_DEBIT) {
				debit += i->getValue();
			}
		}
		t.commit();
	}

	long credit = 0;
	/*
	 * Bet report.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::Account> r(db->query<persistence::Account>());
		for (odb::result<persistence::Account>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (i->getType() == persistence::ATTENDANT_CREDIT) {
				credit += i->getValue();
			}
		}
		t.commit();
	}

	double denomination = 0.0;
	std::string currency = "";
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

	CDKSWINDOW *scroll = newCDKSwindow(screen, CENTER, 1, LINES - 3, COLS,
			"Machine Accounting", 1000, TRUE, FALSE);

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

	/* Total in. */{
		char message[100];
		sprintf(message, "Total In");
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Cashless in. */{
		char message[100];
		sprintf(message, "  Cashless In:\t\t\t\t%8.2lf %s",
				(debit * denomination), currency.c_str());
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Total out. */{
		char message[100];
		sprintf(message, "Total Out");
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Cancelled credits. */{
		char message[100];
		sprintf(message, "  Cancelled Credits Paid by Attendant:\t%8.2lf %s",
				(credit * denomination), currency.c_str());
		addCDKSwindow(scroll, "", BOTTOM);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	addCDKSwindow(scroll, "", BOTTOM);
	addCDKSwindow(scroll, "\n", BOTTOM);
	activateCDKSwindow(scroll, NULL);

	destroyCDKSwindow(scroll);

	refreshCDKScreen(screen);
}

}
