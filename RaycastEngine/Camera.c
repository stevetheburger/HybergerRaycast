#include "Camera.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "TrigConstants.h"
#include "World.h"
#include "Float2D.h"

//Creates camera with specified field of view, specified number of rays, at the specified location, and angle.
struct sCamera* CreateCamera(double fov, unsigned int num_rays, struct Float2D loc, double look)
{
	struct sCamera* ret_val = NULL;
	//Check field of view.
	if(fov > 0)
	{
		//Create camera object.
		ret_val = malloc(sizeof(struct sCamera));
		if(ret_val != NULL)
		{
			//Allocate the hits and distances arrays. Make sure they are zeroed.
			ret_val->Hits = calloc(num_rays, sizeof(struct Float2D));
			ret_val->Distances = calloc(num_rays, sizeof(double));

			//Allocates the angles array. Only half of the arc is calculated, as both halves are mirrored.
			ret_val->Angles = malloc(sizeof(double) * num_rays);
			if(ret_val->Hits != NULL && ret_val->Distances != NULL && ret_val->Angles != NULL)
			{
				//If all allocations are successfull, initialize.
				ret_val->Fov = fov;
				ret_val->NumRays = num_rays;
				ret_val->Location.X = loc.X; ret_val->Location.Y = loc.Y;
				ret_val->Look = look;

				//Do calculation of angles through view plane in front of player, to ensure that the view angles are optimal.
				double x_incr = tan(fov/2) * 2 / num_rays;
				double x_start =  x_incr/2-(num_rays / 2) * x_incr;
				for(unsigned int i = 0; i < num_rays; ++i)
				{
					//Get angle.
					ret_val->Angles[i] = atan(x_start);
					//Next position in plane.
					x_start += x_incr;
					//Round for accuracy.
					x_start = round(x_start * 10000000)/10000000;
				}
			}
			else
				//If allocation is unsuccesful ensure allocation is atomic by destroying camera object.
				DestroyCamera(&ret_val);
		}
	}
	return ret_val;
}
//Destroys camera at teardown.
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

/// <summary>
/// Do individual raycast.
/// </summary>
/// <param name="ray_angle">The angle of the ray (precalculated in camera object)</param>
/// <param name="look_angle">The angle of the camera</param>
/// <param name="origin">The position of the camera in the world</param>
/// <param name="lvl">The level that the camera is on</param>
/// <param name="ray_hit">The float vertex that the ray hit a wall at</param>
/// <param name="x_or_y">whether the wall is an x or a y hit (for lighting purposes)</param>
/// <returns></returns>
double CastRay(double ray_angle, double look_angle, struct Float2D origin, struct sLevel_Data* lvl, struct Float2D* ray_hit, char* x_or_y)
{
	//Create intermediary variables.
	struct Float2D hit_x = {0}, hit_y = {0}, offset = {0};
	//Angle is look the angle of the camera in the world added to the precalculated angle of the ray.
	double angle = ray_angle + look_angle, distX=1024, distY=1024, tangent = tan(angle), cotangent = 1/tangent;
	int count = 0, index;

	//Ensure that the angle is normalized.
	if(angle>=FULL_CIRCLE) angle-=FULL_CIRCLE; if(angle<0) angle+=FULL_CIRCLE;

	//Set up increment.
	//If East/Right
	if(angle<QUARTER_CIRCLE || angle>THREE_QUARTER_CIRCLE) { hit_x.X = (int)origin.X + 1; hit_x.Y = tangent * (hit_x.X - origin.X) + origin.Y; offset.X = 1; offset.Y = offset.X * tangent; }
	//If West/Left
	else if(angle>QUARTER_CIRCLE && angle<THREE_QUARTER_CIRCLE) { hit_x.X = (int)origin.X; hit_x.Y = tangent * (hit_x.X - origin.X) + origin.Y; offset.X = -1; offset.Y = offset.X * tangent; }

	//Calculate incrementally for vertical walls until limit is reached or wall hit.
	while(count < VIEW_MAX)
	{
		//Calculate index for array.
		index = (int)hit_x.Y * lvl->Size.X + (int)hit_x.X;
		if
		(
			hit_x.Y < lvl->Size.Y && 
			hit_x.Y >= 0 && 
			(lvl->TileData[index].Type || lvl->TileData[index - 1].Type)
		)
		{
			//If wall is hit, do distance formula on hit and set to return it.
			distX = sqrt((origin.X-hit_x.X)*(origin.X-hit_x.X) + (origin.Y-hit_x.Y)*(origin.Y-hit_x.Y));
			break;
		}
		//If no hit, do next increment.
		else { hit_x.X += offset.X; hit_x.Y += offset.Y; ++count; }
	}

	//Reset counter for horizontal walls.
	count = 0;
	//Set up increment.
	//If North/"Up"
	if(angle<M_PI) { hit_y.Y = (int)origin.Y + 1; hit_y.X = cotangent * (hit_y.Y - origin.Y) + origin.X; offset.Y = 1; offset.X = offset.Y * cotangent; }
	//If South/"Down"
	else if(angle>M_PI) { hit_y.Y = (int)origin.Y; hit_y.X = cotangent * (hit_y.Y - origin.Y) + origin.X; offset.Y = -1; offset.X = offset.Y * cotangent; }

	//Calculate incrementally for horizontal walls until limit is reached or wall hit.
	while(count < VIEW_MAX)
	{
		//Calculate index for array.
		index = (int)hit_y.Y * lvl->Size.X + (int)hit_y.X;
		if
		(	
			hit_y.X < lvl->Size.X && 
			hit_y.X >= 0 && 
			(lvl->TileData[index].Type || lvl->TileData[index - lvl->Size.X].Type)
		)
		{
			//If wall is hit, do distance formula on hit and set to return it.
			distY = sqrt((origin.X-hit_y.X)*(origin.X-hit_y.X) + (origin.Y-hit_y.Y)*(origin.Y-hit_y.Y));
			break;		
		}
		//If no hit, do next increment.
		else { hit_y.X += offset.X; hit_y.Y += offset.Y; ++count; }
	}

	//Check if vertical or horizontal distance is shorter. Return the shorter distance.
	if(distX < distY)
	{
		if(x_or_y != NULL) *x_or_y = 1;
		if(ray_hit != NULL) { ray_hit->X = hit_x.X; ray_hit->Y = hit_x.Y; }
		//Return perpindicular distance in order to correct for fish-eye view.
		return distX * cos(ray_angle);
	}
	else
	{
		if(x_or_y != NULL) *x_or_y = 0;
		if(ray_hit != NULL) { ray_hit->X = hit_y.X; ray_hit->Y = hit_y.Y; }
		//Return perpindicular distance in order to correct for fish-eye view.
		return distY * cos(ray_angle);
	}
}

void CalcVision(struct sCamera* cam, struct sLevel_Data* lvl)
{
	//Counters.
	unsigned int ray_count = 0;
	unsigned int angle_count = 0;
	//Intermediate variable.
	struct Float2D ray_hit;

	//Loop through all the rays and calculate the player's vision in the world against the walls.
	for(unsigned int ray_cout; ray_count < cam->NumRays; ++ray_count)
	{
		//Calculate from the center to the left.
		cam->Distances[ray_count] = CastRay(cam->Angles[ray_count], cam->Look, cam->Location, lvl, &ray_hit, NULL);
		//Store hit for overhead view.
		cam->Hits[ray_count].X = ray_hit.X;
		cam->Hits[ray_count].Y = ray_hit.Y;
	}
}