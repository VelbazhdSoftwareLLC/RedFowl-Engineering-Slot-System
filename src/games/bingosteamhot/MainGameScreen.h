#ifndef GAME_BINGOSTEAMHOT_MAINGAMESCREEN_H_
#define GAME_BINGOSTEAMHOT_MAINGAMESCREEN_H_

#include "SlotDefinition.h"

namespace bingosteamhot {

extern int offsets[bingosteamhot::REELS_LENGTH];

extern void initMainGame();
extern void deinitMainGame();
extern void drawMainGame();

}

#endif
