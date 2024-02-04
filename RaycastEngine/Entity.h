#pragma once

#include <SDL.h>
#include "Float2D.h"

#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 4
#define PLAYER_FEET_PER_SEC 3
#define PLAYER_TURN_RATE_PER_SEC 2 * M_PI
#define FULL_CIRCLE 2 * M_PI

//A special controller structure for the player. May get used for AI as well.
struct sPlayer_Controller
{
	char IsPlayer;
	char w_down, d_down, s_down, a_down, q_down, e_down;
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
struct sEntity_Data* CreateEntity(struct Float2D, double, unsigned char, unsigned char, char);
struct sEntity_Data* CreatePlayerEntity(struct Float2D, double, unsigned char, char);
void DestroyEntity(struct sEntity_Data *);

//Runtime functions
inline void DoInput(struct sPlayer_Controller* cntrl)
{
	if(cntrl->IsPlayer)
	{
		static SDL_Event event;
	
		while(SDL_PollEvent(&event))
		{
			//Get key state if down.
			if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_w)
				{
					cntrl->w_down = 1;
				}
				else if(event.key.keysym.sym == SDLK_d)
				{
					cntrl->d_down = 1;
				}
				else if(event.key.keysym.sym == SDLK_s)
				{
					cntrl->s_down = 1;
				}
				else if(event.key.keysym.sym == SDLK_a)
				{
					cntrl->a_down = 1;
				}
				else if(event.key.keysym.sym == SDLK_q)
				{
					cntrl->q_down = 1;
				}
				else if(event.key.keysym.sym == SDLK_e)
				{
					cntrl->e_down = 1;
				}
			}
			//Get key state if up.
			else if(event.type == SDL_KEYUP)
			{
				if(event.key.keysym.sym == SDLK_w)
				{
					cntrl->w_down = 0;
				}
				else if(event.key.keysym.sym == SDLK_d)
				{
					cntrl->d_down = 0;
				}
				else if(event.key.keysym.sym == SDLK_s)
				{
					cntrl->s_down = 0;
				}
				else if(event.key.keysym.sym == SDLK_a)
				{
					cntrl->a_down = 0;
				}
				else if(event.key.keysym.sym == SDLK_q)
				{
					cntrl->q_down = 0;
				}
				else if(event.key.keysym.sym == SDLK_e)
				{
					cntrl->e_down = 0;
				}
			}
			////Detect mouse-x movement.
			//else if(event.type == SDL_MOUSEMOTION)
			//{
			//	printf("%i\n", event.motion.xrel);
			//	cntrl->mouse_x += event.motion.xrel * delta;
			//	if(cntrl->mouse_x >= 360.0f) cntrl->mouse_x -= 360.0f;
			//	else if(cntrl->mouse_x < 0.0f) cntrl->mouse_x += 360.0f;
			//}
		}
	}
}

inline void DoMove(struct sEntity_Data* ent, double delta)
{
	static struct Float2D Direction;

	if(ent->Controller.q_down)
	{
		ent->Look -= PLAYER_TURN_RATE_PER_SEC * delta;
		if(ent->Look >= FULL_CIRCLE)
			ent->Look -= FULL_CIRCLE;
		else if(ent->Look < 0)
			ent->Look += FULL_CIRCLE;
	}
	if(ent->Controller.e_down)
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
	if(ent->Controller.w_down)
	{
		ent->Velocity.X += Direction.X;
		ent->Velocity.Y += Direction.Y;
	}
	if(ent->Controller.s_down)
	{
		ent->Velocity.X += -Direction.X;
		ent->Velocity.Y += -Direction.Y;
	}
	if(ent->Controller.d_down)
	{
		ent->Velocity.X += -Direction.Y;
		ent->Velocity.Y += Direction.X;
	}
	if(ent->Controller.a_down)
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
