#include <stdlib.h>
#include <SDL.h>
#include "Entity.h"

struct sEntity_Data* CreateDefaultEntity()
{
	struct sEntity_Data* ret_val = malloc(sizeof(struct sEntity_Data));
	if(ret_val != NULL)
	{
		ret_val->Lvl = 0;
		ret_val->Location.X = 0;
		ret_val->Location.Y = 0;
		ret_val->Velocity.X = 0;
		ret_val->Velocity.Y = 0;
		ret_val->Look = 0;
		ret_val->Type = 0;
		ret_val->Controller.IsPlayer = 0;
		ret_val->Controller.Keys = 0;
	}
	return ret_val;
}

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
		ret_val->Controller.Keys = 0;
	}

	return ret_val;
}

struct sEntity_Data* CreatePlayerEntity(struct Float2D loc, double dir, unsigned char lvl, char is_player)
{
	return CreateEntity(loc, dir, lvl, 1, is_player);
}

void DestroyEntity(struct sEntity_Data** ent_ptr)
{
	if(ent_ptr != NULL && *ent_ptr != NULL)
		free(*ent_ptr);
}

struct sEntity_Data* EntityDeepCopy(struct sEntity_Data* original)
{	
	struct sEntity_Data* ret_val = NULL;
	if(original != NULL)
	{
		ret_val = malloc(sizeof(struct sEntity_Data));
		if(ret_val != NULL)
		{
			ret_val->Lvl = original->Lvl;
			ret_val->Location.X = original->Location.X;
			ret_val->Location.Y = original->Location.Y;
			ret_val->Velocity.X = original->Velocity.X;
			ret_val->Velocity.Y = original->Velocity.Y;
			ret_val->Look = original->Look;
			ret_val->Type = original->Type;
			ret_val->Controller.IsPlayer = original->Controller.IsPlayer;
			ret_val->Controller.Keys = original->Controller.Keys;
		}
	}
	return ret_val;
}