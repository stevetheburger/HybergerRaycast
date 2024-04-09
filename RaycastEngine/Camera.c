#include "Camera.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "TrigConstants.h"
#include "World.h"
#include "Float2D.h"

struct sCamera* CreateCamera(double fov, unsigned int num_rays, struct Float2D loc, double look)
{
	struct sCamera* ret_val = NULL;
	if(fov > 0)
	{
		ret_val = malloc(sizeof(struct sCamera));
		if(ret_val != NULL)
		{
			ret_val->Hits = calloc(num_rays, sizeof(struct Float2D));
			ret_val->Distances = calloc(num_rays, sizeof(double));
			ret_val->Angles = malloc(sizeof(double) * num_rays/2);
			if(ret_val->Hits != NULL && ret_val->Distances != NULL && ret_val->Angles != NULL)
			{
				ret_val->Fov = fov;
				ret_val->NumRays = num_rays;
				ret_val->Location.X = loc.X; ret_val->Location.Y = loc.Y;
				ret_val->Look = look;
				//double angle_incr = fov / num_rays;
				//double angle_start = -fov/2;
				//for(int i = 0; i < num_rays; ++i)
				//{
				//	ret_val->Angles[i] = angle_start;
				//	angle_start+=angle_incr;
				//	angle_start = round(angle_start * 10000000)/10000000;
				//}
				double x_incr = tan(fov/2) * 2 / num_rays;
				double x_start = x_incr / 2;
				for(unsigned int i = 0; i < num_rays / 2; ++i)
				{
					ret_val->Angles[i] = atan(x_start);
					x_start += x_incr;
					x_start = round(x_start * 10000000)/10000000;
				}
			}
			else
				DestroyCamera(&ret_val);
		}
	}
	return ret_val;
}

void DestroyCamera(struct sCamera** cam)
{
	if(cam != NULL)
	{
		if((*cam) != NULL)
		{
			if((*cam)->Angles != NULL) free((*cam)->Angles);
			if((*cam)->Distances != NULL) free((*cam)->Distances);
			if((*cam)->Hits != NULL) free((*cam)->Hits);
			free(*cam);
		}
	}
}

double CastRay(double ray_angle, double look_angle, struct Float2D origin, struct sLevel_Data* lvl, struct Float2D* ray_hit, char* x_or_y)
{
	struct Float2D hit_x = {0}, hit_y = {0}, offset = {0};
	double angle = ray_angle + look_angle, distX=1024, distY=1024, tangent = tan(angle), cotangent = 1/tangent, correction_angle = 0;
	int count = 0, index;

	if(angle>=FULL_CIRCLE) angle-=FULL_CIRCLE; if(angle<0) angle+=FULL_CIRCLE;

	if(angle<QUARTER_CIRCLE || angle>THREE_QUARTER_CIRCLE) { hit_x.X = (int)origin.X + 1; hit_x.Y = tangent * (hit_x.X - origin.X) + origin.Y; offset.X = 1; offset.Y = offset.X * tangent; }
	else if(angle>QUARTER_CIRCLE && angle<THREE_QUARTER_CIRCLE) { hit_x.X = (int)origin.X; hit_x.Y = tangent * (hit_x.X - origin.X) + origin.Y; offset.X = -1; offset.Y = offset.X * tangent; }

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
			distX = sqrt((origin.X-hit_x.X)*(origin.X-hit_x.X) + (origin.Y-hit_x.Y)*(origin.Y-hit_x.Y));
			break;
		}
		else { hit_x.X += offset.X; hit_x.Y += offset.Y; ++count; }
	}

	count = 0;
	if(angle<M_PI) { hit_y.Y = (int)origin.Y + 1; hit_y.X = cotangent * (hit_y.Y - origin.Y) + origin.X; offset.Y = 1; offset.X = offset.Y * cotangent; }
	else if(angle>M_PI) { hit_y.Y = (int)origin.Y; hit_y.X = cotangent * (hit_y.Y - origin.Y) + origin.X; offset.Y = -1; offset.X = offset.Y * cotangent; }

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
			distY = sqrt((origin.X-hit_y.X)*(origin.X-hit_y.X) + (origin.Y-hit_y.Y)*(origin.Y-hit_y.Y));
			break;		
		}
		else { hit_y.X += offset.X; hit_y.Y += offset.Y; ++count; }
	}

	if(distX < distY)
	{
		if(x_or_y != NULL) *x_or_y = 1;
		if(ray_hit != NULL) { ray_hit->X = hit_x.X; ray_hit->Y = hit_x.Y; }
		return distX * cos(ray_angle);
	}
	else
	{
		if(x_or_y != NULL) *x_or_y = 0;
		if(ray_hit != NULL) { ray_hit->X = hit_y.X; ray_hit->Y = hit_y.Y; }
		return distY * cos(ray_angle);
	}
}

void CalcVision(struct sCamera* cam, struct sLevel_Data* lvl)
{
	//Do raycast.
	////double fan_start = wrld->Player->Look - FIELD_OF_VIEW_HALF, fan_running = 0;
	//double columns_per_cast = view->ViewSize.X * COLUMNS_FACTOR;
	//double x_draw_size = 0, remainder = 0;
	//int x_draw_position = view->ViewPos.X, num_pixels = 0;

	unsigned int ray_count = 0;
	unsigned int angle_count = 0;
	struct Float2D ray_hit;

	while(ray_count < cam->NumRays)
	{
		cam->Distances[ray_count] = CastRay(-cam->Angles[angle_count], cam->Look, cam->Location, lvl, &ray_hit, NULL);
		cam->Hits[ray_count].X = ray_hit.X;
		cam->Hits[ray_count].Y = ray_hit.Y;
		++ray_count;
		cam->Distances[ray_count] = CastRay(cam->Angles[angle_count], cam->Look, cam->Location, lvl, &ray_hit, NULL);
		cam->Hits[ray_count].X = ray_hit.X;
		cam->Hits[ray_count].Y = ray_hit.Y;
		++ray_count;
		++angle_count;
		
		//x_draw_size = columns_per_cast + remainder;
		//num_pixels = (int)x_draw_size;
		//remainder = x_draw_size - num_pixels;
		//while(num_pixels>0)
		//{
		//	SDL_RenderDrawLine(renderer, x_draw_position, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position, (int)(view->ViewSize.Y + draw_height) / 2);
		//	++x_draw_position;
		//	--num_pixels;
		//}
		//fan_running += VIEW_CAST_INCREMENT;
		//fan_running = round(fan_running * 10000000)/10000000;
	}
}