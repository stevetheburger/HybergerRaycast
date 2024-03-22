#pragma once

#include <SDL.h>
#include "Float2D.h"
#include "Int2D.h"
#include "World.h"

#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 4
#define PLAYER_FEET_PER_SEC 3
#define PLAYER_TURN_RATE_PER_SEC M_PI
#define FULL_CIRCLE 6.28318531
#define W_KEY 0b00000001
#define A_KEY 0b00000010
#define S_KEY 0b00000100
#define D_KEY 0b00001000
#define Q_KEY 0b00010000
#define E_KEY 0b00100000

//A special controller structure for the player. May get used for AI as well.
struct sPlayer_Controller
{
	char IsPlayer;
	char Keys;
};

struct sEntity_Data 
{
	unsigned char Lvl;
	struct Float2D Location;
	struct Float2D Velocity;
	double Look;
	unsigned char Type;
	struct sPlayer_Controller Controller;
};

//Initialization and teardown.
struct sEntity_Data* CreateDefaultEntity();
struct sEntity_Data* CreateEntity(struct Float2D, double, unsigned char, unsigned char, char);
struct sEntity_Data* EntityDeepCopy(struct sEntity_Data*);
struct sEntity_Data* CreatePlayerEntity(struct Float2D, double, unsigned char, char);
void DestroyEntity(struct sEntity_Data**);

//Runtime functions
inline void DoInput(struct sPlayer_Controller* cntrl, SDL_Event* event)
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

inline void DoMove(struct sEntity_Data* ent, double delta)
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
	}
}