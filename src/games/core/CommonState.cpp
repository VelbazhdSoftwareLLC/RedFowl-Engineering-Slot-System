#include "CommonState.h"

namespace core {

int CommonState::rtp = 90;

string CommonState::title;

//TODO Find better place for session id.
unsigned long CommonState::sessionId = 0;

bool CommonState::autoPlayMode = false;

bool CommonState::baseGameMode = false;

bool CommonState::freeGamesMode = false;

bool CommonState::bonusGameMode = false;

bool CommonState::bonusSelected = false;

double CommonState::denomination = 0.0;

int CommonState::credit = 0;

int CommonState::totalBet = 0;

int CommonState::totalWin = 0;

int CommonState::bonusWin = 0;

int CommonState::bingoLineBonusWin = 0;

int CommonState::bingoBonusWin = 0;

int CommonState::bingoBallNumber;

int CommonState::numberOfBettingLines = 0;

int CommonState::singleLineBet = 0;

int CommonState::freeGamesNumber = 0;

int CommonState::freeGamesMultiplier = 1;

int CommonState::scatterMultiplier = 1;

//TODO It should be a pointer and it will point base reels or free reels.
vector<vector<int> > CommonState::reels;

vector<vector<int> > CommonState::baseReels;

vector<vector<int> > CommonState::freeReels;

vector<vector<int> > CommonState::view;

vector<int> CommonState::reelsMinOffset;

vector<int> CommonState::reelsMaxOffset;

vector<int> CommonState::reelsStops;

}
