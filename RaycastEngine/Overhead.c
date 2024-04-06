#include "Overhead.h"

#include <SDL.h>
#include <stdlib.h>

#include "Application.h"
#include "World.h"
#include "Viewport.h"
#include "Entity.h"
#include "Camera.h"

void DrawOverheadView(struct SDL_Renderer* renderer, struct sEntity_Data* plyr, struct sCamera* cam, struct sLevel_Data* lvl, struct sView* view)
{
	//Draw background
	SDL_SetRenderDrawColor(renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	DrawWorldOverhead(renderer, lvl, view);

	DrawEntityOverhead(renderer, plyr, view);
	//DrawPlayerViewLine(renderer, wrld->Player, wrld_ptr, view);
	DrawPlayerViewFan(renderer, cam, view);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, view->ViewSize.X - 1, 0, view->ViewSize.X - 1, view->ViewSize.Y - 1);
}

void DrawEntityOverhead(struct SDL_Renderer* renderer, struct sEntity_Data* entity, struct sView* view)
{
	if(entity != NULL)
	{
		//struct Int2D position;
		if(entity->Controller.IsPlayer)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		struct Float2D position = fSub(WorldToViewCoord(entity->Location, view), (struct Float2D){PLAYER_WIDTH / 2, PLAYER_WIDTH / 2});
		SDL_RenderFillRect(renderer, &(struct SDL_Rect){(int)position.X, (int)position.Y , PLAYER_WIDTH, PLAYER_WIDTH});
	}
}

void DrawPlayerViewLine(struct SDL_Renderer* renderer, struct sCamera* cam, struct sView* view)
{
	struct Float2D end, start = {0};
	end = WorldToViewCoord(cam->Hits[cam->NumRays/2], view);
	start = WorldToViewCoord(cam->Location, view);
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, (int)start.X, (int)start.Y, (int)end.X, (int)end.Y);
}

void DrawPlayerViewFan(struct SDL_Renderer* renderer, struct sCamera* cam, struct sView* view)
{
	struct Float2D end, start = {0};
	start = WorldToViewCoord(cam->Location, view);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	//double fan_start = entity->Look - FIELD_OF_VIEW/2, fan_running = 0;
	//while(fan_running < FIELD_OF_VIEW)
	//{
	//	CastRay(fan_start + fan_running, entity->Location, &(*data)->LevelData[entity->Lvl], &end, NULL);
	//	end = WorldToViewCoord(end, view);
	//	SDL_RenderDrawLine(renderer, (int)start.X, (int)start.Y, (int)end.X, (int)end.Y);
	//	fan_running += VIEW_CAST_INCREMENT;
	//}

	unsigned int count = 0;
	while(count < cam->NumRays)
	{
		end = WorldToViewCoord(cam->Hits[count], view);
		SDL_RenderDrawLine(renderer, (int)start.X, (int)start.Y, (int)end.X, (int)end.Y);

		++count;
	}
}

void DrawWorldOverhead(struct SDL_Renderer* renderer, struct sLevel_Data* lvl, struct sView* view)
{
	//struct sWorld_Data* wrld = *wrld_ptr;
	//if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	//{
		//Set up variables.
		SDL_Rect rect = {0, 0, TILE_WIDTH_OVERHEAD, TILE_HEIGHT_OVERHEAD};
		struct Float2D wrld_coord;
		struct Float2D view_coord;
		int count_x = 0, count_y = 0;

		//struct sLevel_Data* lvl = &wrld->LevelData[wrld->Player->Lvl];

		while(count_y < lvl->Size.Y)
		{
			while(count_x < lvl->Size.X)
			{
				wrld_coord.X = count_x;
				wrld_coord.Y = count_y;
				view_coord = WorldToViewCoord(wrld_coord, view);

				if(lvl->TileData[count_y * lvl->Size.X + count_x].Type == 0)
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				else if(lvl->TileData[count_y * lvl->Size.X + count_x].Type == 1)
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

				rect.x = (int)view_coord.X;
				rect.y = (int)view_coord.Y;
				SDL_RenderFillRect(renderer, &rect);

				++count_x;
			}
			count_x = 0;
			wrld_coord.X = 0.0;
			++count_y;
		}
	//}
}

struct Float2D WorldToViewCoord(struct Float2D in, struct sView* view)
{
	return (struct Float2D){TILE_WIDTH_OVERHEAD * ( in.X - view->CameraPositionInWrld.X ) + view->ViewSize.X / 2, TILE_HEIGHT_OVERHEAD * ( in.Y - view->CameraPositionInWrld.Y ) + view->ViewSize.Y / 2};
}
struct Float2D ViewToWorldCoord(struct Float2D in, struct sView* view)
{
	return (struct Float2D){( in.X - view->ViewSize.X / 2 ) / TILE_WIDTH_OVERHEAD + view->CameraPositionInWrld.X, (in.Y - view->ViewSize.Y / 2 ) / TILE_HEIGHT_OVERHEAD + view->CameraPositionInWrld.Y};
}