#include <iostream>

#include <SDL2/SDL.h>

#include "../core/BingoBonus.h"
#include "../core/CommonState.h"
#include "../core/Calculations.h"
#include "../core/Visualization.h"
#include "../core/FiniteStateMachine.h"

#include "SlotState.h"
#include "SlotDefinition.h"
#include "MainGameScreen.h"
#include "SlotCalculations.h"

#include "EventLoop.h"

namespace bingotropicalhot {

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
	core::FiniteStateMachine::reelsSpinFlag.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::reelsStops[i] = 0;
		core::FiniteStateMachine::reelsSpinFlag[i] = false;
	}

	/*
	 * RTP should be loaded from database.
	 */
	core::baseReels.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::baseReels[i].resize(REEL_LENGTH);
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::baseReels[i][j] = baseGameReels[core::rtp - 90][i][j];
		}
	}

	//TODO Find better place to initialize decorative initial view.
	core::Calculations::generatDecorative(core::baseReels);

	//TODO Find better place to initialize bingo cards.
	core::BingoBonus::generateRandomBingoCard();
	core::bingoLineBonusWin = 0;
	core::bingoBonusWin = 0;

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

					/*
					 * Credit changed by user.
					 */
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

					/*
					 * Switch off auto play if spin button is used.
					 */
					if (core::autoPlayMode == true) {
						core::autoPlayMode = false;
					}

					if (core::totalBet <= 0) {
						//TODO Report no credit.
						//cerr << "Total bet is empty ..." << endl;
						break;
					}

					if (core::credit < core::totalBet) {
						//TODO Report no credit.
						//cerr << "No enough credit ..." << endl;
						break;
					}

					if (core::BingoBonus::bingoLineIndex != -1
							&& core::BingoBonus::bingoCardIndex != -1) {
						core::BingoBonus::generateRandomBingoCard();
						core::bingoLineBonusWin = 0;
						core::bingoBonusWin = 0;
					}

					core::FiniteStateMachine::spinAllReels();
					runBaseGame(core::totalWin);
					core::FiniteStateMachine::lastSpinTick = SDL_GetTicks();
					memset(offsets, 0, REELS_LENGTH * sizeof(int));

					break;
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

		case core::PAYTABLE_SCREEN:
			break;

		case core::HELP_SCREEN:
			break;
		}
	}

	deinitMainGame();
}

}
