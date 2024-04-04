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

double CastRay(double angle, struct sEntity_Data* viewer, struct sLevel_Data* lvl, struct Float2D* ray_hit, char* x_or_y)
{
	struct Float2D hit_x = {0}, hit_y = {0}, offset = {0};
	struct Float2D source = viewer->Location;
	double distX=1024, distY=1024, tangent = tan(angle), cotangent = 1/tangent, correction_angle = 0;
	int count = 0, index;

	if(angle>=FULL_CIRCLE) angle-=FULL_CIRCLE; if(angle<0) angle+=FULL_CIRCLE;

	if(angle<QUARTER_CIRCLE || angle>THREE_QUARTER_CIRCLE) { hit_x.X = (int)source.X + 1; hit_x.Y = tangent * (hit_x.X - source.X) + source.Y; offset.X = 1; offset.Y = offset.X * tangent; }
	else if(angle>QUARTER_CIRCLE && angle<THREE_QUARTER_CIRCLE) { hit_x.X = (int)source.X; hit_x.Y = tangent * (hit_x.X - source.X) + source.Y; offset.X = -1; offset.Y = offset.X * tangent; }

	while(count < VIEW_MAX)
	{
		index = (int)hit_x.Y * lvl->Size.X + (int)hit_x.X;
		if
		(
			hit_x.Y < lvl->Size.Y && 
			hit_x.Y >= 0 && 
			(lvl->TileData[index].Type || lvl->TileData[index - 1].Type)
		)
		{
			distX = sqrt((source.X-hit_x.X)*(source.X-hit_x.X) + (source.Y-hit_x.Y)*(source.Y-hit_x.Y));
			break;
		}
		else { hit_x.X += offset.X; hit_x.Y += offset.Y; ++count; }
	}

	count = 0;
	if(angle<M_PI) { hit_y.Y = (int)source.Y + 1; hit_y.X = cotangent * (hit_y.Y - source.Y) + source.X; offset.Y = 1; offset.X = offset.Y * cotangent; }
	else if(angle>M_PI) { hit_y.Y = (int)source.Y; hit_y.X = cotangent * (hit_y.Y - source.Y) + source.X; offset.Y = -1; offset.X = offset.Y * cotangent; }

	while(count < VIEW_MAX)
	{
		index = (int)hit_y.Y * lvl->Size.X + (int)hit_y.X;
		if
		(	
			hit_y.X < lvl->Size.X && 
			hit_y.X >= 0 && 
			(lvl->TileData[index].Type || lvl->TileData[index - lvl->Size.X].Type)
		)
		{
			distY = sqrt((source.X-hit_y.X)*(source.X-hit_y.X) + (source.Y-hit_y.Y)*(source.Y-hit_y.Y));
			break;		
		}
		else { hit_y.X += offset.X; hit_y.Y += offset.Y; ++count; }
	}

	correction_angle = viewer->Look - angle; if(correction_angle<0) correction_angle+=FULL_CIRCLE; if(correction_angle>=FULL_CIRCLE) correction_angle-=FULL_CIRCLE;
	if(distX < distY)
	{
		if(x_or_y != NULL) *x_or_y = 1;
		if(ray_hit != NULL) { ray_hit->X = hit_x.X; ray_hit->Y = hit_x.Y; }
		return distX * cos(correction_angle);
	}
	else
	{
		if(x_or_y != NULL) *x_or_y = 0;
		if(ray_hit != NULL) { ray_hit->X = hit_y.X; ray_hit->Y = hit_y.Y; }
		return distY * cos(correction_angle);
	}
}