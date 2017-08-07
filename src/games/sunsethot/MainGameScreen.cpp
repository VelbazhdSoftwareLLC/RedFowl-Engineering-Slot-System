#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "../core/FontImages.h"
#include "../core/CommonState.h"
#include "../core/Visualization.h"
#include "../core/FiniteStateMachine.h"
#include "../core/SelectThreeBonusGame.h"

#include "SlotState.h"

#include "MainGameScreen.h"

using namespace std;

namespace sunsethot {

#define FPS 32

#define OFFSET_X 0
#define OFFSET_Y 0
#define SCALE_X 0.5
#define SCALE_Y 0.5

#define BUTTONS_LENGTH 11
#define MARKS_LENGTH (2 * LINES_LENGTH)

int offsets[sunsethot::REELS_LENGTH] = { 0, 0, 0, 0, 0 };

static SDL_TimerID spinAnimationCounterUpdateTimer = NULL;

static const SDL_Surface *backgroundSurface = IMG_Load(
		"./sunsethot/images/Background.png");

static const SDL_Surface *marksSurface[MARKS_LENGTH] = {
		IMG_Load("./sunsethot/images/LeftMark01.png"),
		IMG_Load("./sunsethot/images/LeftMark02.png"),
		IMG_Load("./sunsethot/images/LeftMark03.png"),
		IMG_Load("./sunsethot/images/LeftMark04.png"),
		IMG_Load("./sunsethot/images/LeftMark05.png"),
		IMG_Load("./sunsethot/images/RightMark01.png"),
		IMG_Load("./sunsethot/images/RightMark02.png"),
		IMG_Load("./sunsethot/images/RightMark03.png"),
		IMG_Load("./sunsethot/images/RightMark04.png"),
		IMG_Load("./sunsethot/images/RightMark05.png"), };

static const SDL_Surface *symbolsSurface[] = {
NULL, IMG_Load("./sunsethot/images/Symbol01.png"),
NULL, IMG_Load("./sunsethot/images/Symbol03.png"),
		IMG_Load("./sunsethot/images/Symbol04.png"),
		IMG_Load("./sunsethot/images/Symbol05.png"),
		IMG_Load("./sunsethot/images/Symbol06.png"),
		IMG_Load("./sunsethot/images/Symbol07.png"),
		IMG_Load("./sunsethot/images/Symbol08.png"),
		IMG_Load("./sunsethot/images/Symbol09.png"),
		IMG_Load("./sunsethot/images/Symbol10.png"),
		IMG_Load("./sunsethot/images/Symbol11.png"),
		NULL,
		NULL,
		NULL, IMG_Load("./sunsethot/images/Symbol15.png"),
		IMG_Load("./sunsethot/images/Symbol16.png"), };

static const SDL_Surface *linesSurface[LINES_LENGTH] = {
		IMG_Load("./sunsethot/images/Line01.png"),
		IMG_Load("./sunsethot/images/Line02.png"),
		IMG_Load("./sunsethot/images/Line03.png"),
		IMG_Load("./sunsethot/images/Line04.png"),
		IMG_Load("./sunsethot/images/Line05.png"), };

static const SDL_Surface *buttonsSurface[] = { IMG_Load(
		"./sunsethot/images/PayoutButton.png"), IMG_Load(
		"./sunsethot/images/SpinButton.png"), IMG_Load(
		"./sunsethot/images/MaxBetButton.png"), NULL, IMG_Load(
		"./sunsethot/images/CoinsButton.png"), IMG_Load(
		"./sunsethot/images/LinesButton.png"), IMG_Load(
		"./sunsethot/images/AutoPlayButton.png"), NULL,
		IMG_Load("./sunsethot/images/SwapButton.png"), NULL,
		NULL, };

static SDL_Rect backgroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect linesCoordinates[] = { { OFFSET_X + 21, OFFSET_Y + 1, 0, 0 }, {
OFFSET_X + 19, OFFSET_Y + 1, 0, 0 }, { OFFSET_X + 16, OFFSET_Y + 1, 0, 0 }, {
OFFSET_X + 19, OFFSET_Y + 1, 0, 0 }, { OFFSET_X + 19, OFFSET_Y + 1, 0, 0 }, };

static SDL_Rect marksCoordinates[] = { {
OFFSET_X + 40, OFFSET_Y + 362 }, {
OFFSET_X + 40, OFFSET_Y + 195 }, {
OFFSET_X + 40, OFFSET_Y + 536 }, {
OFFSET_X + 40, OFFSET_Y + 113 }, {
OFFSET_X + 40, OFFSET_Y + 619 }, {
OFFSET_X + 1337, OFFSET_Y + 362 }, {
OFFSET_X + 1337, OFFSET_Y + 195 }, {
OFFSET_X + 1337, OFFSET_Y + 536 }, {
OFFSET_X + 1337, OFFSET_Y + 113 }, {
OFFSET_X + 1337, OFFSET_Y + 619 }, };

static SDL_Rect buttonsCoordinates[] = { { OFFSET_X + 50, OFFSET_Y + 753 }, {
OFFSET_X + 1228, OFFSET_Y + 795 }, { OFFSET_X + 1055, OFFSET_Y + 795 }, {
OFFSET_X + 606, OFFSET_Y + 760 }, { OFFSET_X + 883, OFFSET_Y + 795 }, {
OFFSET_X + 712, OFFSET_Y + 795 }, { OFFSET_X + 540, OFFSET_Y + 795 }, {
OFFSET_X + 640, OFFSET_Y + 777 }, { OFFSET_X + 266, OFFSET_Y + 770 }, {
OFFSET_X + 205, OFFSET_Y + 777 }, { OFFSET_X + 96, OFFSET_Y + 777 }, };

static SDL_Rect symbolsCoordinates[][3] = { { {
OFFSET_X + 190, OFFSET_Y + 168, 0, 0 },
		{ OFFSET_X + 190, OFFSET_Y + 337, 0, 0 }, {
		OFFSET_X + 190,
		OFFSET_Y + 498, 0, 0 } }, { { OFFSET_X + 428, OFFSET_Y + 168, 0, 0 }, {
OFFSET_X + 428, OFFSET_Y + 337, 0, 0 }, {
OFFSET_X + 428, OFFSET_Y + 498, 0, 0 } }, { { OFFSET_X + 659, OFFSET_Y + 168, 0,
		0 }, {
OFFSET_X + 659, OFFSET_Y + 337, 0, 0 }, {
OFFSET_X + 659,
OFFSET_Y + 498, 0, 0 } }, { { OFFSET_X + 892, OFFSET_Y + 168, 0, 0 }, {
OFFSET_X + 892, OFFSET_Y + 337, 0, 0 }, {
OFFSET_X + 892,
OFFSET_Y + 498, 0, 0 } }, { { OFFSET_X + 1125, OFFSET_Y + 168, 0, 0 }, {
OFFSET_X + 1125, OFFSET_Y + 337, 0, 0 }, {
OFFSET_X + 1125,
OFFSET_Y + 498, 0, 0 } }, };

static void drawText(int x, int y, const char *text) {
	for (int i = 0; i < strlen(text); i++) {
		SDL_Rect coord = { OFFSET_X + x + i * core::FontImages::OVERLAP,
		OFFSET_Y + y, 0, 0 };
		SDL_BlitSurface((SDL_Surface*) core::FontImages::SYMBOLS[(int) text[i]],
		NULL, core::canvas, &coord);
	}
}

static void drawText(int x, int y, int number) {
	char text[100] = "";
	sprintf(text, "%d", number);
	drawText(x, y, text);
}

void initMainGame() {
	int width = 0;
	int height = 0;
	int index = -1;
	for (int s = 0; s < SYMBOLS_LENGTH; s++) {
		if (symbolsSurface[s] != NULL) {
			index = s;
			for (int j = 0; j < ROWS_LENGTH; j++) {
				for (int i = 0; i < REELS_LENGTH; i++) {
					if (core::view[i][j] == EMPTY) {
						continue;
					}
					SDL_BlitSurface(
							(SDL_Surface*) symbolsSurface[core::view[i][j]],
							NULL, core::canvas, &symbolsCoordinates[i][j]);
				}
			}

			if (symbolsSurface[s]->w > width) {
				width = symbolsSurface[s]->w;
			}
			if (symbolsSurface[s]->h > height) {
				height = symbolsSurface[s]->h;
			}
		}
	}

	core::FontImages::load("./sunsethot/images/LCDRed32x32.png");
}

void deinitMainGame() {
	//TODO Free all image surfaces.
}

//TODO Move to BonusGameScreen.cpp
void drawBonusGame() {
	/*
	 * Draw backgroud image.
	 */
	SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, core::canvas,
			&backgroundCoordinates);

	/*
	 * Draw buttons.
	 */
	for (int i = 0; i < BUTTONS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) buttonsSurface[i], NULL, core::canvas,
				&buttonsCoordinates[i]);
	}

	/*
	 * Draw marks images.
	 */
	for (int i = 0; i < MARKS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) marksSurface[i], NULL, core::canvas,
				&marksCoordinates[i]);
	}

	drawText(86, 834, core::credit);
	drawText(620, 750, core::freeGamesNumber);
	drawText(780, 750, core::numberOfBettingLines);
	drawText(950, 750, core::singleLineBet);
	drawText(1120, 750, core::totalBet);
	drawText(1278, 750, core::totalWin);

	if (core::bonusSelected == false) {
		for (int i = 1; i <= 3; i++) {
			//TODO Do not use magic numbers.
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[15], NULL,
					core::canvas, &symbolsCoordinates[i][2]);
		}

		drawText(640, 695, "Select One");
	} else if (core::bonusSelected == true) {
		for (int i = 1; i <= 3; i++) {
			if (i != core::reelBonusIndex) {
				continue;
			}

			//TODO Do not use magic numbers.
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[15], NULL,
					core::canvas, &symbolsCoordinates[i][2]);
		}

		drawText(450, 695, core::bonusOptions[0]);
		drawText(690, 695, core::bonusOptions[1]);
		drawText(930, 695, core::bonusOptions[2]);
	}

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

//TODO Move to FreeGamesScreen.cpp
void drawFreeGame() {
	/*
	 * Draw backgroud image.
	 */
	SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, core::canvas,
			&backgroundCoordinates);

	/*
	 * Draw symbols images.
	 */
	for (int j = 0; j < ROWS_LENGTH; j++) {
		for (int i = 0; i < REELS_LENGTH; i++) {
			if (core::view[i][j] == EMPTY) {
				continue;
			}
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[core::view[i][j]],
			NULL, core::canvas, &symbolsCoordinates[i][j]);
		}
	}

	/*
	 * Draw lines.
	 */
	for (int i = 0; i < gameWins.size(); i++) {
		SDL_BlitSurface((SDL_Surface*) linesSurface[gameWins[i].lineIndex],
				NULL, core::canvas, &linesCoordinates[gameWins[i].lineIndex]);
	}

	/*
	 * Draw buttons.
	 */
	for (int i = 0; i < BUTTONS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) buttonsSurface[i], NULL, core::canvas,
				&buttonsCoordinates[i]);
	}

	/*
	 * Draw marks images.
	 */
	for (int i = 0; i < MARKS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) marksSurface[i], NULL, core::canvas,
				&marksCoordinates[i]);
	}

	drawText(86, 834, core::credit);
	drawText(620, 750, core::freeGamesNumber);
	drawText(780, 750, core::numberOfBettingLines);
	drawText(950, 750, core::singleLineBet);
	drawText(1120, 750, core::totalBet);
	drawText(1278, 750, core::totalWin);

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

void drawMainGame() {
	//TODO When there is a full size background image clean is not needed.
	//SDL_FillRect(canvas, NULL, 0);

	/*
	 * Draw backgroud image.
	 */
	SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, core::canvas,
			&backgroundCoordinates);

	/*
	 * Draw symbols images.
	 */
//	for (int i = 0; i < REELS_LENGTH; i++) {
//		/*
//		 * If the reel is not spinning draw static picture. Draw next animation frame otherwise.
//		 */
//		if (reelsSpinFlag[i] == false) {
//			for (int j = 0; j < ROWS_LENGTH; j++) {
//				if (view[i][j] == EMPTY) {
//					continue;
//				}
//				SDL_BlitSurface((SDL_Surface*) symbols[view[i][j]], NULL,
//						canvas, &symbolsCoordinates[i][j]);
//			}
//		} else {
//			static int velocities[REELS_LENGTH] = { 200, 300, 400, 500, 600 };
//
//			offsets[i] = (SDL_GetTicks() - lastSpinTick) / 1000.f
//					* velocities[i];
//			int length = 0;
//			for (int j = 0; j < ROWS_LENGTH; j++) {
//				length += symbols[reels[i][j]]->h;
//			}
//
//			int index;
//			int dy = offsets[i] % length;
//			length = 0;
//			for (index = 0; index < ROWS_LENGTH; index++) {
//				length += symbols[reels[i][index]]->h;
//				if (dy < length) {
//					break;
//				}
//			}
//
//			Uint16 b = length - dy;
//			Uint16 a = symbols[reels[i][index]]->h - b;
//			Sint16 c = symbolsCoordinates[i][0].y - 3;
//
//			/*
//			 * Show part of the first symbol.
//			 */
//			if (b > 0) {
//				SDL_Rect coordinates = { 0, a, symbols[reels[i][index]]->w, b };
//				SDL_Rect position = { symbolsCoordinates[i][0].x, c, 0, 0 };
//				SDL_BlitSurface((SDL_Surface *) symbols[reels[i][index]],
//						&coordinates, canvas, &position);
//				c += b;
//			}
//
//			/*
//			 * Show full second symbol.
//			 */{
//				index = (index + 1) % REELS_LENGTH;
//				SDL_Rect coordinates = { 0, 0, symbols[reels[i][index]]->w,
//						symbols[reels[i][index]]->h };
//				SDL_Rect position = { symbolsCoordinates[i][0].x, c, 0, 0 };
//				SDL_BlitSurface((SDL_Surface *) symbols[reels[i][index]],
//						&coordinates, canvas, &position);
//				c += symbols[reels[i][index]]->h;
//			}
//
//			/*
//			 * Show full third symbol.
//			 */{
//				index = (index + 1) % REELS_LENGTH;
//				SDL_Rect coordinates = { 0, 0, symbols[reels[i][index]]->w,
//						symbols[reels[i][index]]->h };
//				SDL_Rect position = { symbolsCoordinates[i][0].x, c, 0, 0 };
//				SDL_BlitSurface((SDL_Surface *) symbols[reels[i][index]],
//						&coordinates, canvas, &position);
//				c += symbols[reels[i][index]]->h;
//			}
//
//			/*
//			 * Show part of the fourth symbol.
//			 */{
//				index = (index + 1) % REELS_LENGTH;
//				SDL_Rect coordinates = { 0, 0, symbols[reels[i][index]]->w, a };
//				SDL_Rect position = { symbolsCoordinates[i][0].x, c, 0, 0 };
//				SDL_BlitSurface((SDL_Surface *) symbols[reels[i][index]],
//						&coordinates, canvas, &position);
//			}
//		}
//	}
	for (int j = 0; j < ROWS_LENGTH; j++) {
		for (int i = 0; i < REELS_LENGTH; i++) {
			if (core::view[i][j] == EMPTY) {
				continue;
			}
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[core::view[i][j]],
			NULL, core::canvas, &symbolsCoordinates[i][j]);
		}
	}

	/*
	 * Draw lines.
	 */
	for (int i = 0; i < gameWins.size(); i++) {
		SDL_BlitSurface((SDL_Surface*) linesSurface[gameWins[i].lineIndex],
				NULL, core::canvas, &linesCoordinates[gameWins[i].lineIndex]);
	}

	/*
	 * Draw buttons.
	 */
	for (int i = 0; i < BUTTONS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) buttonsSurface[i], NULL, core::canvas,
				&buttonsCoordinates[i]);
	}

	/*
	 * Draw marks images.
	 */
	for (int i = 0; i < MARKS_LENGTH; i++) {
		SDL_BlitSurface((SDL_Surface*) marksSurface[i], NULL, core::canvas,
				&marksCoordinates[i]);
	}

	drawText(86, 834, core::credit);
	drawText(620, 750, core::freeGamesNumber);
	drawText(780, 750, core::numberOfBettingLines);
	drawText(950, 750, core::singleLineBet);
	drawText(1120, 750, core::totalBet);
	drawText(1278, 750, core::totalWin);

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

}
