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

namespace tropicalhot {

#define FPS 32

#define OFFSET_X 0
#define OFFSET_Y 0
#define SCALE_X 0.5
#define SCALE_Y 0.5

#define BUTTONS_LENGTH 11
#define MARKS_LENGTH (2 * LINES_LENGTH)

int offsets[tropicalhot::REELS_LENGTH] = { 0, 0, 0, 0, 0 };

static SDL_TimerID spinAnimationCounterUpdateTimer = NULL;

static const SDL_Surface *backgroundSurface = IMG_Load(
			"./tropicalhot/images/Background.png");

static const SDL_Surface *foregroundSurface = IMG_Load(
			"./tropicalhot/images/Foreground.png");

static const SDL_Surface *logoSurface = IMG_Load(
		"./tropicalhot/images/LogoTop.png");

static const SDL_Surface *messageBarSurface = IMG_Load(
			"./tropicalhot/images/MessageBar.png");

static const SDL_Surface *marksSurface[MARKS_LENGTH] = { IMG_Load(
		"./tropicalhot/images/LeftMark01.png"), IMG_Load(
				"./tropicalhot/images/LeftMark02.png"), IMG_Load(
						"./tropicalhot/images/LeftMark03.png"), IMG_Load(
								"./tropicalhot/images/LeftMark04.png"), IMG_Load(
										"./tropicalhot/images/LeftMark05.png"), IMG_Load(
												"./tropicalhot/images/RightMark01.png"), IMG_Load(
														"./tropicalhot/images/RightMark02.png"), IMG_Load(
																"./tropicalhot/images/RightMark03.png"), IMG_Load(
																		"./tropicalhot/images/RightMark04.png"), IMG_Load(
																				"./tropicalhot/images/RightMark05.png"),
													   };

static const SDL_Surface *symbolsSurface[] = {
	NULL, IMG_Load("./tropicalhot/images/Symbol01.png"),
	NULL, IMG_Load("./tropicalhot/images/Symbol03.png"), IMG_Load(
		"./tropicalhot/images/Symbol04.png"), IMG_Load(
			"./tropicalhot/images/Symbol05.png"), IMG_Load(
				"./tropicalhot/images/Symbol06.png"), IMG_Load(
					"./tropicalhot/images/Symbol07.png"), IMG_Load(
						"./tropicalhot/images/Symbol08.png"), IMG_Load(
							"./tropicalhot/images/Symbol09.png"), IMG_Load(
								"./tropicalhot/images/Symbol10.png"), IMG_Load(
									"./tropicalhot/images/Symbol11.png"),
	NULL,
	NULL,
	NULL, IMG_Load("./tropicalhot/images/Symbol15.png"), IMG_Load(
		"./tropicalhot/images/Symbol16.png"),
};

static const SDL_Surface *linesSurface[LINES_LENGTH] = { IMG_Load(
		"./tropicalhot/images/Line01.png"), IMG_Load(
				"./tropicalhot/images/Line02.png"), IMG_Load(
						"./tropicalhot/images/Line03.png"), IMG_Load(
								"./tropicalhot/images/Line04.png"), IMG_Load(
										"./tropicalhot/images/Line05.png"),
													   };

static const SDL_Surface *linesMarksBarsSurface[] = { IMG_Load(
		"./tropicalhot/images/RightBar.png"), IMG_Load(
				"./tropicalhot/images/LeftBar.png"),
													};

static const SDL_Surface *buttonsSurface[] = {
	NULL, //IMG_Load("./tropicalhot/images/SwapButton.png"),
	IMG_Load("./tropicalhot/images/PayoutButton.png"), IMG_Load(
		"./tropicalhot/images/SpinButton.png"), IMG_Load(
			"./tropicalhot/images/MaxBetButton.png"), IMG_Load(
				"./tropicalhot/images/BetButton.png"), IMG_Load(
					"./tropicalhot/images/CoinsButton.png"), IMG_Load(
						"./tropicalhot/images/LinesButton.png"), IMG_Load(
							"./tropicalhot/images/AutoPlayButton.png"), IMG_Load(
								"./tropicalhot/images/WinBar.png"), IMG_Load(
									"./tropicalhot/images/SpinsBar.png"), IMG_Load(
										"./tropicalhot/images/CreditBar.png"),
};

static SDL_Rect backgroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 0, 0, 0 };

static SDL_Rect foregroundCoordinates = { OFFSET_X + 0, OFFSET_Y + 773, 0, 0 };

static SDL_Rect logoCoordinates = { OFFSET_X + 349, OFFSET_Y + 0, 0, 0 };

static SDL_Rect messageBarCoordinates = { OFFSET_X + 240, OFFSET_Y + 696, 0, 0 };

static SDL_Rect linesMarksBarsCoordinates[] = { {
		OFFSET_X + 1197, OFFSET_Y + 130, 0, 0
	}, {
		OFFSET_X + 106, OFFSET_Y + 130, 0, 0
	},
};

static SDL_Rect linesCoordinates[] = { { OFFSET_X + 204, OFFSET_Y + 394, 0, 0 },
	{
		OFFSET_X + 204, OFFSET_Y + 228, 0, 0
	}, {
		OFFSET_X + 204, OFFSET_Y
		+ 553, 0, 0
	}, {
		OFFSET_X + 180, OFFSET_Y + 163, 0, 0
	}, {
		OFFSET_X + 180, OFFSET_Y
		+ 227, 0, 0
	},
};

static SDL_Rect marksCoordinates[] = { {
		OFFSET_X + 150, OFFSET_Y + 382
	}, {
		OFFSET_X + 150, OFFSET_Y + 213
	}, {
		OFFSET_X + 150, OFFSET_Y + 549
	}, {
		OFFSET_X + 150, OFFSET_Y + 145
	}, {
		OFFSET_X + 150, OFFSET_Y + 618
	}, {
		OFFSET_X + 1231, OFFSET_Y + 382
	}, {
		OFFSET_X + 1231, OFFSET_Y + 213
	}, {
		OFFSET_X + 1231, OFFSET_Y + 549
	}, {
		OFFSET_X + 1231, OFFSET_Y + 145
	}, {
		OFFSET_X + 1231, OFFSET_Y + 618
	},
};

static SDL_Rect buttonsCoordinates[] = { {
		OFFSET_X + 889, OFFSET_Y + 769
	}, {
		OFFSET_X + 860, OFFSET_Y + 846
	}, {
		OFFSET_X + 1040, OFFSET_Y + 690
	}, {
		OFFSET_X + 860, OFFSET_Y + 764
	}, {
		OFFSET_X + 606, OFFSET_Y + 760
	}, {
		OFFSET_X + 478, OFFSET_Y + 760
	}, {
		OFFSET_X + 350, OFFSET_Y + 760
	}, {
		OFFSET_X + 860, OFFSET_Y + 804
	}, {
		OFFSET_X + 734, OFFSET_Y + 760
	}, {
		OFFSET_X + 222, OFFSET_Y + 760
	}, {
		OFFSET_X + 96, OFFSET_Y + 760
	},
};

static SDL_Rect symbolsCoordinates[][3] = { { {
			OFFSET_X + 260, OFFSET_Y + 154,
			0, 0
		}, { OFFSET_X + 260, OFFSET_Y + 331, 0, 0 }, {
			OFFSET_X + 260,
			OFFSET_Y + 507, 0, 0
		}
	}, {

		{ OFFSET_X + 450, OFFSET_Y + 154, 0, 0 },
		{ OFFSET_X + 450, OFFSET_Y + 331, 0, 0 }, {
			OFFSET_X + 450, OFFSET_Y
			+ 507, 0, 0
		}
	}, {

		{ OFFSET_X + 638, OFFSET_Y + 154, 0, 0 },
		{ OFFSET_X + 638, OFFSET_Y + 331, 0, 0 }, {
			OFFSET_X + 638, OFFSET_Y
			+ 507, 0, 0
		}
	}, {

		{ OFFSET_X + 826, OFFSET_Y + 154, 0, 0 },
		{ OFFSET_X + 826, OFFSET_Y + 331, 0, 0 }, {
			OFFSET_X + 826, OFFSET_Y
			+ 507, 0, 0
		}
	}, {

		{ OFFSET_X + 1018, OFFSET_Y + 154, 0, 0 }, {
			OFFSET_X + 1018, OFFSET_Y + 331, 0,
			0
		}, { OFFSET_X + 1018, OFFSET_Y + 507, 0, 0 }
	},
};

static void drawText(int x, int y, const char *text) {
	for (int i = 0; i < strlen(text); i++) {
		SDL_Rect coord = { OFFSET_X + x + i * core::FontImages::OVERLAP,
						   OFFSET_Y + y, 0, 0
						 };
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

	core::FontImages::load("./tropicalhot/images/LCDGreen32x32.png");
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
	 * Draw foreground  image.
	 */{
		SDL_BlitSurface((SDL_Surface*) foregroundSurface, NULL, core::canvas,
						&foregroundCoordinates);
	}

	drawText(96, 784, core::CommonState::credit);
	drawText(246, 784, core::CommonState::freeGamesNumber);
	drawText(370, 784, core::CommonState::numberOfBettingLines);
	drawText(500, 784, core::CommonState::singleLineBet);
	drawText(626, 784, core::CommonState::totalBet);
	drawText(754, 784, core::CommonState::totalWin);

	if (core::CommonState::bonusSelected == false) {
		for (int i = 1; i <= 3; i++) {
			//TODO Do not use magic numbers.
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[15], NULL,
							core::canvas, &symbolsCoordinates[i][2]);
		}

		drawText(640, 695, "Select One");
	} else if (core::CommonState::bonusSelected == true) {
		for (int i = 1; i <= 3; i++) {
			if (i != core::SelectThreeBonusGame::SelectThreeBonusGame::reelBonusIndex) {
				continue;
			}

			//TODO Do not use magic numbers.
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[15], NULL,
							core::canvas, &symbolsCoordinates[i][2]);
		}

		drawText(450, 695, core::SelectThreeBonusGame::bonusOptions[0]);
		drawText(690, 695, core::SelectThreeBonusGame::bonusOptions[1]);
		drawText(930, 695, core::SelectThreeBonusGame::bonusOptions[2]);
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
	 * Draw symbols images.
	 */
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
	 * Draw foreground  image.
	 */{
		SDL_BlitSurface((SDL_Surface*) foregroundSurface, NULL, core::canvas,
						&foregroundCoordinates);
	}

	drawText(96, 784, core::CommonState::credit);
	drawText(246, 784, core::CommonState::freeGamesNumber);
	drawText(370, 784, core::CommonState::numberOfBettingLines);
	drawText(500, 784, core::CommonState::singleLineBet);
	drawText(626, 784, core::CommonState::totalBet);
	drawText(754, 784, core::CommonState::totalWin);

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
//						symbols[r60eels[i][index]]->h };
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
	 * Draw foreground  image.
	 */{
		SDL_BlitSurface((SDL_Surface*) foregroundSurface, NULL, core::canvas,
						&foregroundCoordinates);
	}

	drawText(96, 784, core::CommonState::credit);
	drawText(246, 784, core::CommonState::freeGamesNumber);
	drawText(370, 784, core::CommonState::numberOfBettingLines);
	drawText(500, 784, core::CommonState::singleLineBet);
	drawText(626, 784, core::CommonState::totalBet);
	drawText(754, 784, core::CommonState::totalWin);

	/*
	 * Double buffer animation.
	 */
	SDL_UpdateWindowSurface(core::window);
}

}
