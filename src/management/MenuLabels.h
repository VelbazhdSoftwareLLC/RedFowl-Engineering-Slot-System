#ifndef MANAGEMENT_MENULABELS_H_
#define MANAGEMENT_MENULABELS_H_

#include <string>

#include <cdk.h>

namespace management {

/**
 * Menu labels are used only for initialization. It is done by this way in order
 * if there is a change in menu strings the change to be done only in one place.
 * By this way relatively easy language can be changed.
 *
 * @author Todor Balabanov
 */
struct MenuLabels {

	/*
	 * It is used for array size.
	 */
	static const int WIDTH = 6;

	/*
	 * It is used for array size.
	 */
	static int HEIGHT[];

	/*
	 * Location on the screen.
	 */
	static int LOCATION[];

	/*
	 * First row is full with menus and all other rows are menu items. If there
	 * is empty string it means that there is no menu or menu item.
	 */
	static const char *VALUES[MAX_MENU_ITEMS][MAX_SUB_ITEMS];

	/*
	 * Each menu lable can call a particular function by function pointer.
	 */
	static void (* const FUNCTIONS[MAX_MENU_ITEMS][MAX_SUB_ITEMS])();
};

}

#endif
