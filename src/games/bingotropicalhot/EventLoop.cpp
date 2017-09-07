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
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::baseReels[i].resize(REEL_LENGTH);
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::CommonState::baseReels[i][j] = baseGameReels[core::CommonState::rtp - 90][i][j];
		}
	}

	//TODO Find better place to initialize decorative initial view.
	core::Calculations::generatDecorative(core::CommonState::baseReels);

	//TODO Find better place to initialize bingo cards.
	core::BingoBonus::generateRandomBingoCard();
	core::CommonState::bingoLineBonusWin = 0;
	core::CommonState::bingoBonusWin = 0;

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

					/*
					 * Credit changed by user.
					 */
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

					if (core::BingoBonus::bingoLineIndex != -1
							&& core::BingoBonus::bingoCardIndex != -1) {
						core::BingoBonus::generateRandomBingoCard();
						core::CommonState::bingoLineBonusWin = 0;
						core::CommonState::bingoBonusWin = 0;
					}

					core::FiniteStateMachine::spinAllReels();
					runBaseGame(core::CommonState::totalWin);
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
