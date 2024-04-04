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

//Runtime functions
void DoInput(struct sPlayer_Controller* cntrl, SDL_Event* event)
{
	if(cntrl->IsPlayer)
	{
		//Get key state if down.
		if(event->type == SDL_KEYDOWN)
		{
			if(event->key.keysym.sym == SDLK_w)
			{
				cntrl->Keys |= W_KEY;
				if(cntrl->Keys & S_KEY) 
					cntrl->Keys &= ~S_KEY;
			}
			else if(event->key.keysym.sym == SDLK_d)
			{
				cntrl->Keys |= D_KEY;
				if(cntrl->Keys & A_KEY) 
					cntrl->Keys &= ~A_KEY;				
			}
			else if(event->key.keysym.sym == SDLK_s)
			{
				cntrl->Keys |= S_KEY;
				if(cntrl->Keys & W_KEY) 
					cntrl->Keys &= ~W_KEY;
			}
			else if(event->key.keysym.sym == SDLK_a)
			{
				cntrl->Keys |= A_KEY;
				if(cntrl->Keys & D_KEY) 
					cntrl->Keys &= ~D_KEY;
			}
			else if(event->key.keysym.sym == SDLK_q)
			{
				cntrl->Keys |= Q_KEY;
				if(cntrl->Keys & E_KEY) 
					cntrl->Keys &= ~E_KEY;
			}
			else if(event->key.keysym.sym == SDLK_e)
			{
				cntrl->Keys |= E_KEY;
				if(cntrl->Keys & D_KEY) 
					cntrl->Keys &= ~D_KEY;
			}
		}
		//Get key state if up.
		else if(event->type == SDL_KEYUP)
		{
			if(event->key.keysym.sym == SDLK_w)
			{
				cntrl->Keys &= ~W_KEY;
			}
			else if(event->key.keysym.sym == SDLK_d)
			{
				cntrl->Keys &= ~D_KEY;
			}
			else if(event->key.keysym.sym == SDLK_s)
			{
				cntrl->Keys &= ~S_KEY;
			}
			else if(event->key.keysym.sym == SDLK_a)
			{
				cntrl->Keys &= ~A_KEY;
			}
			else if(event->key.keysym.sym == SDLK_q)
			{
				cntrl->Keys &= ~Q_KEY;
			}
			else if(event->key.keysym.sym == SDLK_e)
			{
				cntrl->Keys &= ~E_KEY;
			}
		}
	}
}

void DoMove(struct sEntity_Data* ent, double delta)
{
	static struct Float2D Direction;

	if(ent->Controller.Keys & Q_KEY)
	{
		ent->Look -= PLAYER_TURN_RATE_PER_SEC * delta;
		if(ent->Look >= FULL_CIRCLE)
			ent->Look -= FULL_CIRCLE;
		else if(ent->Look < 0)
			ent->Look += FULL_CIRCLE;
	}
	if(ent->Controller.Keys & E_KEY)
	{
		ent->Look += PLAYER_TURN_RATE_PER_SEC * delta;
		if(ent->Look >= FULL_CIRCLE)
			ent->Look -= FULL_CIRCLE;
		else if(ent->Look < 0)
			ent->Look += FULL_CIRCLE;
	}

	Direction.X = cos(ent->Look);
	Direction.Y = sin(ent->Look);

	//Calculate velocity from key state.
	if(ent->Controller.Keys & W_KEY)
	{
		ent->Velocity.X += Direction.X;
		ent->Velocity.Y += Direction.Y;
	}
	if(ent->Controller.Keys & S_KEY)
	{
		ent->Velocity.X += -Direction.X;
		ent->Velocity.Y += -Direction.Y;
	}
	if(ent->Controller.Keys & D_KEY)
	{
		ent->Velocity.X += -Direction.Y;
		ent->Velocity.Y += Direction.X;
	}
	if(ent->Controller.Keys & A_KEY)
	{
		ent->Velocity.X += Direction.Y;
		ent->Velocity.Y += -Direction.X;
	}

	//Normalize and apply.
	if(ent->Velocity.X != 0 || ent->Velocity.Y != 0) 
	{	
		double magnitude = sqrt((ent->Velocity.X) * (ent->Velocity.X) + (ent->Velocity.Y) * (ent->Velocity.Y));
		ent->Velocity.X /= magnitude;
		ent->Velocity.Y /= magnitude;
		ent->Velocity.X *= PLAYER_FEET_PER_SEC * delta;
		ent->Velocity.Y *= PLAYER_FEET_PER_SEC * delta;

		ent->Location = fAdd(ent->Location, ent->Velocity);
		ent->Velocity.X = ent->Velocity.Y = 0;
	}
}