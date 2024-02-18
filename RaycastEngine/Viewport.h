#pragma once

#include <SDL.h>
#include <stdio.h>
#include "World.h"

#define VIEW_MAX 100

void DrawFirstPersonView(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);

//inline char CheckWallX(struct sLevel_Data* lvl, struct Float2D coords)
//{
//	return lvl->TileData[(int)coords.Y * lvl->Size.X + (int)ceil(coords.X)].Type || lvl->TileData[(int)coords.Y * lvl->Size.X + (int)ceil(coords.X) - 1].Type;
//}
//inline char CheckWallY(struct sLevel_Data* lvl, struct Float2D coords)
//{
//	//printf("Coords for y: %f, %f\n", coords.X, coords.Y);
//	return ;
//}

inline struct Float2D CastRay(double angle, struct Float2D source, struct sLevel_Data* lvl)
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
		return hit_x;
	else
		return hit_y;
}

//inline struct Float2D CalcPlayerViewLineX(struct sEntity_Data* entity, struct sLevel_Data* lvl)
//{
//	struct Float2D ray_hit;
//	double cosine = cos(entity->Look);
//	if(cosine > 0)
//	{
//		int count = 0;
//		while(count < VIEW_MAX)
//		{
//			ray_hit.X = ceil(entity->Location.X + count);
//			ray_hit.Y = sin(entity->Look) * ( ray_hit.X - entity->Location.X ) / cosine + entity->Location.Y;
//			if(ray_hit.Y < lvl->Size.Y && ray_hit.Y >= 0 && CheckWallX(lvl, ray_hit))
//			{
//				return ray_hit;
//			}
//			else if(ray_hit.Y >= lvl->Size.Y || ray_hit.Y < 0)
//			{
//				ray_hit.X = -1;
//				ray_hit.Y = -1;
//				return ray_hit;
//			}
//			++count;
//		}
//	}
//	else if(cosine < 0)
//	{
//		int count = 0;
//		while(count < VIEW_MAX)
//		{
//			ray_hit.X = floor(entity->Location.X - count);
//			ray_hit.Y = sin(entity->Look) * ( ray_hit.X - entity->Location.X ) / cosine + entity->Location.Y;
//			if(ray_hit.Y < lvl->Size.Y && ray_hit.Y >= 0 && CheckWallX(lvl, ray_hit))
//			{
//				//printf("Hit at (%f, %f)\n", ray_hit.X, ray_hit.Y);
//				return ray_hit;
//			}
//			else if(ray_hit.Y >= lvl->Size.Y || ray_hit.Y < 0)
//			{
//				ray_hit.X = 0;
//				ray_hit.Y = 0;
//				return ray_hit;
//			}
//
//			++count;
//		}
//	}
//	else
//	{
//		ray_hit.X = -1;
//		ray_hit.Y = -1;
//	}
//
//	return ray_hit;
//}
//inline struct Float2D CalcPlayerViewLineY(struct sEntity_Data* entity, struct sLevel_Data* lvl)
//{
//	struct Float2D ray_hit;
//	double sine = sin(entity->Look);
//	if(sine > 0)
//	{
//		int count = 0;
//		while(count < VIEW_MAX)
//		{
//			ray_hit.Y = ceil(entity->Location.Y + count);
//			ray_hit.X = cos(entity->Look) * ( ray_hit.Y - entity->Location.Y ) / sine + entity->Location.X;
//			if(ray_hit.X < lvl->Size.X && ray_hit.X >= 0 && CheckWallY(lvl, ray_hit))
//			{
//				printf("Hit at (%f, %f)\n", ray_hit.X, ray_hit.Y);
//				return ray_hit;
//			}
//			else if(ray_hit.X >= lvl->Size.X || ray_hit.X < 0)
//			{
//				ray_hit.X = -1;
//				ray_hit.Y = -1;
//				return ray_hit;
//			}
//			++count;
//		}
//	}
//	else if(sine < 0)
//	{
//		int count = 0;
//		while(count < VIEW_MAX)
//		{
//			ray_hit.Y = floor(entity->Location.Y - count);
//			ray_hit.X = cos(entity->Look) * ( ray_hit.Y - entity->Location.Y ) / sine + entity->Location.X;
//			if(ray_hit.X < lvl->Size.X && ray_hit.X >= 0 && CheckWallY(lvl, ray_hit))
//			{
//				printf("Hit at (%f, %f)\n", ray_hit.X, ray_hit.Y);
//				return ray_hit;
//			}
//			else if(ray_hit.X >= lvl->Size.X || ray_hit.X < 0)
//			{
//				ray_hit.X = -1;
//				ray_hit.Y = -1;
//				return ray_hit;
//			}
//			++count;
//		}
//	}
//	else
//	{
//		ray_hit.X = -1;
//		ray_hit.Y = -1;
//	}
//
//	return ray_hit;
//}