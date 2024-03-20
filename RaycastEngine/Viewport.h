#pragma once

#include <SDL.h>
#include <stdio.h>
#include "World.h"

//#define VIEW_HEIGHT 256
#define VIEW_HORIZON 0.45
#define VIEW_MAX 100
#define THREE_QUARTER_CIRCLE 3*M_PI/2
#define QUARTER_CIRCLE M_PI/2

void DrawFirstPersonView(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);

inline float dist(float ax, float ay, float bx, float by)
{
	return ;
}

inline double CastRay(double angle, struct Float2D source, struct sLevel_Data* lvl, struct Float2D* ray_hit, char* x_or_y)
{
	struct Float2D hit_x = {0}, hit_y = {0}, offset = {0};
	double distX=1024, distY=1024, tangent = tan(angle), cotangent = 1/tangent;
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

	if(distX < distY)
	{
		if(x_or_y != NULL) *x_or_y = 1;
		if(ray_hit != NULL) { ray_hit->X = hit_x.X; ray_hit->Y = hit_x.Y; }
		return distX;
	}
	else
	{
		if(x_or_y != NULL) *x_or_y = 0;
		if(ray_hit != NULL) { ray_hit->X = hit_y.X; ray_hit->Y = hit_y.Y; }
		return distY;
	}
}