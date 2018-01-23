#include <SDL2/SDL.h>

#include "../core/CommonState.h"
#include "../core/Calculations.h"
#include "../core/Visualization.h"
#include "../core/FiniteStateMachine.h"

#include "MainGameScreen.h"
#include "SlotDefinition.h"
#include "SlotCalculations.h"

#include "EventLoop.h"

namespace tropicalhot {

using namespace std;

void loop() {
	/*
	 * Initialize bet parameters.
	 */
	core::CommonState::singleLineBet = MIN_SINGLE_LINE_BET;
	core::CommonState::numberOfBettingLines = MAX_SELECTED_LINES;
	core::CommonState::totalBet = core::CommonState::numberOfBettingLines * core::CommonState::singleLineBet;

	/*
	 * Initialize symbols view.
	 */
	core::CommonState::view.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::view[i].resize(ROWS_LENGTH);
		for (int j = 0; j < ROWS_LENGTH; j++) {
			core::CommonState::view[i][j] = EMPTY;
		}
	}

	/*
	 * Initialize reels stops offsets.
	 */
	core::CommonState::reelsMinOffset.resize(REELS_LENGTH);
	core::CommonState::reelsMaxOffset.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::reelsMinOffset[i] = reelsMinOffset[i];
		core::CommonState::reelsMaxOffset[i] = reelsMaxOffset[i];
	}

	/*
	 * Initialize reels stops.
	 */
	core::CommonState::reelsStops.resize(REELS_LENGTH);
	core::FiniteStateMachine::reelsSpinFlag.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::reelsStops[i] = 0;
		core::FiniteStateMachine::reelsSpinFlag[i] = false;
	}

	/*
	 * RTP should be loaded from database.
	 */
	core::CommonState::baseReels.resize(REELS_LENGTH);
	core::CommonState::freeReels.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::baseReels[i].resize(REEL_LENGTH);
		core::CommonState::freeReels[i].resize(REEL_LENGTH);
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::CommonState::baseReels[i][j] = baseGameReels[core::CommonState::rtp - 90][i][j];
			core::CommonState::freeReels[i][j] = freeGamesReels[core::CommonState::rtp - 90][i][j];
		}
	}

	//TODO Find better place to initialize decorative initial view.
	core::Calculations::generatDecorative(core::CommonState::baseReels);

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
					if (core::CommonState::bonusGameMode == true) {
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

						core::CommonState::bonusSelected = true;
						runBonusGame(core::CommonState::totalWin, index);
						core::FiniteStateMachine::lastSpinTick = SDL_GetTicks();
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
					core::CommonState::numberOfBettingLines++;

					if (core::CommonState::numberOfBettingLines > MAX_SELECTED_LINES) {
						core::CommonState::numberOfBettingLines = MIN_SELECTED_LINES;
					}

					core::CommonState::totalBet = core::CommonState::numberOfBettingLines
												  * core::CommonState::singleLineBet;
					break;

				/*
				 * Bet changed by user.
				 */
				case SDLK_b:
					core::CommonState::singleLineBet++;

					if (core::CommonState::singleLineBet > MAX_SINGLE_LINE_BET) {
						core::CommonState::singleLineBet = MIN_SINGLE_LINE_BET;
					}

					core::CommonState::totalBet = core::CommonState::numberOfBettingLines
												  * core::CommonState::singleLineBet;
					break;

				/*
				 * Max bet changed by user.
				 */
				case SDLK_m:
					core::CommonState::singleLineBet = MAX_SINGLE_LINE_BET;
					core::CommonState::numberOfBettingLines = MAX_SELECTED_LINES;

					core::CommonState::totalBet = core::CommonState::numberOfBettingLines
												  * core::CommonState::singleLineBet;
					break;

				case SDLK_c:
					if (core::CommonState::credit + CREDIT_INCREMENT <= MAX_CREDIT) {
						core::CommonState::credit += CREDIT_INCREMENT;
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

					if (core::CommonState::bonusGameMode == true) {
						break;
					}

					if (core::CommonState::freeGamesMode == true) {
						break;
					}

					/*
					 * Switch off auto play if spin button is used.
					 */
					if (core::CommonState::autoPlayMode == true) {
						core::CommonState::autoPlayMode = false;
					}

					if (core::CommonState::totalBet <= 0) {
						//TODO Report no credit.
						//cerr << "Total bet is empty ..." << endl;
						break;
					}

					if (core::CommonState::credit < core::CommonState::totalBet) {
						//TODO Report no credit.
						//cerr << "No enough credit ..." << endl;
						break;
					}

					if (core::CommonState::freeGamesNumber <= 0) {
						core::FiniteStateMachine::spinAllReels();
						runBaseGame(core::CommonState::totalWin);
						core::FiniteStateMachine::lastSpinTick = SDL_GetTicks();
						memset(offsets, 0, REELS_LENGTH * sizeof(int));

						/*
						 * Handle bonus game.
						 */
						if (core::CommonState::bonusWin > 0) {
							core::CommonState::baseGameMode = false;
							core::CommonState::bonusGameMode = true;
						}

						/*
						 * Free games mode after base game with conditions for free games.
						 */
						if (core::CommonState::freeGamesNumber > 0) {
							core::CommonState::baseGameMode = false;
							core::CommonState::freeGamesMode = true;
						}
					}

					break;
				}
			}
		}

		if (core::CommonState::baseGameMode == true) {
			core::activeScreen = core::MAIN_GAME_SCREEN;
		} else if (core::CommonState::freeGamesMode == true
				   && core::CommonState::bonusGameMode == false) {
			if (SDL_GetTicks() - core::FiniteStateMachine::lastSpinTick > 1000) {
				core::activeScreen = core::FREE_GAME_SCREEN;
				core::FiniteStateMachine::spinAllReels();
				runFreeGame(core::CommonState::totalWin);
				core::FiniteStateMachine::lastSpinTick = SDL_GetTicks();

				if (core::CommonState::freeGamesNumber <= 0) {
					core::CommonState::baseGameMode = true;
					core::CommonState::freeGamesMode = false;
				}
			}
		} else if (core::CommonState::bonusGameMode == true) {
			/*
			 * Timeout for bonus screen switch on.
			 */
			if (core::CommonState::bonusSelected == false
					&& SDL_GetTicks() - core::FiniteStateMachine::lastSpinTick > 1000) {
				core::activeScreen = core::BONUS_GAME_SCREEN;
			}

			/*
			 * Timeout for bonus mode switch off.
			 */
			if (core::CommonState::bonusSelected == true
					&& SDL_GetTicks() - core::FiniteStateMachine::lastSpinTick > 3000) {
				core::CommonState::bonusWin = 0;
				core::CommonState::bonusGameMode = false;
				core::CommonState::bonusSelected = false;
				if (core::CommonState::freeGamesMode == false) {
					core::CommonState::baseGameMode = true;
					core::activeScreen = core::MAIN_GAME_SCREEN;
				} else if (core::CommonState::freeGamesMode == true) {
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
