#include <SDL.h>
#include "Application.h"
#include "Viewport.h"

void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sWorld_Data** wrld_ptr, struct sView* view)
{
	struct sWorld_Data* wrld = *wrld_ptr;
	if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	{
		SDL_Rect rect;
		rect.x = view->ViewPos.X;
		rect.y = view->ViewPos.Y;
		rect.w = view->ViewSize.X;
		rect.h = view->ViewSize.Y;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);
	}
}