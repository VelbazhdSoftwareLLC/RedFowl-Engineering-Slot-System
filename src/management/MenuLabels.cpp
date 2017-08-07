#include "CallMenuFunctions.h"

#include "MenuLabels.h"

namespace management {

int MenuLabels::HEIGHT[] = { 9, 13, 2, 12, 3, 2 };

/*
 * There is some kind of bug and this array should finish with NONE value.
 */
int MenuLabels::LOCATION[] = { LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, NONE };

const char *MenuLabels::VALUES[MAX_MENU_ITEMS][MAX_SUB_ITEMS] = { { "Accounting",
		"Machine Accounting", "Game Accounting",
		"Total Denomination Accounting", "Game Denomination Accounting",
		"Voucher Accounting", "Security Accounting", "Error Accounting",
		"Bill Accounting", "", "", "", "", "", ""
	}, { "Diagnostics",
		"Keyboard Test", "Doors and Keys Test", "Top Lights Test",
		"Sound Test/Setup", "Video Test", "Printer Test", "Bill Validator Test",
		"Touch Screen Calibration", "Temperature Monitor", "Coin Acceptor",
		"Hopper", "Programs Versions", "", ""
	}, { "History", "Display", "",
		"", "", "", "", "", "", "", "", "", "", "", ""
	}, { "Event Logs",
		"All Events Log", "Credit In Log", "Cash Out Log", "Bill Accepted Log",
		"Hand Pay Log", "Progressive Log", "Voucher In Log", "Voucher Out Log",
		"Cashless In Log", "Cashless Out Log", "Host Bonus and Mystery Log", "",
		"", ""
	}, { "Service", "Hopper Fill", "Authorization", "", "", "",
		"", "", "", "", "", "", "", "", ""
	}, { "Exit", "Close", "", "",
		"", "", "", "", "", "", "", "", "", "", ""
	},
};

void (* const MenuLabels::FUNCTIONS[MAX_MENU_ITEMS][MAX_SUB_ITEMS])() = {
	{	NULL, machineAccountingMenuOption, gameAccountingMenuOption, totalDenominationAccountingMenuOption, gameDenominationAccountingMenuOption, voucherAccountingMenuOption, securityAccountingMenuOption, errorAccountingMenuOption, billAccountingMenuOption, NULL, NULL, NULL, NULL, NULL, NULL},
	{	NULL, keyboardTestMenuOption, dorsAndKeysTestMenuOption, topLightsTestMenuOption, soundTestSetupMenuOption, videoTestMenuOption, printerTestMenuOption, billValidatorTestMenuOption, touchScreenCalibrationMenuOption, temperatureMonitorMenuOption, coinAcceptorMenuOption, hopperMenuOption, programsVersionsMenuOption, NULL, NULL},
	{	NULL, displayMenuOption, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
	{	NULL, allEventsLogMenuOption, creditInLogMenuOption, cashOutLogMenuOption, billAcceptedLogMenuOption, handPayLogMenuOption, progressiveLogMenuOption, voucherInLogMenuOption, voucherOutLogMenuOption, cashlessInLogMenuOption, cashlessOutLogMenuOption, hostBonusAndMysteryLogMenuOption, NULL, NULL, NULL},
	{	NULL, hoperFillMenuOption, authorizationoMenuOptin, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
	{	NULL, closeMenuOption, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

}
