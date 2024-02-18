#pragma once

#include <SDL.h>
#include <stdio.h>
#include "World.h"

//#define VIEW_HEIGHT 256
#define VIEW_HORIZON 0.45
#define VIEW_MAX 100

void DrawFirstPersonView(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);

inline struct Float2D CastRay(double angle, struct Float2D source, struct sLevel_Data* lvl, char * x_or_y)
{
	struct Float2D hit_x = {0}, hit_y = {0};
	double cosine = cos(angle);
	double sine = sin(angle);
	int count = 0;
	if(cosine != 0)
	{
		double sineovercosine = sine/cosine;
		if(cosine > 0)
		{
			while(count < VIEW_MAX)
			{
				hit_x.X = ceil(source.X + count);
				hit_x.Y = sineovercosine * (hit_x.X - source.X) + source.Y;
				if
				(
					hit_x.Y < lvl->Size.Y && 
					hit_x.Y >= 0 && 
					(lvl->TileData[(int)hit_x.Y * lvl->Size.X + (int)ceil(hit_x.X)].Type || lvl->TileData[(int)hit_x.Y * lvl->Size.X + (int)ceil(hit_x.X) - 1].Type)
				)
					break;
				++count;
			}
		}
		else if(cosine < 0)
		{
			while(count < VIEW_MAX)
			{
				hit_x.X = floor(source.X - count);
				hit_x.Y = sineovercosine * (hit_x.X - source.X) + source.Y;
				if
				(
					hit_x.Y < lvl->Size.Y && 
					hit_x.Y >= 0 && 
					(lvl->TileData[(int)hit_x.Y * lvl->Size.X + (int)ceil(hit_x.X)].Type || lvl->TileData[(int)hit_x.Y * lvl->Size.X + (int)ceil(hit_x.X) - 1].Type)
				)
					break;
				++count;
			}
		}
	}
	if(sine != 0)
	{
		count = 0;
		double cosineoversine = cosine/sine;
		if(sine > 0)
		{
			while(count < VIEW_MAX)
			{
				hit_y.Y = ceil(source.Y + count);
				hit_y.X = cosineoversine * (hit_y.Y - source.Y) + source.X;
				if
				(	
					hit_y.X < lvl->Size.X && 
					hit_y.X >= 0 && 
					(lvl->TileData[(int)ceil(hit_y.Y) * lvl->Size.X + (int)hit_y.X].Type || lvl->TileData[((int)ceil(hit_y.Y) - 1) * lvl->Size.X + (int)hit_y.X].Type)
				)
					break;
				++count;
			}
		}
		else if(sine < 0)
		{
			while(count < VIEW_MAX)
			{
				hit_y.Y = floor(source.Y - count);
				hit_y.X = cosineoversine * (hit_y.Y - source.Y) + source.X;
				if
				(
					hit_y.X < lvl->Size.X && 
					hit_y.X >= 0 && 
					(lvl->TileData[(int)ceil(hit_y.Y) * lvl->Size.X + (int)hit_y.X].Type || lvl->TileData[((int)ceil(hit_y.Y) - 1) * lvl->Size.X + (int)hit_y.X].Type)
				)
					break;
				++count;
			}
		}
	}
	if(sqrt(pow(hit_x.X - source.X, 2) + pow(hit_x.Y - source.Y, 2)) < sqrt(pow(hit_y.X - source.X, 2) + pow(hit_y.Y - source.Y, 2)))
	{
		if(x_or_y != NULL) *x_or_y = 1;	
		return hit_x;
	}
	else
	{
		if(x_or_y != NULL) *x_or_y = 0;
		return hit_y;
	}
}