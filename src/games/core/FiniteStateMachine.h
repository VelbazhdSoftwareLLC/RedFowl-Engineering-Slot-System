#ifndef GAME_CORE_SLOTFINITESTATEMACHINE_H_
#define GAME_CORE_SLOTFINITESTATEMACHINE_H_

#include <vector>

#include <SDL2/SDL.h>

namespace core {

extern Uint32 lastSpinTick;
extern std::vector<bool> reelsSpinFlag;

extern void reset();
extern void spinAllReels();
extern bool areAllReelsStoped();

}

#endif
