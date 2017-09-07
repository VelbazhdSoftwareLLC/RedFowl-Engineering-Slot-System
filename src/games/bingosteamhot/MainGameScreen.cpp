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

namespace bingosteamhot {

#define FPS 32

#define OFFSET_X 0
#define OFFSET_Y 0
#define SCALE_X 0.5
#define SCALE_Y 0.5

#define BUTTONS_LENGTH 11
#define MARKS_LENGTH (2 * LINES_LENGTH)
#define BINGO_BALLS_LENGTH 90

int offsets[bingosteamhot::REELS_LENGTH] = { 0, 0, 0, 0, 0 };

static SDL_TimerID spinAnimationCounterUpdateTimer = NULL;

static const SDL_Color NUMBERS_SHADOW_COLOR = { 89, 89, 95 };

static const SDL_Color BINGO_BONUS_NUMBERS_COLOR = { 236, 196, 117 };

static const SDL_Color BINGO_LINE_BONUS_NUMBERS_COLOR = { 206, 186, 97 };

static const SDL_Color NUMBERS_OUT_OF_SPHERE_COLOR = { 144, 116, 77 };

static const SDL_Color NUMBERS_IN_THE_SPHERE_COLOR = { 76, 51, 21 };

static const SDL_Surface *backgroundSurface = IMG_Load(
		"./bingosteamhot/images/Background.png");

static const SDL_Surface *foregroundSurface = IMG_Load(
		"./bingosteamhot/images/Foreground.png");

static const SDL_Surface *logoSurface = IMG_Load(
		"./bingosteamhot/images/Logo.png");

static const SDL_Surface *messageBarSurface = IMG_Load(
		"./bingosteamhot/images/MessageBar.png");

static const SDL_Surface *talonsSurface[core::BingoBonus::BINGO_CARDS_TALONS] =
		{ IMG_Load("./bingosteamhot/images/BingoCard01.png"), IMG_Load(
				"./bingosteamhot/images/BingoCard02.png"), IMG_Load(
				"./bingosteamhot/images/BingoCard04.png"), IMG_Load(
				"./bingosteamhot/images/BingoCard03.png"), IMG_Load(
				"./bingosteamhot/images/BingoCard05.png"), IMG_Load(
				"./bingosteamhot/images/BingoCard06.png"), };

static const SDL_Surface *marksSurface[MARKS_LENGTH] = { IMG_Load(
		"./bingosteamhot/images/LeftMark01.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark02.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark03.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark04.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark05.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark06.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark07.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark08.png"), IMG_Load(
		"./bingosteamhot/images/LeftMark09.png"), IMG_Load(
		"./bingosteamhot/images/RightMark01.png"), IMG_Load(
		"./bingosteamhot/images/RightMark02.png"), IMG_Load(
		"./bingosteamhot/images/RightMark03.png"), IMG_Load(
		"./bingosteamhot/images/RightMark04.png"), IMG_Load(
		"./bingosteamhot/images/RightMark05.png"), IMG_Load(
		"./bingosteamhot/images/RightMark06.png"), IMG_Load(
		"./bingosteamhot/images/RightMark07.png"), IMG_Load(
		"./bingosteamhot/images/RightMark08.png"), IMG_Load(
		"./bingosteamhot/images/RightMark09.png"), };

static const SDL_Surface *bingoBallsSurface[BINGO_BALLS_LENGTH] = { IMG_Load(
		"./bingosteamhot/images/BingoBall01.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall02.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall03.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall04.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall05.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall06.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall07.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall08.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall09.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall10.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall11.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall12.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall13.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall14.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall15.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall16.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall17.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall18.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall19.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall20.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall21.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall22.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall23.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall24.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall25.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall26.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall27.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall28.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall29.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall30.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall31.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall32.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall33.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall34.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall35.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall36.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall37.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall38.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall39.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall40.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall41.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall42.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall43.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall44.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall45.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall46.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall47.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall48.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall49.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall50.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall51.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall52.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall53.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall54.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall55.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall56.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall57.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall58.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall59.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall60.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall61.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall62.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall63.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall64.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall65.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall66.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall67.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall68.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall69.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall70.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall71.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall72.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall73.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall74.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall75.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall76.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall77.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall78.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall79.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall80.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall81.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall82.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall83.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall84.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall85.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall86.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall87.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall88.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall89.png"), IMG_Load(
		"./bingosteamhot/images/BingoBall90.png"), };

static const SDL_Surface *ballBarSurface = IMG_Load(
		"./bingosteamhot/images/BallBar.png");

static const SDL_Surface *symbolsSurface[] = {
NULL,
NULL,
NULL, IMG_Load("./bingosteamhot/images/Symbol03.png"), IMG_Load(
		"./bingosteamhot/images/Symbol04.png"), IMG_Load(
		"./bingosteamhot/images/Symbol05.png"), IMG_Load(
		"./bingosteamhot/images/Symbol06.png"), IMG_Load(
		"./bingosteamhot/images/Symbol07.png"), IMG_Load(
		"./bingosteamhot/images/Symbol08.png"), IMG_Load(
		"./bingosteamhot/images/Symbol09.png"), IMG_Load(
		"./bingosteamhot/images/Symbol10.png"), IMG_Load(
		"./bingosteamhot/images/Symbol11.png"), IMG_Load(
		"./bingosteamhot/images/Symbol12.png"),
NULL,
NULL,
NULL,
NULL, };

static const SDL_Surface *linesSurface[LINES_LENGTH] = { IMG_Load(
		"./bingosteamhot/images/Line01.png"), IMG_Load(
		"./bingosteamhot/images/Line02.png"), IMG_Load(
		"./bingosteamhot/images/Line03.png"), IMG_Load(
		"./bingosteamhot/images/Line04.png"), IMG_Load(
		"./bingosteamhot/images/Line05.png"), IMG_Load(
		"./bingosteamhot/images/Line06.png"), IMG_Load(
		"./bingosteamhot/images/Line07.png"), IMG_Load(
		"./bingosteamhot/images/Line08.png"), IMG_Load(
		"./bingosteamhot/images/Line09.png"), };

static const SDL_Surface *linesMarksBarsSurface[] = { IMG_Load(
		"./bingosteamhot/images/LineMarkBarRight.png"), IMG_Load(
		"./bingosteamhot/images/LineMarkBarLeft.png"), };

static const SDL_Surface *buttonsSurface[] = { IMG_Load(
		"./bingosteamhot/images/PayoutButton.png"), IMG_Load(
		"./bingosteamhot/images/SpinButton.png"), IMG_Load(
		"./bingosteamhot/images/MaxBetButton.png"), IMG_Load(
		"./bingosteamhot/images/BetButton.png"), IMG_Load(
		"./bingosteamhot/images/CoinsButton.png"), IMG_Load(
		"./bingosteamhot/images/LinesButton.png"), IMG_Load(
		"./bingosteamhot/images/AutoPlayButton.png"), IMG_Load(
		"./bingosteamhot/images/Win.png"), IMG_Load(
		"./bingosteamhot/images/SwapButton.png"), IMG_Load(
		"./bingosteamhot/images/Spins.png"), IMG_Load(
		"./bingosteamhot/images/CreditButton.png"), };

static SDL_Rect bingoBallsCoordinates =
		{ OFFSET_X + 1239, OFFSET_Y + 265, 0, 0 };

static SDL_Rect ballBarCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect foregroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect logoCoordinates = { OFFSET_X + 192, OFFSET_Y + 0, 0, 0 };

static SDL_Rect messageBarCoordinates = { OFFSET_X + 268, OFFSET_Y + 551, 0, 0 };

static SDL_Rect linesMarksBarsCoordinates[] = { {
OFFSET_X + 1121, OFFSET_Y + 137, 0, 0 }, {
OFFSET_X + 208, OFFSET_Y + 137, 0, 0 }, };

static SDL_Rect backgroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect linesCoordinates[] = { {
OFFSET_X + 272, OFFSET_Y + 291, 0, 0 }, {
OFFSET_X + 272, OFFSET_Y + 145, 0, 0 }, {
OFFSET_X + 272, OFFSET_Y + 435, 0, 0 }, {
OFFSET_X + 272, OFFSET_Y + 124, 0, 0 }, {
OFFSET_X + 246, OFFSET_Y + 107, 0, 0 }, {
OFFSET_X + 266, OFFSET_Y + 175, 0, 0 }, {
OFFSET_X + 266, OFFSET_Y + 161, 0, 0 }, {
OFFSET_X + 270, OFFSET_Y + 97, 0, 0 }, {
OFFSET_X + 265, OFFSET_Y + 104, 0, 0 }, };

static SDL_Rect bingoCardsCoordinates[] = { { OFFSET_X + 92, OFFSET_Y + 606 }, {
OFFSET_X + 518, OFFSET_Y + 606 }, { OFFSET_X + 943, OFFSET_Y + 606 }, {
OFFSET_X + 92, OFFSET_Y + 677 }, { OFFSET_X + 518, OFFSET_Y + 677 }, {
OFFSET_X + 943, OFFSET_Y + 677 }, };

static SDL_Rect marksCoordinates[] = { {
OFFSET_X + 204, OFFSET_Y + 320 }, {
OFFSET_X + 204, OFFSET_Y + 177 }, {
OFFSET_X + 204, OFFSET_Y + 464 }, {
OFFSET_X + 204, OFFSET_Y + 124 }, {
OFFSET_X + 204, OFFSET_Y + 517 }, {
OFFSET_X + 204, OFFSET_Y + 231 }, {
OFFSET_X + 204, OFFSET_Y + 410 }, {
OFFSET_X + 204, OFFSET_Y + 437 }, {
OFFSET_X + 204, OFFSET_Y + 204 },

{
OFFSET_X + 1201, OFFSET_Y + 320 }, {
OFFSET_X + 1201, OFFSET_Y + 177 }, {
OFFSET_X + 1201, OFFSET_Y + 464 }, {
OFFSET_X + 1201, OFFSET_Y + 124 }, {
OFFSET_X + 1201, OFFSET_Y + 517 }, {
OFFSET_X + 1201, OFFSET_Y + 410 }, {
OFFSET_X + 1201, OFFSET_Y + 231 }, {
OFFSET_X + 1201, OFFSET_Y + 204 }, {
OFFSET_X + 1201, OFFSET_Y + 437 }, };

static SDL_Rect buttonsCoordinates[] = { { OFFSET_X + 757, OFFSET_Y + 746 }, {
OFFSET_X + 1161, OFFSET_Y + 735 }, { OFFSET_X + 757, OFFSET_Y + 792 }, {
OFFSET_X + 519, OFFSET_Y + 758 }, { OFFSET_X + 411, OFFSET_Y + 758 }, {
OFFSET_X + 302, OFFSET_Y + 758 }, { OFFSET_X + 757, OFFSET_Y + 839 }, {
OFFSET_X + 628, OFFSET_Y + 758 }, { OFFSET_X + 965, OFFSET_Y + 746 }, {
OFFSET_X + 193, OFFSET_Y + 758 }, { OFFSET_X + 84, OFFSET_Y + 758 }, };

static SDL_Rect symbolsCoordinates[][3] = { { {
OFFSET_X + 298, OFFSET_Y + 118, 0, 0 },
		{ OFFSET_X + 298, OFFSET_Y + 266, 0, 0 }, { OFFSET_X + 298, OFFSET_Y
				+ 414, 0, 0 } }, { { OFFSET_X + 474, OFFSET_Y + 118, 0, 0 }, {
OFFSET_X + 474, OFFSET_Y + 266, 0, 0 }, {
OFFSET_X + 474, OFFSET_Y + 414, 0, 0 } }, { { OFFSET_X + 651, OFFSET_Y + 118, 0,
		0 }, {
OFFSET_X + 651, OFFSET_Y + 266, 0, 0 }, {
OFFSET_X + 651, OFFSET_Y + 414, 0, 0 } }, { { OFFSET_X + 827, OFFSET_Y + 118, 0,
		0 }, {
OFFSET_X + 827, OFFSET_Y + 266, 0, 0 }, {
OFFSET_X + 827, OFFSET_Y + 414, 0, 0 } }, { { OFFSET_X + 1003, OFFSET_Y + 118,
		0, 0 }, {
OFFSET_X + 1003, OFFSET_Y + 266, 0, 0 }, {
OFFSET_X + 1003, OFFSET_Y + 414, 0, 0 } }, };

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

static void drawBingoNumbers() {
	static TTF_Font *font = TTF_OpenFont("./bingosteamhot/fonts/Prototype.ttf",
			16);

	char number[100] = "";
	SDL_Color color = { 0, 0, 0 };

	for (int t = 0, csx = 0, csy = 0; t < core::BingoBonus::BINGO_CARDS_TALONS;
			t++) {
		for (int i = 0, j = 0; i < core::BingoBonus::BINGO_CARDS_WIDTH; i++) {
			for (int z = 0;
					z
							< (core::BingoBonus::BINGO_CARDS_LENGTH
									/ core::BingoBonus::BINGO_CARDS_TALONS);
					z++) {
				j = t * core::BingoBonus::BINGO_CARDS_LENGTH
						/ core::BingoBonus::BINGO_CARDS_TALONS + z;
				if (core::BingoBonus::bingoCards[i][j] == 0) {
					continue;
				}

				sprintf(number, "%2d", core::BingoBonus::bingoCards[i][j]);

				/*Draw shadows for all numbers .*/{
					color = NUMBERS_SHADOW_COLOR;

					SDL_Surface *text = TTF_RenderText_Solid(font, number,
							color);
					SDL_Rect coord = { OFFSET_X + csx + 100 + i * 44 + 1,
					OFFSET_Y + csy + 605 + z * 20, 0, 0 };
					SDL_BlitSurface(text, NULL, core::canvas, &coord);
					SDL_FreeSurface(text);
				}

				if (core::BingoBonus::bingoNumbersOut[i][j] == true) {
					if (core::BingoBonus::bingoCardIndex == t) {
						if (core::BingoBonus::bingoLineIndex == j) {
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
					} else if (core::BingoBonus::bingoLineIndex == j) {
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
				SDL_Rect coord = { OFFSET_X + csx + 100 + i * 44 + 1, OFFSET_Y
						+ csy + 605 + z * 20 + 1, 0, 0 };
				SDL_BlitSurface(text, NULL, core::canvas, &coord);
				SDL_FreeSurface(text);
			}
		}

		if (t < 2) {
			csx += 426;
		} else if (t == 2) {
			csx = 0;
			csy += 72;
		} else if (t > 2) {
			csx += 426;
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
					if (core::CommonState::view[i][j] == EMPTY) {
						continue;
					}
					SDL_BlitSurface(
							(SDL_Surface*) symbolsSurface[core::CommonState::view[i][j]],
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

	core::FontImages::load("./bingosteamhot/images/LCDBlue32x32.png");
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
			if (core::CommonState::view[i][j] == EMPTY) {
				continue;
			}
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[core::CommonState::view[i][j]],
			NULL, core::canvas, &symbolsCoordinates[i][j]);
		}
	}

	/*
	 * Draw logo image.
	 */{
		SDL_BlitSurface((SDL_Surface*) logoSurface, NULL, core::canvas,
				&logoCoordinates);
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
	for (int i = 0; i < core::BingoBonus::BINGO_CARDS_TALONS; i++) {
		SDL_BlitSurface((SDL_Surface*) talonsSurface[i], NULL, core::canvas,
				&bingoCardsCoordinates[i]);
	}

	drawBingoNumbers();

	/*
	 * Draw lines.
	 */
	for (int i = 0; i < gameWins.size(); i++) {
		SDL_BlitSurface((SDL_Surface*) linesSurface[gameWins[i].lineIndex],
		NULL, core::canvas, &linesCoordinates[gameWins[i].lineIndex]);
	}

	/*
	 * Draw lines marks bars.
	 */
	for (int i = 0; i < 2; i++) {
		SDL_BlitSurface((SDL_Surface*) linesMarksBarsSurface[i], NULL,
				core::canvas, &linesMarksBarsCoordinates[i]);
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
	 * Draw ball bar images.
	 */{
		SDL_BlitSurface((SDL_Surface*) ballBarSurface, NULL, core::canvas,
				&ballBarCoordinates);
	}

	/*
	 * Draw bingo ball images.
	 */
	if (1 <= core::CommonState::bingoBallNumber && core::CommonState::bingoBallNumber <= 90) {
		SDL_BlitSurface(
				(SDL_Surface*) bingoBallsSurface[core::CommonState::bingoBallNumber - 1],
				NULL, core::canvas, &bingoBallsCoordinates);
	}

	/*
	 * Draw foreground  image.
	 */{
		SDL_BlitSurface((SDL_Surface*) foregroundSurface, NULL, core::canvas,
				&foregroundCoordinates);
	}

	drawText(100, 801, core::CommonState::credit);
	drawText(350, 801, core::CommonState::numberOfBettingLines);
	drawText(455, 801, core::CommonState::singleLineBet);
	drawText(565, 801, core::CommonState::totalBet);
	drawText(645, 801, core::CommonState::totalWin);

	if (core::CommonState::bingoLineBonusWin > 0) {
		static char text[100] = "";
		sprintf(text, "Line Bonus %d", core::CommonState::bingoLineBonusWin);
		drawText(610, 554, text);
	}
	if (core::CommonState::bingoBonusWin > 0) {
		static char text[100] = "";
		sprintf(text, "Bingo Bonus %d", core::CommonState::bingoBonusWin);
		drawText(610, 554, text);
	}

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

}
