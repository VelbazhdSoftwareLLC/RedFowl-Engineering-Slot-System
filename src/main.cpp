#include <cstdlib>

#include "./management/TuiPointers.h"
#include "./management/CreditManagement.h"
#include "./management/SlotManagementSystem.h"
#include "./games/sunsethot/SunsetHot.h"
#include "./games/tropicalhot/TropicalHot.h"
#include "./games/bingosteamhot/BingoSteamHot.h"
#include "./games/bingotropicalhot/BingoTropicalHot.h"

using namespace sunsethot;
using namespace management;
using namespace tropicalhot;
using namespace bingosteamhot;
using namespace bingotropicalhot;

/**
 * It should be zero if there is no open session.
 */
unsigned long session = 1;

int main(int argc, char *argv[]) {
	/*
	 * Initialize screen and colors.
	 */
	management::window = initscr();
	management::screen = initCDKScreen(window);
	initCDKColor();

	/*
	 * All screens demo.
	 */
	main2(argc, argv, session);
	main10(argc, argv, session);
	main11(argc, argv, session);
	main12(argc, argv, session);
	main13(argc, argv, session);
	main2(argc, argv, session);
	main1(argc, argv, session);

	/*
	 * Accounting system entry point.
	 */
	//main1(argc, argv, session);
	/*
	 * Credit management entry point.
	 */
	//main2(argc, argv, session);
	/*
	 * Game entry point.
	 */
	//main10(argc, argv, session);
	//main11(argc, argv, session);
	//main12(argc, argv, session);
	//main13(argc, argv, session);
	destroyCDKScreen(management::screen);
	delwin(management::window);
	endCDK();
	endwin();

	return (EXIT_SUCCESS);
}
