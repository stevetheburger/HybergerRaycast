#include <SDL.h>
#include "Viewport.h"

void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sWorld_Data** wrld_ptr, struct Int2D view_pos, struct Int2D view_size)
{
	struct sWorld_Data* wrld = *wrld_ptr;
	if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	{
		SDL_Rect rect;
		rect.x = view_pos.X;
		rect.y = view_pos.Y;
		rect.w = view_size.X;
		rect.h = view_size.Y;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);
	}
}