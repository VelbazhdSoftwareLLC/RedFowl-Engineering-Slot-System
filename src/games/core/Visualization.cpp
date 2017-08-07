#include "ActiveScreen.h"

#include "Visualization.h"

namespace core {

SDL_Window *window = NULL;

SDL_Surface *canvas = NULL;

core::ActiveScreen activeScreen = core::MAIN_GAME_SCREEN;

}
