#include <SDL.h>
#include "Application.h"
#include "Viewport.h"

void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sWorld_Data** wrld_ptr, struct sView* view)
{

	struct sWorld_Data* wrld = *wrld_ptr;
	if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	{
		double above_horizon = (1 - VIEW_HORIZON) * view->ViewSize.Y;
		double below_horizon = VIEW_HORIZON * view->ViewSize.Y;

		//Draw sky.
		SDL_Rect rect;
		rect.x = view->ViewPos.X;
		rect.y = view->ViewPos.Y;
		rect.w = view->ViewSize.X;
		rect.h = view->ViewSize.Y;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);

		//Draw stand-in floor.
		rect.y = above_horizon;
		rect.h = below_horizon;

		SDL_SetRenderDrawColor(renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);

		//Do raycast.
		struct Float2D end = {0};

		double fan = wrld->Player->Look - PLAYER_VIEW_RESOLUTION * 30;
		for(int r = 0; r < 60; ++r)
		{
			char x_or_y = 1;
			double dist = CastRay(fan, wrld->Player->Location, &(wrld->LevelData[wrld->Player->Lvl]), &end, &x_or_y);
			if(x_or_y)
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			else
				SDL_SetRenderDrawColor(renderer, 127, 0, 0, SDL_ALPHA_OPAQUE);

			double draw_height = dist / VIEW_MAX;
			double x_view_pos = (fan - (wrld->Player->Look - PLAYER_VIEW)) / (PLAYER_VIEW * 2) * view->ViewSize.X + view->ViewPos.X;
			SDL_RenderDrawLine(renderer, x_view_pos, view->ViewPos.Y + above_horizon * draw_height / 2, x_view_pos, rect.y + below_horizon * draw_height / 2); 
			fan += PLAYER_VIEW_RESOLUTION;
		}
	}
}