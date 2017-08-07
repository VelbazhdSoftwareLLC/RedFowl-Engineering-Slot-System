#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "../core/FontImages.h"
#include "../core/BingoBonus.h"
#include "../core/CommonState.h"
#include "../core/Visualization.h"
#include "../core/FiniteStateMachine.h"

#include "SlotState.h"

#include "MainGameScreen.h"

using namespace std;

namespace bingotropicalhot {

#define FPS 32

#define OFFSET_X 0
#define OFFSET_Y 0
#define SCALE_X 0.5
#define SCALE_Y 0.5

#define BUTTONS_LENGTH 11
#define MARKS_LENGTH (2 * LINES_LENGTH)
#define BINGO_BALLS_LENGTH 90

int offsets[bingotropicalhot::REELS_LENGTH] = { 0, 0, 0, 0, 0 };

static SDL_TimerID spinAnimationCounterUpdateTimer = NULL;

static const SDL_Color NUMBERS_SHADOW_COLOR = { 89, 89, 95 };

static const SDL_Color BINGO_BONUS_NUMBERS_COLOR = { 236, 196, 117 };

static const SDL_Color BINGO_LINE_BONUS_NUMBERS_COLOR = { 206, 186, 97 };

static const SDL_Color NUMBERS_OUT_OF_SPHERE_COLOR = { 144, 116, 77 };

static const SDL_Color NUMBERS_IN_THE_SPHERE_COLOR = { 76, 51, 21 };

static const SDL_Surface *backgroundSurface = IMG_Load(
		"./bingotropicalhot/images/Background.png");

static const SDL_Surface *foregroundSurface = IMG_Load(
		"./bingotropicalhot/images/Foreground.png");

static const SDL_Surface *logoSurface = IMG_Load(
		"./bingotropicalhot/images/Logo.png");

static const SDL_Surface *messageBarSurface = IMG_Load(
		"./bingotropicalhot/images/MessageBar.png");

static const SDL_Surface *talonsSurface[BINGO_CARDS_TALONS] = {
		IMG_Load("./bingotropicalhot/images/BingoCard01.png"),
		IMG_Load("./bingotropicalhot/images/BingoCard02.png"),
		IMG_Load("./bingotropicalhot/images/BingoCard04.png"),
		IMG_Load("./bingotropicalhot/images/BingoCard03.png"),
		IMG_Load("./bingotropicalhot/images/BingoCard05.png"),
		IMG_Load("./bingotropicalhot/images/BingoCard06.png"), };

static const SDL_Surface *marksSurface[MARKS_LENGTH] = { IMG_Load(
		"./bingotropicalhot/images/LeftMark01.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark02.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark03.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark04.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark05.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark06.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark07.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark08.png"), IMG_Load(
		"./bingotropicalhot/images/LeftMark09.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark01.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark02.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark03.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark04.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark05.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark06.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark07.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark08.png"), IMG_Load(
		"./bingotropicalhot/images/RightMark09.png"), };

static const SDL_Surface *bingoBallsSurface[BINGO_BALLS_LENGTH] = {
		IMG_Load("./bingotropicalhot/images/BingoBall01.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall02.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall03.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall04.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall05.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall06.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall07.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall08.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall09.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall10.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall11.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall12.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall13.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall14.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall15.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall16.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall17.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall18.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall19.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall20.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall21.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall22.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall23.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall24.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall25.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall26.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall27.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall28.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall29.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall30.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall31.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall32.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall33.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall34.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall35.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall36.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall37.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall38.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall39.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall40.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall41.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall42.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall43.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall44.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall45.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall46.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall47.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall48.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall49.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall50.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall51.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall52.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall53.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall54.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall55.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall56.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall57.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall58.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall59.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall60.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall61.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall62.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall63.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall64.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall65.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall66.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall67.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall68.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall69.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall70.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall71.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall72.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall73.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall74.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall75.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall76.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall77.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall78.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall79.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall80.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall81.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall82.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall83.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall84.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall85.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall86.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall87.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall88.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall89.png"),
		IMG_Load("./bingotropicalhot/images/BingoBall90.png"), };

static const SDL_Surface *ballBarSurface = IMG_Load(
		"./bingotropicalhot/images/BallBar.png");

static const SDL_Surface *symbolsSurface[] = {
NULL,
NULL,
NULL, IMG_Load("./bingotropicalhot/images/Symbol03.png"),
		IMG_Load("./bingotropicalhot/images/Symbol04.png"),
		IMG_Load("./bingotropicalhot/images/Symbol05.png"),
		IMG_Load("./bingotropicalhot/images/Symbol06.png"),
		IMG_Load("./bingotropicalhot/images/Symbol07.png"),
		IMG_Load("./bingotropicalhot/images/Symbol08.png"),
		IMG_Load("./bingotropicalhot/images/Symbol09.png"),
		IMG_Load("./bingotropicalhot/images/Symbol10.png"),
		IMG_Load("./bingotropicalhot/images/Symbol11.png"),
		IMG_Load("./bingotropicalhot/images/Symbol12.png"),
		NULL,
		NULL,
		NULL,
		NULL, };

static const SDL_Surface *linesSurface[LINES_LENGTH] = { IMG_Load(
		"./bingotropicalhot/images/Line01.png"), IMG_Load(
		"./bingotropicalhot/images/Line02.png"), IMG_Load(
		"./bingotropicalhot/images/Line03.png"), IMG_Load(
		"./bingotropicalhot/images/Line04.png"), IMG_Load(
		"./bingotropicalhot/images/Line05.png"), IMG_Load(
		"./bingotropicalhot/images/Line06.png"), IMG_Load(
		"./bingotropicalhot/images/Line07.png"), IMG_Load(
		"./bingotropicalhot/images/Line08.png"), IMG_Load(
		"./bingotropicalhot/images/Line09.png"), };

static const SDL_Surface *linesMarksBarsSurface[] = { IMG_Load(
		"./bingotropicalhot/images/LineMarkBarRight.png"),
		IMG_Load(
				"./bingotropicalhot/images/LineMarkBarLeft.png"), };

static const SDL_Surface *buttonsSurface[] =
		{ IMG_Load(
				"./bingotropicalhot/images/PayoutButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/SpinButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/MaxBetButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/BetButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/CoinsButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/LinesButton.png"),
				IMG_Load(
						"./bingotropicalhot/images/AutoPlayButton.png"),
				IMG_Load("./bingotropicalhot/images/Win.png"),
				IMG_Load(
						"./bingotropicalhot/images/SwapButton.png"),
				IMG_Load("./bingotropicalhot/images/Spins.png"),
				IMG_Load(
						"./bingotropicalhot/images/CreditButton.png"), };

static SDL_Rect bingoBallsCoordinates =
		{ OFFSET_X + 1212, OFFSET_Y + 119, 0, 0 };

static SDL_Rect ballBarCoordinates = { OFFSET_X + 1195, OFFSET_Y + 115, 0, 0 };

static SDL_Rect foregroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect logoCoordinates = { OFFSET_X + 195, OFFSET_Y + 0, 0, 0 };

static SDL_Rect messageBarCoordinates = { OFFSET_X + 255, OFFSET_Y + 574, 0, 0 };

static SDL_Rect linesMarksBarsCoordinates[] = { {
OFFSET_X + 1121, OFFSET_Y + 137, 0, 0 }, {
OFFSET_X + 208, OFFSET_Y + 137, 0, 0 }, };

static SDL_Rect backgroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect linesCoordinates[] = { { OFFSET_X + 260, OFFSET_Y + 324, 0, 0 },
		{ OFFSET_X + 264, OFFSET_Y + 192, 0, 0 }, {
		OFFSET_X + 256, OFFSET_Y + 457, 0, 0 }, { OFFSET_X + 266, OFFSET_Y
				+ 136, 0, 0 }, {
		OFFSET_X + 256, OFFSET_Y + 203, 0, 0 }, {
		OFFSET_X + 256,
		OFFSET_Y + 232, 0, 0 }, { OFFSET_X + 253, OFFSET_Y + 232, 0, 0 }, {
		OFFSET_X + 256, OFFSET_Y + 200, 0, 0 }, {
		OFFSET_X + 261,
		OFFSET_Y + 203, 0, 0 }, };

static SDL_Rect bingoCardsCoordinates[] = { { OFFSET_X + 97, OFFSET_Y + 633 }, {
OFFSET_X + 518, OFFSET_Y + 633 }, { OFFSET_X + 938, OFFSET_Y + 633 }, {
OFFSET_X + 97, OFFSET_Y + 700 }, { OFFSET_X + 518, OFFSET_Y + 700 }, {
OFFSET_X + 938, OFFSET_Y + 700 }, };

static SDL_Rect marksCoordinates[] = { { OFFSET_X + 253, OFFSET_Y + 322 }, {
OFFSET_X + 248, OFFSET_Y + 183 }, {
OFFSET_X + 246, OFFSET_Y + 459 }, { OFFSET_X + 256, OFFSET_Y + 136 }, {
OFFSET_X + 253, OFFSET_Y + 512 }, {
OFFSET_X + 248, OFFSET_Y + 243 }, { OFFSET_X + 246, OFFSET_Y + 400 }, {
OFFSET_X + 255, OFFSET_Y + 430 }, {
OFFSET_X + 259, OFFSET_Y + 213 },

{ OFFSET_X + 1147, OFFSET_Y + 322 }, {
OFFSET_X + 1152, OFFSET_Y + 183 }, { OFFSET_X + 1153, OFFSET_Y + 459 }, {
OFFSET_X + 1141, OFFSET_Y + 136 }, {
OFFSET_X + 1145, OFFSET_Y + 512 }, { OFFSET_X + 1154, OFFSET_Y + 400 }, {
OFFSET_X + 1152, OFFSET_Y + 243 }, {
OFFSET_X + 1143, OFFSET_Y + 213 }, { OFFSET_X + 1144, OFFSET_Y + 430 }, };

static SDL_Rect buttonsCoordinates[] = { { OFFSET_X + 747, OFFSET_Y + 854 }, {
OFFSET_X + 1043, OFFSET_Y + 735 }, { OFFSET_X + 747, OFFSET_Y + 778 }, {
OFFSET_X + 531, OFFSET_Y + 777 }, { OFFSET_X + 422, OFFSET_Y + 777 }, {
OFFSET_X + 314, OFFSET_Y + 777 }, { OFFSET_X + 747, OFFSET_Y + 815 }, {
OFFSET_X + 640, OFFSET_Y + 777 }, { OFFSET_X + 889, OFFSET_Y + 769 }, {
OFFSET_X + 205, OFFSET_Y + 777 }, { OFFSET_X + 96, OFFSET_Y + 777 }, };

static SDL_Rect symbolsCoordinates[][3] = { { {
OFFSET_X + 319, OFFSET_Y + 146, 0, 0 },
		{ OFFSET_X + 319, OFFSET_Y + 281, 0, 0 }, {
		OFFSET_X + 319,
		OFFSET_Y + 417, 0, 0 } }, { { OFFSET_X + 485, OFFSET_Y + 146, 0, 0 }, {
OFFSET_X + 485, OFFSET_Y + 281, 0, 0 }, {
OFFSET_X + 485, OFFSET_Y + 417, 0, 0 } }, { { OFFSET_X + 652, OFFSET_Y + 146, 0,
		0 }, {
OFFSET_X + 652, OFFSET_Y + 281, 0, 0 }, {
OFFSET_X + 652,
OFFSET_Y + 417, 0, 0 } }, { { OFFSET_X + 818, OFFSET_Y + 146, 0, 0 }, {
OFFSET_X + 818, OFFSET_Y + 281, 0, 0 }, {
OFFSET_X + 818,
OFFSET_Y + 417, 0, 0 } }, { { OFFSET_X + 984, OFFSET_Y + 146, 0, 0 }, {
OFFSET_X + 984, OFFSET_Y + 281, 0, 0 }, {
OFFSET_X + 984,
OFFSET_Y + 417, 0, 0 } }, };

static void drawText(int x, int y, const char *text) {
	for (int i = 0; i < strlen(text); i++) {
		SDL_Rect coord = { OFFSET_X + x + i * core::FontImages::OVERLAP, OFFSET_Y + y,
				0, 0 };
		SDL_BlitSurface((SDL_Surface*) core::FontImages::SYMBOLS[(int) text[i]], NULL,
				core::canvas, &coord);
	}
}

static void drawText(int x, int y, int number) {
	char text[100] = "";
	sprintf(text, "%d", number);
	drawText(x, y, text);
}

static void drawBingoNumbers() {
	static TTF_Font *font = TTF_OpenFont(
			"./bingotropicalhot/fonts/Prototype.ttf", 16);

	char number[100] = "";
	SDL_Color color = { 0, 0, 0 };

	for (int t = 0, csx = 0, csy = 0; t < BINGO_CARDS_TALONS; t++) {
		for (int i = 0, j = 0; i < BINGO_CARDS_WIDTH; i++) {
			for (int z = 0; z < (BINGO_CARDS_LENGTH / BINGO_CARDS_TALONS);
					z++) {
				j = t * BINGO_CARDS_LENGTH / BINGO_CARDS_TALONS + z;
				if (core::bingoCards[i][j] == 0) {
					continue;
				}

				sprintf(number, "%2d", core::bingoCards[i][j]);

				/*Draw shadows for all numbers .*/{
					color = NUMBERS_SHADOW_COLOR;

					SDL_Surface *text = TTF_RenderText_Solid(font, number,
							color);
					SDL_Rect coord = { OFFSET_X + csx + 115 + i * 44 + 1,
					OFFSET_Y + csy + 633 + z * 20, 0, 0 };
					SDL_BlitSurface(text, NULL, core::canvas, &coord);
					SDL_FreeSurface(text);
				}

				if (core::bingoNumbersOut[i][j] == true) {
					if (core::bingoCardIndex == t) {
						if (core::bingoLineIndex == j) {
							/*
							 * Bingo line bonus numbers color.
							 */
							color = BINGO_LINE_BONUS_NUMBERS_COLOR;
						} else {
							/*
							 * Bingo bonus numbers color.
							 */
							color = BINGO_BONUS_NUMBERS_COLOR;
						}
					} else if (core::bingoLineIndex == j) {
						/*
						 * Bingo line bonus numbers color.
						 */
						color = BINGO_LINE_BONUS_NUMBERS_COLOR;
					} else {
						/*
						 * Numbers out of the sphere color.
						 */
						color = NUMBERS_OUT_OF_SPHERE_COLOR;
					}
				} else {
					/*
					 * Numbers in the cards still in the sphere color.
					 */
					color = NUMBERS_IN_THE_SPHERE_COLOR;
				}

				SDL_Surface *text = TTF_RenderText_Solid(font, number, color);
				SDL_Rect coord = { OFFSET_X + csx + 115 + i * 44 + 1, OFFSET_Y
						+ csy + 633 + z * 20 + 1, 0, 0 };
				SDL_BlitSurface(text, NULL, core::canvas, &coord);
				SDL_FreeSurface(text);
			}
		}

		if (t < 2) {
			csx += 418;
		} else if (t == 2) {
			csx = 0;
			csy += 67;
		} else if (t > 2) {
			csx += 418;
		}
	}
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
					SDL_BlitSurface((SDL_Surface*) symbolsSurface[core::view[i][j]], NULL,
							core::canvas, &symbolsCoordinates[i][j]);
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

	core::FontImages::load( "./bingotropicalhot/images/LCDBlue32x32.png" );
}

void deinitMainGame() {
	//TODO Free all image surfaces.
}

void drawMainGame() {
	//TODO When there is a full size background image clean is not needed.
	//SDL_FillRect(canvas, NULL, 0);

	/*
	 * Draw backgroud image.
	 */{
		SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, core::canvas,
				&backgroundCoordinates);
	}

//	/*
//	 * Draw symbols images.
//	 */
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
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[core::view[i][j]], NULL, core::canvas,
					&symbolsCoordinates[i][j]);
		}
	}

	/*
	 * Draw logo image.
	 */{
		SDL_BlitSurface((SDL_Surface*) logoSurface, NULL, core::canvas, &logoCoordinates);
	}

	/*
	 * Draw mesage bar image.
	 */{
		SDL_BlitSurface((SDL_Surface*) messageBarSurface, NULL, core::canvas,
				&messageBarCoordinates);
	}

	/*
	 * Draw bingo talons images.
	 */
	for (int i = 0; i < BINGO_CARDS_TALONS; i++) {
		SDL_BlitSurface((SDL_Surface*) talonsSurface[i], NULL, core::canvas,
				&bingoCardsCoordinates[i]);
	}

	drawBingoNumbers();

	/*
	 * Draw lines.
	 */
	for (int i = 0; i < gameWins.size(); i++) {
		SDL_BlitSurface((SDL_Surface*) linesSurface[gameWins[i].lineIndex], NULL,
				core::canvas, &linesCoordinates[gameWins[i].lineIndex]);
	}

	/*
	 * Draw lines marks bars.
	 */
	for (int i = 0; i < 2; i++) {
		SDL_BlitSurface((SDL_Surface*) linesMarksBarsSurface[i], NULL, core::canvas,
				&linesMarksBarsCoordinates[i]);
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

	/*
	 * Draw foreground  image.
	 */{
		SDL_BlitSurface((SDL_Surface*) foregroundSurface, NULL, core::canvas,
				&foregroundCoordinates);
	}

	/*
	 * Draw ball bar images.
	 */{
		SDL_BlitSurface((SDL_Surface*) ballBarSurface, NULL, core::canvas,
				&ballBarCoordinates);
	}

	/*
	 * Draw bingo ball images.
	 */
	if (1 <= core::bingoBallNumber && core::bingoBallNumber <= 90) {
		SDL_BlitSurface((SDL_Surface*) bingoBallsSurface[core::bingoBallNumber - 1], NULL,
				core::canvas, &bingoBallsCoordinates);
	}

	drawText(100, 795, core::credit);
	drawText(350, 795, core::numberOfBettingLines);
	drawText(455, 795, core::singleLineBet);
	drawText(565, 795, core::totalBet);
	drawText(675, 795, core::totalWin);

	if (core::bingoLineBonusWin > 0) {
		static char text[100] = "";
		sprintf(text, "Line Bonus %d", core::bingoLineBonusWin);
		drawText(610, 572, text);
	}
	if (core::bingoBonusWin > 0) {
		static char text[100] = "";
		sprintf(text, "Bingo Bonus %d", core::bingoBonusWin);
		drawText(610, 572, text);
	}

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

}
