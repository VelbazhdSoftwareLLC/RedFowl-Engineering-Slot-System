#include <iostream>

#include <cdk/cdk.h>

#include "MenuLabels.h"
#include "TuiPointers.h"
#include "ApplicationState.h"

#include "SlotManagementSystem.h"

namespace management {

/*
 * This gets called after every movement.
 */
static int onMove(EObjectType type, void *object, void *data, chtype input) {
	CDKMENU *menu = (CDKMENU *) object;
	return 0;
}

int main1(int argc, char *argv[], unsigned long &session) {
	CDKMENU *menu = newCDKMenu(screen, MenuLabels::VALUES, MenuLabels::WIDTH, MenuLabels::HEIGHT, MenuLabels::LOCATION, TOP, A_UNDERLINE, A_REVERSE);

	/*
	 * Create the post process function.
	 */
	setCDKMenuPostProcess(menu, onMove, NULL);

	/*
	 * Draw the CDK screen.
	 */
	refreshCDKScreen(screen);

	while (applicationDone == false) {
		/*
		 * Activate the menu.
		 */
		int selection = activateCDKMenu(menu, (chtype *) NULL);

		/*
		 * Determine how the user exitted from the widget.
		 */
		if (menu->exitType
				== vNORMAL&& MenuLabels::FUNCTIONS[selection / 100][selection % 100 + 1] != NULL) {
			MenuLabels::FUNCTIONS[selection / 100][selection % 100 + 1]();
		}
	}

	destroyCDKMenu(menu);

	return (EXIT_SUCCESS);
}

}
