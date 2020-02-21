#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <SDL2/SDL.h>
#include "ini.h"

struct SData {
	SDL_Window *win;
	SDL_Event ev;
};

#endif
