#include "Viewport.h"

#include <SDL.h>
#include <stdio.h>

#include "TrigConstants.h"
#include "Application.h"
#include "Camera.h"
#include "World.h"
#include "Entity.h"

void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sCamera* cam, struct sView* view)
{

	//struct sWorld_Data* wrld = *wrld_ptr;
	//if(wrld != NULL && wrld->Player != NULL && wrld->Player->Lvl < wrld->LevelCount)
	//{

	//Draw sky.
	SDL_Rect rect;
	rect.x = view->ViewPos.X;
	rect.y = view->ViewPos.Y;
	rect.w = view->ViewSize.X;
	rect.h = view->ViewSize.Y;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);

	////Do raycast.
	double draw_height = 0.0, x_draw_size = 0.0, remainder_left = 0.0, remainder_right = 0.0, correction_angle = 0.0;
	int columns_per_cast = view->ViewSize.X / cam->NumRays, x_draw_position_left = view->ViewPos.X + view->ViewSize.X / 2 - 1, x_draw_position_right = x_draw_position_left + 1, num_pixels = 0;

	unsigned int count = 0;
	while(count < cam->NumRays)
	{
		draw_height = view->ViewSize.Y/cam->Distances[count++]; if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;

		x_draw_size = columns_per_cast + remainder_left;
		num_pixels = (int)x_draw_size;
		remainder_left = x_draw_size - num_pixels;
		while(num_pixels>0)
		{
			SDL_RenderDrawLine(renderer, x_draw_position_left, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position_left, (int)(view->ViewSize.Y + draw_height) / 2);
 			--x_draw_position_left;
			--num_pixels;
		}

		draw_height = view->ViewSize.Y/(cam->Distances[count++]); if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;

		x_draw_size = columns_per_cast + remainder_right;
		num_pixels = (int)x_draw_size;
		remainder_right = x_draw_size - num_pixels;
		while(num_pixels>0)
		{
			SDL_RenderDrawLine(renderer, x_draw_position_right, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position_right, (int)(view->ViewSize.Y + draw_height) / 2);
			++x_draw_position_right;
			--num_pixels;
		}
	}
}

//double CastRay(double angle, struct sEntity_Data* viewer, struct sLevel_Data* lvl, struct Float2D* ray_hit, char* x_or_y)
//{
//	struct Float2D hit_x = {0}, hit_y = {0}, offset = {0};
//	struct Float2D source = viewer->Location;
//	double distX=1024, distY=1024, tangent = tan(angle), cotangent = 1/tangent, correction_angle = 0;
//	int count = 0, index;
//
//	if(angle>=FULL_CIRCLE) angle-=FULL_CIRCLE; if(angle<0) angle+=FULL_CIRCLE;
//
//	if(angle<QUARTER_CIRCLE || angle>THREE_QUARTER_CIRCLE) { hit_x.X = (int)source.X + 1; hit_x.Y = tangent * (hit_x.X - source.X) + source.Y; offset.X = 1; offset.Y = offset.X * tangent; }
//	else if(angle>QUARTER_CIRCLE && angle<THREE_QUARTER_CIRCLE) { hit_x.X = (int)source.X; hit_x.Y = tangent * (hit_x.X - source.X) + source.Y; offset.X = -1; offset.Y = offset.X * tangent; }
//
//	while(count < VIEW_MAX)
//	{
//		index = (int)hit_x.Y * lvl->Size.X + (int)hit_x.X;
//		if
//		(
//			hit_x.Y < lvl->Size.Y && 
//			hit_x.Y >= 0 && 
//			(lvl->TileData[index].Type || lvl->TileData[index - 1].Type)
//		)
//		{
//			distX = sqrt((source.X-hit_x.X)*(source.X-hit_x.X) + (source.Y-hit_x.Y)*(source.Y-hit_x.Y));
//			break;
//		}
//		else { hit_x.X += offset.X; hit_x.Y += offset.Y; ++count; }
//	}
//
//	count = 0;
//	if(angle<M_PI) { hit_y.Y = (int)source.Y + 1; hit_y.X = cotangent * (hit_y.Y - source.Y) + source.X; offset.Y = 1; offset.X = offset.Y * cotangent; }
//	else if(angle>M_PI) { hit_y.Y = (int)source.Y; hit_y.X = cotangent * (hit_y.Y - source.Y) + source.X; offset.Y = -1; offset.X = offset.Y * cotangent; }
//
//	while(count < VIEW_MAX)
//	{
//		index = (int)hit_y.Y * lvl->Size.X + (int)hit_y.X;
//		if
//		(	
//			hit_y.X < lvl->Size.X && 
//			hit_y.X >= 0 && 
//			(lvl->TileData[index].Type || lvl->TileData[index - lvl->Size.X].Type)
//		)
//		{
//			distY = sqrt((source.X-hit_y.X)*(source.X-hit_y.X) + (source.Y-hit_y.Y)*(source.Y-hit_y.Y));
//			break;		
//		}
//		else { hit_y.X += offset.X; hit_y.Y += offset.Y; ++count; }
//	}
//
//	//correction_angle = viewer->Look - angle; if(correction_angle<0) correction_angle+=FULL_CIRCLE; if(correction_angle>=FULL_CIRCLE) correction_angle-=FULL_CIRCLE;
//	if(distX < distY)
//	{
//		if(x_or_y != NULL) *x_or_y = 1;
//		if(ray_hit != NULL) { ray_hit->X = hit_x.X; ray_hit->Y = hit_x.Y; }
//		return distX/* * cos(correction_angle)*/;
//	}
//	else
//	{
//		if(x_or_y != NULL) *x_or_y = 0;
//		if(ray_hit != NULL) { ray_hit->X = hit_y.X; ray_hit->Y = hit_y.Y; }
//		return distY/* * cos(correction_angle)*/;
//	}
//}