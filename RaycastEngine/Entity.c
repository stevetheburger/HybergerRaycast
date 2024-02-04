#include <stdlib.h>
#include <SDL.h>
#include "Entity.h"

struct sEntity_Data* CreateEntity(struct Float2D loc, double dir, unsigned char lvl, unsigned char type, char is_player)
{
	struct sEntity_Data* ret_val = malloc(sizeof(struct sEntity_Data));
	if(ret_val != NULL)
	{
		ret_val->Lvl = lvl;
		ret_val->Location.X = loc.X;
		ret_val->Location.Y = loc.Y;
		ret_val->Velocity.X = 0;
		ret_val->Velocity.Y = 0;
		if(dir >= FULL_CIRCLE)
			ret_val->Look = dir - FULL_CIRCLE;
		else if(dir < 0)
			ret_val->Look = dir + FULL_CIRCLE;
		else
			ret_val->Look = dir;

		ret_val->Type = type;
		ret_val->Controller.IsPlayer = is_player;
		ret_val->Controller.w_down = ret_val->Controller.a_down = ret_val->Controller.s_down = ret_val->Controller.d_down = ret_val->Controller.q_down = ret_val->Controller.e_down = 0;
	}

	return ret_val;
}

struct sEntity_Data* CreatePlayerEntity(struct Float2D loc, double dir, unsigned char lvl, char is_player)
{
	return CreateEntity(loc, dir, lvl, 1, is_player);
}

void DestroyEntity(struct sEntity_Data* ent_ptr)
{
	free(ent_ptr);
}