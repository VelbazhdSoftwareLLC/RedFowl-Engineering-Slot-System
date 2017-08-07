#ifndef GAME_SUNSETHOT_MAINGAMESCREEN_H_
#define GAME_SUNSETHOT_MAINGAMESCREEN_H_

#include "SlotDefinition.h"

namespace sunsethot {

extern int offsets[sunsethot::REELS_LENGTH];

extern void initMainGame();
extern void deinitMainGame();
extern void drawBonusGame();
extern void drawFreeGame();
extern void drawMainGame();

}

#endif
