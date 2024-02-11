#include <SDL.h>
#include "Overhead.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

void DrawOverheadView(struct SDL_Renderer* renderer, struct sWorld_Data** wrld_ptr, struct Int2D view_pos, struct Int2D view_size)
{
	struct sWorld_Data* wrld = *wrld_ptr;
	if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	{
		//Set up variables.
		SDL_Rect rect;
		rect.w = TILE_WIDTH;
		rect.h = TILE_HEIGHT;
		rect.x = rect.y = 0;
		int count_x, count_y;
		count_x = count_y = 0;

		//Draw background
		SDL_SetRenderDrawColor(renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		struct sLevel_Data* lvl = &wrld->LevelData[wrld->Player->Lvl];

		while(count_y < lvl->Size.Y)
		{
			rect.y = TILE_HEIGHT * ( count_y - wrld->Player->Location.Y ) + view_size.Y / 2;
			while(count_x < lvl->Size.X)
			{
				rect.x = TILE_WIDTH * ( count_x - wrld->Player->Location.X ) + view_size.X / 2;
 				//rect.x = TILE_WIDTH * count_x - view_size.X / 2 + position.X;
				//rect.x = TILE_WIDTH * count_x;

				if(lvl->TileData[count_y * lvl->Size.X + count_x].Type == 0)
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				else if(lvl->TileData[count_y * lvl->Size.X + count_x].Type == 1)
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

				SDL_RenderFillRect(renderer, &rect);

				++count_x;
			}
			rect.x = 0;
			count_x = 0;
			++count_y;
		}

		struct Int2D position, endpoint;
		position.X = view_size.X / 2;
		position.Y = view_size.Y / 2;
		endpoint.X = position.X + cos(wrld->Player->Look) * 100;
		endpoint.Y = position.Y + sin(wrld->Player->Look) * 100;

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, position.X, position.Y, endpoint.X, endpoint.Y);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		rect.w = PLAYER_WIDTH;
		rect.h = PLAYER_HEIGHT;
		rect.x = position.X - PLAYER_WIDTH / 2;
		rect.y = position.Y - PLAYER_HEIGHT / 2;
		//rect.x = ( view_size.X - PLAYER_WIDTH ) / 2;
		//rect.y = ( view_size.Y - PLAYER_HEIGHT ) / 2;
		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, view_size.X, 0, view_size.X, view_size.Y);
	}
}