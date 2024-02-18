#pragma once

#include "Entity.h"
#include "EntityQueue.h"
#include "Int2D.h"
#include "Float2D.h"

struct sTile_Data
{
	struct Int2D Location;
	unsigned char Type;
};

struct sLevel_Data
{
	struct sEntity_Queue* EntityQueue;
	struct sTile_Data* TileData;
	struct Int2D Size;
};

struct sWorld_Data
{
	struct sEntity_Data* Player;
	struct sLevel_Data* LevelData;
	unsigned char LevelCount;
};

//Function declarations.
struct sWorld_Data* CreateWorldHardcode();
struct sWorld_Data* CreateWorldFromFile(const char*);
void WorldToFile(struct sWorld_Data*, const char*);
void DestroyWorld(struct sWorld_Data**);
void SetPlayerEntity(struct sWorld_Data*, struct sEntity_Data*);
struct sTile_Data* GetTileByInt(struct sLevel_Data*, struct Int2D);
struct sTile_Data* GetTileByFloat(struct sLevel_Data*, struct Float2D);