#ifndef GAMES_CORE_COMMONSTATE_H_
#define GAMES_CORE_COMMONSTATE_H_

#include <vector>
#include <string>

namespace core {

using namespace std;

class CommonState {

public:

	/**
	 * Game RTP.
	 */
	static int rtp;

	/**
	 * Game title.
	 */
	static string title;

	/**
	 * Session identifier.
	 */
	static unsigned long sessionId;

	/**
	 * Auto play flag.
	 */
	static bool autoPlayMode;

	/**
	 * Base game mode flag.
	 */
	static bool baseGameMode;

	/**
	 * Free spins mode flag.
	 */
	static bool freeGamesMode;

	/**
	 * Bonus game mode flag.
	 */
	static bool bonusGameMode;

	/**
	 * If the player did bonus selection.
	 */
	static bool bonusSelected;

	/**
	 * Denomination of coins to real money.
	 */
	static double denomination;

	/**
	 * Player credit.
	 */
	static int credit;

	/**
	 * Total bet is calculated by number of lines and single bet.
	 */
	static int totalBet;

	/**
	 * Total win in single game.
	 */
	static int totalWin;

	/**
	 * Bonus win in single game.
	 */
	static int bonusWin;

	/**
	 * Keep track of bingo line bonus.
	 */
	static int bingoLineBonusWin;

	/**
	 * Keep track of bingo bonus.
	 */
	static int bingoBonusWin;

	/**
	 * Keep track of last bingo ball out.
	 */
	static int bingoBallNumber;

	/**
	 * Number of lines in which player bet.
	 */
	static int numberOfBettingLines;

	/**
	 * Amоunt of the bet on a single line.
	 */
	static int singleLineBet;

	/**
	 * Amount of free games to be played.
	 */
	static int freeGamesNumber;

	/**
	 * Multiply free games win.
	 */
	static int freeGamesMultiplier;

	/**
	 * Multiply total bet in scatter win.
	 */
	static int scatterMultiplier;

	/**
	 * Reference to active base game reels.
	 */
	static vector<vector<int> > reels;

	/**
	 * Reference to active base game reels.
	 */
	static vector<vector<int> > baseReels;

	/**
	 * Reference to active free games reels.
	 */
	static vector<vector<int> > freeReels;

	/**
	 * Slot view.
	 */
	static vector<vector<int> > view;

	/**
	 * Each reel minimal offset after spin action.
	 */
	static vector<int> reelsMinOffset;

	/**
	 * Each reel maximum offset after spin action.
	 */
	static vector<int> reelsMaxOffset;

	/**
	 * Reels stops. Indices of the first row after spin.
	 */
	static vector<int> reelsStops;

};

}

#endif
