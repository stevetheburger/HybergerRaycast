#include "Overhead.h"

#include <SDL.h>
#include <stdlib.h>

#include "Application.h"
#include "World.h"
#include "Viewport.h"
#include "Entity.h"
#include "Camera.h"

//Top level overhead view render function.
void DrawOverheadView(struct SDL_Renderer* renderer, struct sEntity_Data* plyr, struct sCamera* cam, struct sLevel_Data* lvl, struct sView* view)
{
	//Draw background
	SDL_SetRenderDrawColor(renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	//Draw the world.
	DrawWorldOverhead(renderer, lvl, view);

	//Draw the player entity.
	DrawEntityOverhead(renderer, plyr, view);
	DrawPlayerViewFan(renderer, cam, view);

	//Draw boarder.
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, view->ViewSize.X - 1, 0, view->ViewSize.X - 1, view->ViewSize.Y - 1);
}

//Draw an entity on the overhead view.
void DrawEntityOverhead(struct SDL_Renderer* renderer, struct sEntity_Data* entity, struct sView* view)
{
	if(entity != NULL)
	{
		//Set player to a different color.
		if(entity->Controller.IsPlayer)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		
		//Calculate position.
		struct Float2D position = fSub(WorldToViewCoord(entity->Location, view), (struct Float2D){PLAYER_WIDTH / 2, PLAYER_WIDTH / 2});
		//Draw.
		SDL_RenderFillRect(renderer, &(struct SDL_Rect){(int)position.X, (int)position.Y , PLAYER_WIDTH, PLAYER_WIDTH});
	}
}
//Draw the rays of the vision on the overhead view.
void DrawPlayerViewFan(struct SDL_Renderer* renderer, struct sCamera* cam, struct sView* view)
{
	//Setup variables.
	struct Float2D end, start = {0};
	//Calculate starting coordinate.
	start = WorldToViewCoord(cam->Location, view);

	//Loop through the rays, and draw them by grabbing the end points and factoring for the current position of the camera.
	unsigned int count = 0;
	while(count < cam->NumRays)
	{
		//Choose color from type for testing purposes.
		switch(cam->HitType[count])
		{
			case 1:
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				break;
			case 2:
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				break;
			case 3:
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
				break;
			case 4:
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 64, 64, 64, SDL_ALPHA_OPAQUE);
		}

		end = WorldToViewCoord(cam->Hits[count], view);
		SDL_RenderDrawLine(renderer, (int)start.X, (int)start.Y, (int)end.X, (int)end.Y);

		++count;
	}
}

//Draw the level grid.
void DrawWorldOverhead(struct SDL_Renderer* renderer, struct sLevel_Data* lvl, struct sView* view)
{
	//Set up variables.
	SDL_Rect rect = {0, 0, TILE_WIDTH_OVERHEAD, TILE_HEIGHT_OVERHEAD};
	struct Float2D wrld_coord;
	struct Float2D view_coord;
	int count_x = 0, count_y = 0;

	//Loop through level grid
	while(count_y < lvl->Size.Y)
	{
		while(count_x < lvl->Size.X)
		{
			wrld_coord.X = count_x;
			wrld_coord.Y = count_y;
			view_coord = WorldToViewCoord(wrld_coord, view);

			//Choose color from type for testing purposes.
			switch(lvl->TileData[count_y * lvl->Size.X + count_x].Type)
			{
				case 1:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
					break;
				case 4:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 64, 64, 64, SDL_ALPHA_OPAQUE);
			}

			//Draw.
			rect.x = (int)view_coord.X;
			rect.y = (int)view_coord.Y;
			SDL_RenderFillRect(renderer, &rect);

			++count_x;
		}
		count_x = 0;
		wrld_coord.X = 0.0;
		++count_y;
	}
}

//Conversion method to move from screen space to world space and vica versa.
struct Float2D WorldToViewCoord(struct Float2D in, struct sView* view)
{
	return (struct Float2D){TILE_WIDTH_OVERHEAD * ( in.X - view->CameraPositionInWrld.X ) + view->ViewSize.X / 2, TILE_HEIGHT_OVERHEAD * ( in.Y - view->CameraPositionInWrld.Y ) + view->ViewSize.Y / 2};
}
struct Float2D ViewToWorldCoord(struct Float2D in, struct sView* view)
{
	return (struct Float2D){( in.X - view->ViewSize.X / 2 ) / TILE_WIDTH_OVERHEAD + view->CameraPositionInWrld.X, (in.Y - view->ViewSize.Y / 2 ) / TILE_HEIGHT_OVERHEAD + view->CameraPositionInWrld.Y};
}