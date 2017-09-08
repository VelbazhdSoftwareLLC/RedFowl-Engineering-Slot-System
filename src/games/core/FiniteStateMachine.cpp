#include "FiniteStateMachine.h"

namespace core {

Uint32 FiniteStateMachine::lastSpinTick = SDL_GetTicks();

vector<bool> FiniteStateMachine::reelsSpinFlag;

}
