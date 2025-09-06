#ifndef GAME_CORE_VISUALISATION_H_
#define GAME_CORE_VISUALISATION_H_

#include <SDL2/SDL.h>

#include "ActiveScreen.h"

namespace core {

#define CANVAS_WIDTH 1440
#define CANVAS_HEIGHT 900

extern SDL_Window *window;
extern SDL_Surface *canvas;
extern core::ActiveScreen activeScreen;

}

#endif

