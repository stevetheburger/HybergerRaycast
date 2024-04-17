#pragma once

#include <SDL.h>

#include "Float2D.h"
#include "Int2D.h"

//Size of the player in overhead view.
#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 4

//Rates for movement and look.
#define PLAYER_FEET_PER_SEC 3
#define PLAYER_TURN_RATE_PER_SEC M_PI

//Control bit flag mask constants.
#define W_KEY 0b00000001
#define A_KEY 0b00000010
#define S_KEY 0b00000100
#define D_KEY 0b00001000
#define Q_KEY 0b00010000
#define E_KEY 0b00100000

//A special controller structure for the player. May get used for AI as well.
struct sPlayer_Controller
{
	char IsPlayer; //Whether the entity is the human player.
	char Keys; //Bit flags for key inputs.
};

struct sEntity_Data 
{
	unsigned char Lvl; //Current plane
	struct Float2D Location;
	struct Float2D Velocity;
	double Look; //Angle one is looking at from East/left, clockwise.
	unsigned char Type; //Type of entity. May be used as an ID later on.
	struct sPlayer_Controller Controller; //The controller structure.
	struct sCamera* Camera; //Camera to follow a player.
};

//Creates an empty default entity.
struct sEntity_Data* CreateDefaultEntity();
//Creates specified entity on the specified level, position, and angle.
struct sEntity_Data* CreateEntity(struct Float2D, double, unsigned char, unsigned char, char);
//Makes a deep copy (clone) of an entity. Pointer will point to new entity object identical to the one supplied, but not the same object itself.
struct sEntity_Data* EntityDeepCopy(struct sEntity_Data*);
//Makes the default player entity at the specified level, position, and angle.
struct sEntity_Data* CreatePlayerEntity(struct Float2D, double, unsigned char, char);
//Destroy entity at teardown.
void DestroyEntity(struct sEntity_Data**);

//Update and control IO functions.
void DoInput(struct sPlayer_Controller*, SDL_Event*);
void DoMove(struct sEntity_Data*, double);

//Sets the camera to follow, presumably player entity.
void SetCamera(struct sEntity_Data*, struct sCamera*);
