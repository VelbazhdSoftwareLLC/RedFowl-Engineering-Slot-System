#include <stdio.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include <ncurses.h>
#include <cdk/cdk.h>

#include "../persistence/Session.h"
#include "../persistence/Session-odb.hxx"
#include "../persistence/Account.h"
#include "../persistence/Account-odb.hxx"
#include "../persistence/MachineConfiguration.h"
#include "../persistence/MachineConfiguration-odb.hxx"

#include "TuiPointers.h"

#include "CreditManagement.h"

namespace management {

static CDKBUTTONBOX *buttons = NULL;

static CDKENTRY *value = NULL;

static CDKENTRY *indicator = NULL;

static int oldCredit = 0;

static int credit = 0;

static int entry1 = 0;

static int entry2 = 0;

static bool decimal = false;

static double denomination = 0.0;

static std::string currency = "";

static bool okSave = false;

static void onAdd(SButton *object) {
	credit += ((100 * entry1 + entry2) / denomination) / 100;

	entry1 = 0;
	entry2 = 0;
	decimal = false;

	static char number[100];
	sprintf(number, "                           %11.2lf",
			(double) entry1 + ((double) entry2 / 100));
	setCDKEntryValue(value, number);
	drawCDKEntry(value, TRUE);

	sprintf(number, "CREDIT: %8.2lf %s", (credit * denomination),
			currency.c_str());
	setCDKEntryValue(indicator, number);
	drawCDKEntry(indicator, FALSE);
}

static void onSubtract(SButton *object) {
	int remove = ((100 * entry1 + entry2) / denomination) / 100;

	if (remove > credit) {
		return;
	}

	credit -= remove;

	entry1 = 0;
	entry2 = 0;
	decimal = false;

	static char number[100];
	sprintf(number, "                           %11.2lf",
			(double) entry1 + ((double) entry2 / 100));
	setCDKEntryValue(value, number);
	drawCDKEntry(value, TRUE);

	sprintf(number, "CREDIT: %8.2lf %s", (credit * denomination),
			currency.c_str());
	setCDKEntryValue(indicator, number);
	drawCDKEntry(indicator, FALSE);
}

static void onSave(SButton *object) {
	okSave = true;
	exitOKCDKScreen(screen);
	return;
}

static void onCancel(SButton *object) {
	entry1 = 0;
	entry2 = 0;
	decimal = false;

	static char number[100];
	sprintf(number, "                           %11.2lf",
			(double) entry1 + ((double) entry2 / 100));
	setCDKEntryValue(value, number);
	drawCDKEntry(value, TRUE);
}

static void onExit(SButton *object) {
	okSave = false;
	exitCancelCDKScreen(screen);
	return;
}

static int onButtonsClick(EObjectType type, void *object, void *data, chtype input) {
	if (input == KEY_ENTER) {
		if (getCDKButtonboxCurrentButton(buttons) == 11) {
			entry1 = 0;
			entry2 = 0;
			decimal = false;
		}

		/*
		 * Protect from overflow.
		 */
		if (entry1 > 9999999) {
			return 1;
		}
		if (entry2 > 9) {
			return 1;
		}

		int *entry = NULL;
		if (decimal == false) {
			entry = &entry1;
		} else if (decimal == true) {
			entry = &entry2;
		}

		switch (getCDKButtonboxCurrentButton(buttons)) {
		case 0:
			*entry = *entry * 10 + 1;
			break;
		case 1:
			*entry = *entry * 10 + 4;
			break;
		case 2:
			*entry = *entry * 10 + 7;
			break;
		case 3:
			decimal = true;
			break;
		case 4:
			*entry = *entry * 10 + 2;
			break;
		case 5:
			*entry = *entry * 10 + 5;
			break;
		case 6:
			*entry = *entry * 10 + 8;
			break;
		case 7:
			*entry = *entry * 10 + 0;
			break;
		case 8:
			*entry = *entry * 10 + 3;
			break;
		case 9:
			*entry = *entry * 10 + 6;
			break;
		case 10:
			*entry = *entry * 10 + 9;
			break;
		}

		static char number[100];
		sprintf(number, "                           %11.2lf",
				(double) entry1 + ((double) entry2 / 100));
		setCDKEntryValue(value, number);
		drawCDKEntry(value, TRUE);
	}

	return 1;
}

static void loadCredit(unsigned long &id) {
	int count = 0;
	std::auto_ptr<odb::core::database> connection(
		new odb::pgsql::database(count, NULL));

	bool found = false;

	odb::transaction t(connection->begin());
	odb::result<persistence::Session> r(
		connection->query<persistence::Session>(
			odb::query<persistence::Session>::id == id));
	for (odb::result<persistence::Session>::iterator i(r.begin()); i != r.end();
			++i) {
		persistence::Session &session(*i);
		oldCredit = credit = session.getCredit();
		found = true;
		break;
	}
	t.commit();

	/*
	 * Register session.
	 */
	if (found == false) {
		persistence::Session session(0, time(NULL), time(NULL));
		session.setCredit(credit);
		odb::transaction t(connection->begin());
		id = connection->persist(session);
		t.commit();
	}
}

static void loadDenomination() {
	int count = 0;
	std::auto_ptr<odb::core::database> connection(
		new odb::pgsql::database(count, NULL));
	odb::transaction t(connection->begin());
	odb::result<persistence::MachineConfiguration> r(
		connection->query<persistence::MachineConfiguration>());
	for (odb::result<persistence::MachineConfiguration>::iterator i(r.begin());
			i != r.end(); ++i) {
		persistence::MachineConfiguration &configuration(*i);
		denomination = configuration.getDenomination();
		currency = configuration.getCurrency();
		break;
	}
	t.commit();
}

static void storeAccountTransaction(unsigned long &id) {
	int count = 0;
	std::auto_ptr<odb::core::database> connection(
		new odb::pgsql::database(count, NULL));

	/* Store account transaction. */{
		odb::transaction t(connection->begin());
		if (oldCredit < credit) {
			persistence::Account account((credit - oldCredit),
										 persistence::ATTENDANT_DEBIT, time(NULL));
			connection->persist(account);
			t.commit();
		} else if (oldCredit > credit) {
			persistence::Account account((oldCredit - credit),
										 persistence::ATTENDANT_CREDIT, time(NULL));
			connection->persist(account);
			t.commit();
		}
	}
}

static void storeCredit(unsigned long &id) {
	int count = 0;
	std::auto_ptr<odb::core::database> connection(
		new odb::pgsql::database(count, NULL));

	odb::transaction t(connection->begin());
	odb::result<persistence::Session> r(
		connection->query<persistence::Session>(
			odb::query<persistence::Session>::id == id));
	for (odb::result<persistence::Session>::iterator i(r.begin()); i != r.end();
			++i) {
		persistence::Session &session(*i);
		session.setCredit(credit);
		connection->update(session);
		t.commit();
		break;
	}
}

int main2(int argc, char *argv[], unsigned long &session) {
	loadDenomination();
	loadCredit(session);

	indicator = newCDKEntry(screen, LEFT, TOP, "", "", A_NORMAL, ' ', vINT, 39,
							0, 20,
							FALSE, FALSE);

	value = newCDKEntry(screen, LEFT, 1, "", "", A_NORMAL, ' ', vINT, 39, 0, 38,
						TRUE, FALSE);

	char *labels[12] = { " 1 ", " 4 ", " 7 ", " . ", " 2 ", " 5 ", " 8 ", " 0 ",
						 " 3 ", " 6 ", " 9 ", "CLR"
					   };
	buttons = newCDKButtonbox(screen, LEFT, 4, 4, 40, NULL, 4, 3, labels, 12,
							  A_REVERSE, TRUE,
							  FALSE);
	setCDKButtonboxPreProcess(buttons, onButtonsClick, NULL);

	CDKBUTTON *add = newCDKButton(screen, 0, 13, "       Add       ", onAdd,
								  TRUE,
								  FALSE);
	CDKBUTTON *subtract = newCDKButton(screen, 22, 13, "    Subtract     ",
									   onSubtract,
									   TRUE,
									   FALSE);
	CDKBUTTON *save = newCDKButton(screen, 0, 16, "      Save       ", onSave,
								   TRUE,
								   FALSE);
	CDKBUTTON *cancel = newCDKButton(screen, 22, 16, "     Cancel      ",
									 onCancel,
									 TRUE,
									 FALSE);
	CDKBUTTON *exit = newCDKButton(screen, 0, 19,
								   "                 Exit                  ", onExit,
								   TRUE,
								   FALSE);

	char number[100] = "";
	//TODO Load currency from the database.
	sprintf(number, "CREDIT: %8.2lf %s", (credit * denomination),
			currency.c_str());
	setCDKEntryValue(indicator, number);
	sprintf(number, "                           %11.2lf", (double)0);
	setCDKEntryValue(value, number);
	drawCDKEntry(indicator, FALSE);
	drawCDKEntry(value, TRUE);
	drawCDKButtonbox(buttons, TRUE);
	drawCDKButton(add, TRUE);
	drawCDKButton(subtract, TRUE);
	drawCDKButton(save, TRUE);
	drawCDKButton(cancel, TRUE);
	drawCDKButton(exit, TRUE);

	traverseCDKScreen(screen);
	if (okSave == true) {
		storeAccountTransaction(session);
		storeCredit(session);
	}

	/*
	 * Release resources.
	 */
	destroyCDKButton(exit);
	destroyCDKButton(cancel);
	destroyCDKButton(save);
	destroyCDKButton(add);
	destroyCDKButton(subtract);
	destroyCDKButtonbox(buttons);
	destroyCDKEntry(value);
	destroyCDKEntry(indicator);

	return (EXIT_SUCCESS);
}

}
