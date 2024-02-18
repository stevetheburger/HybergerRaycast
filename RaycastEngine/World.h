#pragma once

#include "Entity.h"
#include "EntityQueue.h"
#include "Int2D.h"
#include "Float2D.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

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

//Getters for the world data.
//inline struct sLevel_Data* GetLevelData(struct sWorld_Data* wrld_data, unsigned char lvl) 
//{
//	struct sLevel_Data* lvl_data = NULL;
//	if(wrld_data != NULL && wrld_data->LevelData != NULL && lvl < wrld_data->LevelCount)
//	{
//		lvl_data = &wrld_data->LevelData[lvl];
//	}
//	
//	return lvl_data;
//}
//inline struct sTile_Data* GetTileData(struct sLevel_Data* lvl_data, struct Int2D loc) 
//{ 
//	struct sTile_Data* tile_data = NULL;
//	if(
//		lvl_data != NULL && lvl_data->TileData != NULL && 
//		loc.Y >= 0 && loc.X >= 0 && 
//		loc.Y < lvl_data->Size.Y && loc.X < lvl_data->Size.X
//	) {
//		tile_data = &lvl_data->TileData[loc.Y * lvl_data->Size.X + loc.X];
//	}
//	
//	return tile_data;
//}
