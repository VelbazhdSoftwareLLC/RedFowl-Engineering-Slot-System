#include "CommonState.h"

namespace core {

/**
 * RTP used for the reels.
 */
int rtp = 90;

/**
 * Game title as it is used into databese.
 */
string title;

/**
 * Find better place for session id.
 */
unsigned long sessionId = 0;

/**
 * Auto play mode flag.
 */
bool autoPlayMode = false;

/**
 * Base game mode flag.
 */
bool baseGameMode = false;

/**
 * Base game mode flag.
 */
bool freeGamesMode = false;

/**
 * Base game mode flag.
 */
bool bonusGameMode = false;

/**
 * If the player did bonus selection.
 */
bool bonusSelected = false;

/**
 * Denomination of coins to real money.
 */
double denomination = 0.0;

/**
 * Player credit.
 */
int credit = 0;

/**
 * Total bet is calculated by number of lines and single bet.
 */
int totalBet = 0;

/**
 * Total win in single game.
 */
int totalWin = 0;

/**
 * Bonus win in single game.
 */
int bonusWin = 0;

/**
 * Keep track of bingo line bonus.
 */
int bingoLineBonusWin = 0;

/**
 * Keep track of bingo bonus.
 */
int bingoBonusWin = 0;

/**
 * Keep track of last bingo ball out.
 */
int bingoBallNumber;

/**
 * Number of lines in which player bet.
 */
int numberOfBettingLines = 0;

/**
 * Amоunt of the bet on a single line.
 */
int singleLineBet = 0;

/**
 * Amount of free games to be played.
 */
int freeGamesNumber = 0;

/**
 * Multiply free games win.
 */
int freeGamesMultiplier = 1;

/**
 * Multiply total bet in scatter win.
 */
int scatterMultiplier = 1;

/**
 * Reference to active base game reels.
 */
vector<vector<int> > baseReels;

/**
 * Reference to active base game reels.
 */
vector<vector<int> > freeReels;

/**
 * Slot view.
 */
vector<vector<int> > view;

/**
 * Each reel minimal offset after spin action.
 */
vector<int> reelsMinOffset;

/**
 * Each reel maximum offset after spin action.
 */
vector<int> reelsMaxOffset;

/**
 * Reels stops. Indices of the first row after spin.
 */
vector<int> reelsStops;

}
