#include <SDL2/SDL.h>

#include "../core/CommonState.h"
#include "../core/Calculations.h"
#include "../core/Visualization.h"
#include "../core/FiniteStateMachine.h"

#include "MainGameScreen.h"
#include "SlotDefinition.h"
#include "SlotCalculations.h"

#include "EventLoop.h"

namespace sunsethot {

void loop() {
	/*
	 * Initialize bet parameters.
	 */
	core::singleLineBet = MIN_SINGLE_LINE_BET;
	core::numberOfBettingLines = MAX_SELECTED_LINES;
	core::totalBet = core::numberOfBettingLines * core::singleLineBet;

	/*
	 * Initialize symbols view.
	 */
	core::view.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::view[i].resize(ROWS_LENGTH);
		for (int j = 0; j < ROWS_LENGTH; j++) {
			core::view[i][j] = EMPTY;
		}
	}

	/*
	 * Initialize reels stops offsets.
	 */
	core::reelsMinOffset.resize(REELS_LENGTH);
	core::reelsMaxOffset.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::reelsMinOffset[i] = reelsMinOffset[i];
		core::reelsMaxOffset[i] = reelsMaxOffset[i];
	}

	/*
	 * Initialize reels stops.
	 */
	core::reelsStops.resize(REELS_LENGTH);
	core::reelsSpinFlag.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::reelsStops[i] = 0;
		core::reelsSpinFlag[i] = false;
	}

	/*
	 * RTP should be loaded from database.
	 */
	core::baseReels.resize(REELS_LENGTH);
	core::freeReels.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::baseReels[i].resize(REEL_LENGTH);
		core::freeReels[i].resize(REEL_LENGTH);
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::baseReels[i][j] = baseGameReels[core::rtp - 90][i][j];
			core::freeReels[i][j] = freeGamesReels[core::rtp - 90][i][j];
		}
	}

	//TODO Find better place to initialize decorative initial view.
	core::Calculations::generatDecorative(core::baseReels);

	initMainGame();

	SDL_Event event;
	bool done = false;
	while (done == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = true;
				break;

				/*
				 * Read keyboard.
				 */
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {

				case SDLK_2:
				case SDLK_3:
				case SDLK_4:
					if (core::bonusGameMode == true) {
						int index = -1;

						if (event.key.keysym.sym == SDLK_2) {
							index = 1;
						}
						if (event.key.keysym.sym == SDLK_3) {
							index = 2;
						}
						if (event.key.keysym.sym == SDLK_4) {
							index = 3;
						}

						core::bonusSelected = true;
						runBonusGame(core::totalWin, index);
						core::lastSpinTick = SDL_GetTicks();
					}
					break;

				case SDLK_h:
					/*
					 * Print help screen.
					 */
					//TODO Run help.
					break;

				case SDLK_ESCAPE:
					/*
					 * Quit application.
					 */
					done = true;
					break;

					/*
					 * Number of lines changed by user.
					 */
				case SDLK_l:
					core::numberOfBettingLines++;

					if (core::numberOfBettingLines > MAX_SELECTED_LINES) {
						core::numberOfBettingLines = MIN_SELECTED_LINES;
					}

					core::totalBet = core::numberOfBettingLines
							* core::singleLineBet;
					break;

					/*
					 * Bet changed by user.
					 */
				case SDLK_b:
					core::singleLineBet++;

					if (core::singleLineBet > MAX_SINGLE_LINE_BET) {
						core::singleLineBet = MIN_SINGLE_LINE_BET;
					}

					core::totalBet = core::numberOfBettingLines
							* core::singleLineBet;
					break;

					/*
					 * Max bet changed by user.
					 */
				case SDLK_m:
					core::singleLineBet = MAX_SINGLE_LINE_BET;
					core::numberOfBettingLines = MAX_SELECTED_LINES;

					core::totalBet = core::numberOfBettingLines
							* core::singleLineBet;
					break;

				case SDLK_c:
					if (core::credit + CREDIT_INCREMENT <= MAX_CREDIT) {
						core::credit += CREDIT_INCREMENT;
					}
					break;

					/*
					 * Spin reels by user.
					 */
				case SDLK_RETURN:
#ifdef SIMULATION
					break;
#endif
#ifdef OPTIMIZATION
					break;
#endif

					if (core::bonusGameMode == true) {
						break;
					}

					if (core::freeGamesMode == true) {
						break;
					}

					/*
					 * Switch off auto play if spin button is used.
					 */
					if (core::autoPlayMode == true) {
						core::autoPlayMode = false;
					}

					if (core::totalBet <= 0) {
						//TODO Report no credit.
						//std::cerr << "Total bet is empty ..." << std::endl;
						break;
					}

					if (core::credit < core::totalBet) {
						//TODO Report no credit.
						//std::cerr << "No enough credit ..." << std::endl;
						break;
					}

					if (core::freeGamesNumber <= 0) {
						core::spinAllReels();
						runBaseGame(core::totalWin);
						core::lastSpinTick = SDL_GetTicks();
						memset(offsets, 0, REELS_LENGTH * sizeof(int));

						/*
						 * Handle bonus game.
						 */
						if (core::bonusWin > 0) {
							core::baseGameMode = false;
							core::bonusGameMode = true;
						}

						/*
						 * Free games mode after base game with conditions for free games.
						 */
						if (core::freeGamesNumber > 0) {
							core::baseGameMode = false;
							core::freeGamesMode = true;
						}
					}

					break;
				}
			}
		}

		if (core::baseGameMode == true) {
			core::activeScreen = core::MAIN_GAME_SCREEN;
		} else if (core::freeGamesMode == true
				&& core::bonusGameMode == false) {
			if (SDL_GetTicks() - core::lastSpinTick > 1000) {
				core::activeScreen = core::FREE_GAME_SCREEN;
				core::spinAllReels();
				runFreeGame(core::totalWin);
				core::lastSpinTick = SDL_GetTicks();

				if (core::freeGamesNumber <= 0) {
					core::baseGameMode = true;
					core::freeGamesMode = false;
				}
			}
		} else if (core::bonusGameMode == true) {
			/*
			 * Timeout for bonus screen switch on.
			 */
			if (core::bonusSelected == false
					&& SDL_GetTicks() - core::lastSpinTick > 1000) {
				core::activeScreen = core::BONUS_GAME_SCREEN;
			}

			/*
			 * Timeout for bonus mode switch off.
			 */
			if (core::bonusSelected == true
					&& SDL_GetTicks() - core::lastSpinTick > 3000) {
				core::bonusWin = 0;
				core::bonusGameMode = false;
				core::bonusSelected = false;
				if (core::freeGamesMode == false) {
					core::baseGameMode = true;
					core::activeScreen = core::MAIN_GAME_SCREEN;
				} else if (core::freeGamesMode == true) {
					core::activeScreen = core::FREE_GAME_SCREEN;
				}
			}
		}

		/*
		 * Drawing block.
		 */
		switch (core::activeScreen) {
		case core::SPLASH_SCREEN:
			break;

		case core::MAIN_GAME_SCREEN:
			drawMainGame();
			break;

		case core::FREE_GAME_SCREEN:
			drawFreeGame();
			break;

		case core::BONUS_GAME_SCREEN:
			drawBonusGame();
			break;

		case core::PAYTABLE_SCREEN:
			break;

		case core::HELP_SCREEN:
			break;
		}
	}

	deinitMainGame();
}

}
