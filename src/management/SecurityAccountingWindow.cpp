#include <ctime>
#include <string>

#include <cdk/cdk.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "../persistence/DoorState.h"
#include "../persistence/DoorState-odb.hxx"
#include "../persistence/PowerState.h"
#include "../persistence/PowerState-odb.hxx"
#include "../persistence/PlayHistory.h"
#include "../persistence/PlayHistory-odb.hxx"
#include "../persistence/BillStackerAccess.h"
#include "../persistence/BillStackerAccess-odb.hxx"

#include "Util.h"
#include "TuiPointers.h"

#include "SecurityAccountingWindow.h"

namespace management {

using namespace std;

class DoorInfo {
public:
	time_t lastOpen;
	time_t lastClose;
	long count;
	long games;
	DoorInfo() {
		lastOpen = 0;
		lastClose = 0;
		count = 0;
		games = 0;
	}
};

static void readStackerInfo(time_t &remove, time_t &insert, long &count) {
	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	odb::transaction t(db->begin());
	odb::result<persistence::BillStackerAccess> r(
			db->query<persistence::BillStackerAccess>());
	for (odb::result<persistence::BillStackerAccess>::iterator i(r.begin());
			i != r.end(); ++i) {
		if (i->getAction() == persistence::REMOVE
				&& remove < i->getTimestamp()) {
			remove = i->getTimestamp();
			count++;
		}
		if (i->getAction() == persistence::INSERT
				&& insert < i->getTimestamp()) {
			insert = i->getTimestamp();
		}
	}
	t.commit();
}

static void readLastPowerOffDoorInfo(DoorInfo &info,
		const persistence::DoorType &type, const time_t &off,
		const time_t &on) {
}

static void numberOfGamesAflterLastPowerOff(long &games, const time_t &on) {
	games = 0;

	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	odb::transaction t(db->begin());
	odb::result<persistence::PlayHistory> r(
			db->query<persistence::PlayHistory>(
					odb::query<persistence::PlayHistory>::timestamp >= on));
	for (odb::result<persistence::PlayHistory>::iterator i(r.begin());
			i != r.end(); ++i) {
		games++;
	}
	t.commit();
}

static void readPowerInfo(time_t &off, time_t &on, long &count) {
	on = 0;
	off = 0;

	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	odb::transaction t(db->begin());
	odb::result<persistence::PowerState> r(
			db->query<persistence::PowerState>());
	for (odb::result<persistence::PowerState>::iterator i(r.begin());
			i != r.end(); ++i) {
		if (i->getAction() == persistence::OFF && off < i->getTimestamp()) {
			off = i->getTimestamp();
			count++;
		}
		if (i->getAction() == persistence::ON && on < i->getTimestamp()) {
			on = i->getTimestamp();
		}
	}
	t.commit();
}

static void showDoorInfo(CDKSWINDOW *scroll, const char title[],
		const DoorInfo &info) {
	char time[100];
	char message[100];

	sprintf(message, "%s", title);
	addCDKSwindow(scroll, message, BOTTOM);

	sprintf(message, " -Last open:\t\t\t%20s",
			readbleDataTime(time, info.lastOpen));
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, " -Last close:\t\t\t%20s",
			readbleDataTime(time, info.lastClose));
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, " -Open count:\t\t\t%20ld", info.count);
	addCDKSwindow(scroll, message, BOTTOM);
	sprintf(message, " -Games since last close:\t%20ld", info.count);
	addCDKSwindow(scroll, message, BOTTOM);
}

static void readDoorInfo(DoorInfo &info, const persistence::DoorType &type) {
	int argc = 0;
	auto_ptr<odb::core::database> connection(
			new odb::pgsql::database(argc, NULL));
	auto_ptr<odb::core::database> db = connection;

	/*
	 * Door report.
	 */{
		odb::transaction t(db->begin());
		odb::result<persistence::DoorState> r(
				db->query<persistence::DoorState>(
						odb::query<persistence::DoorState>::type == type));
		for (odb::result<persistence::DoorState>::iterator i(r.begin());
				i != r.end(); ++i) {
			if (i->getAction() == persistence::OPEN) {
				if (info.lastOpen < i->getTimestamp()) {
					info.lastOpen = i->getTimestamp();
				}
				info.count++;
			} else if (i->getAction() == persistence::CLOSE) {
				if (info.lastClose < i->getTimestamp()) {
					info.lastClose = i->getTimestamp();
				}
			}
		}
		t.commit();
	}
	{
		odb::transaction t(db->begin());
		odb::result<persistence::PlayHistory> r(
				db->query<persistence::PlayHistory>(
						odb::query<persistence::PlayHistory>::timestamp
								>= info.lastClose));
		for (odb::result<persistence::PlayHistory>::iterator i(r.begin());
				i != r.end(); ++i) {
			info.count++;
		}
		t.commit();
	}
}

void securityAccountingMenuOption() {
	DoorInfo mainDoor;
	DoorInfo logicBoardDoor;
	DoorInfo dropDoor;
	DoorInfo billStackerBoxDoor;
	DoorInfo bellyDoor;
	DoorInfo topBoxDoor;
	DoorInfo billValidatorDoor;
	DoorInfo extDoor;

	readDoorInfo(mainDoor, persistence::MAIN);
	readDoorInfo(logicBoardDoor, persistence::LOGIC_BOARD);
	readDoorInfo(dropDoor, persistence::DROP);
	readDoorInfo(billStackerBoxDoor, persistence::BILL_STACKER_BOX);
	readDoorInfo(bellyDoor, persistence::BALLEY);
	readDoorInfo(topBoxDoor, persistence::TOP_BOX);
	readDoorInfo(billValidatorDoor, persistence::BILL_VALIDATOR);
	readDoorInfo(extDoor, persistence::EXT);

	time_t lastPowerOff = 0;
	time_t lastPowerOn = 0;
	long powerOffCount = 0;
	long gamesAfterLastPowerOff = 0;
	readPowerInfo(lastPowerOff, lastPowerOn, powerOffCount);
	numberOfGamesAflterLastPowerOff(gamesAfterLastPowerOff, lastPowerOn);

	time_t stackerLastRemove = 0;
	time_t stackerLastInsert = 0;
	long stackerCount = 0;
	readStackerInfo(stackerLastRemove, stackerLastInsert, stackerCount);

	CDKSWINDOW *scroll = newCDKSwindow(screen, CENTER, 1, LINES - 3, COLS,
			"Security Accounting", 1000, TRUE, FALSE);

	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Main Door", mainDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Logic Board Door", logicBoardDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Drop Door", dropDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Bill Stacker Box Door", billStackerBoxDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Belly Door", bellyDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Top Box Door", topBoxDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Bill Validator Door", billValidatorDoor);
	addCDKSwindow(scroll, "\n", BOTTOM);
	showDoorInfo(scroll, "Ext Door", extDoor);

	/* Bill stacker access information. */{
		char time[100];
		char message[100];

		addCDKSwindow(scroll, "\n", BOTTOM);
		addCDKSwindow(scroll, "Bill Stacker Access", BOTTOM);

		sprintf(message, " -Last remove:\t\t\t%20s",
				readbleDataTime(time, stackerLastRemove));
		addCDKSwindow(scroll, message, BOTTOM);
		sprintf(message, " -Last insert:\t\t\t%20s",
				readbleDataTime(time, stackerLastInsert));
		addCDKSwindow(scroll, message, BOTTOM);
		sprintf(message, " -Remove count:\t\t\t%20ld", stackerCount);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	/* Power information. */{
		char time[100];
		char message[100];

		addCDKSwindow(scroll, "\n", BOTTOM);
		addCDKSwindow(scroll, "Power Information", BOTTOM);

		sprintf(message, " -Last power off:\t\t%20s",
				readbleDataTime(time, lastPowerOff));
		addCDKSwindow(scroll, message, BOTTOM);
		sprintf(message, " -Last power on:\t\t%20s",
				readbleDataTime(time, lastPowerOn));
		addCDKSwindow(scroll, message, BOTTOM);
		sprintf(message, " -Power off duration:\t\t%20s",
				readbleTimeDifference(time, lastPowerOff, lastPowerOn));
		addCDKSwindow(scroll, message, BOTTOM);
		sprintf(message, " -Games after power off:\t%20ld",
				gamesAfterLastPowerOff);
		addCDKSwindow(scroll, message, BOTTOM);
	}

	//TODO Doors state when power was down ...

	addCDKSwindow(scroll, "\n", BOTTOM);
	activateCDKSwindow(scroll, NULL);

	destroyCDKSwindow(scroll);

	refreshCDKScreen(screen);
}

}
