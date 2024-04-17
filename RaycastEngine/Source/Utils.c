#include <SDL.h>
#include <stdio.h>

#include "Utils.h"

void prnterr() {char msg[100]; SDL_GetErrorMsg(msg, 100); printf("SDL Error: %s\n", msg);}