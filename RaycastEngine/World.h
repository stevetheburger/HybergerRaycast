#pragma once

#include "Int2D.h"
#include "Float2D.h"

//Structure for the tile data - individual square.
struct sTile_Data
{
	struct Int2D Location;
	unsigned short Type;
};

//Structure for the level data - 2D plane of squares
struct sLevel_Data
{
	struct sEntity_Queue* EntityQueue; //Entities in level.
	struct sTile_Data* TileData;
	struct Int2D Size;
};

//Structure for the entire world - multiple levels or overworld.
struct sWorld_Data
{
	struct sEntity_Data* Player; //The player entity
	struct sLevel_Data* LevelData;
	unsigned char LevelCount;
};

//Create default world from code.
struct sWorld_Data* CreateWorldHardcode();
//Create world from specified file.
struct sWorld_Data* CreateWorldFromFile(const char*);
//Destroy world at teardown.
void DestroyWorld(struct sWorld_Data**);
//Write world.
void WorldToFile(struct sWorld_Data*, const char*);

//Set the player.
void SetPlayerEntity(struct sWorld_Data*, struct sEntity_Data*);

//Gets tile by vertex, integer or float (ignores decimal portion).
struct sTile_Data* GetTileByInt(struct sLevel_Data*, struct Int2D);
struct sTile_Data* GetTileByFloat(struct sLevel_Data*, struct Float2D);