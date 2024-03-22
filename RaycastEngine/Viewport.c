#include <SDL.h>
#include "Application.h"
#include "Viewport.h"

void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sWorld_Data** wrld_ptr, struct sView* view)
{

	struct sWorld_Data* wrld = *wrld_ptr;
	if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	{
		//Draw sky.
		SDL_Rect rect;
		rect.x = view->ViewPos.X;
		rect.y = view->ViewPos.Y;
		rect.w = view->ViewSize.X;
		rect.h = view->ViewSize.Y;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);

		//Do raycast.
		double fan_start = wrld->Player->Look - FIELD_OF_VIEW_HALF, fan_running = 0;
		double columns_per_cast = view->ViewSize.X * COLUMNS_FACTOR;
		double x_draw_size = 0, remainder = 0;
		int x_draw_position = view->ViewPos.X, num_pixels = 0;

		while(fan_running < FIELD_OF_VIEW)
		{
			char x_or_y = 1;
			double draw_height = view->ViewSize.Y/CastRay(fan_start + fan_running, wrld->Player, &(wrld->LevelData[wrld->Player->Lvl]), NULL, &x_or_y); if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;
			if(x_or_y)
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			else
				SDL_SetRenderDrawColor(renderer, 127, 0, 0, SDL_ALPHA_OPAQUE);

			x_draw_size = columns_per_cast + remainder;
			num_pixels = (int)x_draw_size;
			remainder = x_draw_size - num_pixels;
			while(num_pixels>0)
			{
				SDL_RenderDrawLine(renderer, x_draw_position, (view->ViewSize.Y - draw_height) / 2, x_draw_position, (view->ViewSize.Y + draw_height) / 2);
				++x_draw_position;
				--num_pixels;
			}
			fan_running += VIEW_CAST_INCREMENT;
			fan_running = round(fan_running * 10000000)/10000000;
		}
	}
}