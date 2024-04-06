#pragma once

#include <SDL.h>

#include "Float2D.h"
#include "Int2D.h"

#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 4
#define PLAYER_FEET_PER_SEC 3
#define PLAYER_TURN_RATE_PER_SEC M_PI
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
	struct sCamera* Camera;
};

//Initialization and teardown.
struct sEntity_Data* CreateDefaultEntity();
struct sEntity_Data* CreateEntity(struct Float2D, double, unsigned char, unsigned char, char);
struct sEntity_Data* EntityDeepCopy(struct sEntity_Data*);
struct sEntity_Data* CreatePlayerEntity(struct Float2D, double, unsigned char, char);
void DestroyEntity(struct sEntity_Data**);

//Update and control IO functions.
void DoInput(struct sPlayer_Controller*, SDL_Event*);
void DoMove(struct sEntity_Data*, double);

void SetCamera(struct sEntity_Data*, struct sCamera*);
