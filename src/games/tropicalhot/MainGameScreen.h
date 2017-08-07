#ifndef GAME_TROPICALHOT_MAINGAMESCREEN_H_
#define GAME_TROPICALHOT_MAINGAMESCREEN_H_

#include "SlotDefinition.h"

namespace tropicalhot {

extern int offsets[tropicalhot::REELS_LENGTH];

extern void initMainGame();
extern void deinitMainGame();
extern void drawBonusGame();
extern void drawFreeGame();
extern void drawMainGame();

}

#endif
