#include "World.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Int2D.h"
#include "EntityQueue.h"
#include "Entity.h"

//Hard coded world.
#define NUM_LVL 1
#define LEVEL_DIM (unsigned int[]){16, 16}
#define LVL_1_TILE (unsigned char[]){\
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,\
1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,\
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,\
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,\
1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,1,\
1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,\
1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,1,\
1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,\
1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,1,\
1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,\
1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,1,\
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,\
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

//Entity map.
#define LVL_1_ENT (unsigned char[]){\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

//Create the hardcoded world above.
struct sWorld_Data* CreateWorldHardcode()
{
	struct sEntity_Data* plyr_data = NULL;
	
	//Create world structure.
	struct sWorld_Data* wrld_data = malloc(sizeof(struct sWorld_Data));
	if(wrld_data == NULL)
		return NULL;

	//Create level structure.
	wrld_data->LevelData = malloc(sizeof(struct sLevel_Data) * NUM_LVL);
	if(wrld_data->LevelData == NULL)
		goto BAD_CREATE;
	wrld_data->LevelCount = NUM_LVL;
	wrld_data->LevelData[0].TileData = malloc(sizeof(struct sTile_Data) * LEVEL_DIM[0] * LEVEL_DIM[1]);
	if(wrld_data->LevelData[0].TileData == NULL)
		goto BAD_CREATE;
	wrld_data->LevelData[0].EntityQueue = CreateEntityQueue();
	if(wrld_data->LevelData[0].EntityQueue == NULL)
		goto BAD_CREATE;

	wrld_data->LevelData[0].Size.X = LEVEL_DIM[0];
	wrld_data->LevelData[0].Size.Y = LEVEL_DIM[1];
	unsigned int count = 0;
	char player_set = 0;
	
	//Loop through level.
	while(count < LEVEL_DIM[0] * LEVEL_DIM[1])
	{
		//Initialize tiles.
		wrld_data->LevelData[0].TileData[count].Type = LVL_1_TILE[count];
		wrld_data->LevelData[0].TileData[count].Location = (struct Int2D){count % LEVEL_DIM[0], count / LEVEL_DIM[0]};
		//Found player, create player.
		if(LVL_1_ENT[count] == 1)
		{
			struct sEntity_Data* plyr_data = CreatePlayerEntity((struct Float2D){count % LEVEL_DIM[0] + 0.5, count / LEVEL_DIM[0] + 0.5}, 0, 0, 1);
			if(plyr_data == NULL)
				goto BAD_CREATE;

			//Add player to entity queue for level
			EnqueueEntity(wrld_data->LevelData[0].EntityQueue, plyr_data);
			wrld_data->Player = plyr_data;
		}
		++count;
	}

	return wrld_data;

	//In case the allocation doesn't work properly.
	BAD_CREATE:
	if(plyr_data != NULL) free(plyr_data);
	DestroyWorld(&wrld_data);
	return NULL;
}

struct sWorld_Data* CreateWorldFromFile(const char *file_path)
{
	//The return variable.
	struct sWorld_Data* wrld_data = NULL;
	struct sEntity_Data* entity_buffer = NULL;
	void* cpy_buffer = NULL;
	FILE* file_ptr;

	//Open file and make sure it exists.
	fopen_s(&file_ptr, file_path, "rb");
	if(file_ptr == NULL)
		return NULL;

	//Create buffer for the dimensions of the world.
	cpy_buffer = malloc(sizeof(struct Int2D));
	if(cpy_buffer == NULL)
		goto BAD_CREATE;

	//Create world structure.
	wrld_data = malloc(sizeof(struct sWorld_Data));
	if(wrld_data == NULL)
		goto BAD_CREATE;

	//Read number of levels from the file.
	if(fread(cpy_buffer, 1, 1, file_ptr) != 1)
		goto BAD_CREATE;

	//Initialize world structure and create the array of level data structures.
	wrld_data->LevelCount = *(unsigned char *)cpy_buffer;
	wrld_data->LevelData = malloc(sizeof(struct sLevel_Data) * wrld_data->LevelCount);
	if(wrld_data->LevelData == NULL)
		goto BAD_CREATE;
	
	//Create entity buffer from which to read entity data from the file.
	entity_buffer = CreateDefaultEntity();
	if(entity_buffer == NULL)
		goto BAD_CREATE;

	//Loop through the levels.
	unsigned char count = 0;
	while(count < wrld_data->LevelCount)
	{
		//Read and create the level data.
		if(fread(cpy_buffer, sizeof(struct Int2D), 1, file_ptr) != 1)
			goto BAD_CREATE;
		wrld_data->LevelData[count].Size = *(struct Int2D *)cpy_buffer;
		wrld_data->LevelData[count].TileData = malloc(sizeof(struct sTile_Data) * iToSize(wrld_data->LevelData[count].Size));
		if(wrld_data->LevelData[count].TileData == NULL)
			goto BAD_CREATE;
		wrld_data->LevelData[count].EntityQueue = CreateEntityQueue();
		if(wrld_data->LevelData[count].EntityQueue == NULL)
			goto BAD_CREATE;

		//Check if there is entities present.
		if(fread(cpy_buffer, sizeof(int), 1, file_ptr) != 1)
			goto BAD_CREATE;
		//Get tile data.
		if(fread(wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), iToSize(wrld_data->LevelData[count].Size), file_ptr) != iToSize(wrld_data->LevelData[count].Size))
			goto BAD_CREATE;

		//Loop through entites if present.
		for(int i = 0; i < *(int *)cpy_buffer; ++i)
		{
			//Read entity data.
			if(entity_buffer == NULL)
				goto BAD_CREATE;
			if(fread(entity_buffer, sizeof(struct sEntity_Data), 1, file_ptr) != 1)
				goto BAD_CREATE;
			
			//If entity is player, add player to world.
			if(entity_buffer->Controller.IsPlayer) 
			{	
				wrld_data->Player = EntityDeepCopy(entity_buffer);
				EnqueueEntity(wrld_data->LevelData[count].EntityQueue, wrld_data->Player);
			}
			else
				EnqueueEntity(wrld_data->LevelData[count].EntityQueue, EntityDeepCopy(entity_buffer));
		}

		++count;
	}

	//Free intermediates and close file.
	free(entity_buffer);
	free(cpy_buffer);
	fclose(file_ptr);

	return wrld_data;

	//Goto in case of error on creation.
	BAD_CREATE:
	if(cpy_buffer != NULL) free(cpy_buffer);
	if(file_ptr != NULL) fclose(file_ptr);
	if(wrld_data != NULL) DestroyWorld(&wrld_data);
	if(entity_buffer != NULL) free(entity_buffer);
	return NULL;
}

void WorldToFile(struct sWorld_Data* wrld_data, const char* file_path)
{
	//File to open.
	FILE* file_ptr = NULL;

	//Null and sanity checks.
	if(wrld_data == NULL)
		return;
	if(wrld_data->LevelData == NULL || wrld_data->LevelCount <= 0)
		return;

	//Attempt to open file.
	fopen_s(&file_ptr, file_path, "wb");
	if(file_ptr == NULL)
		return;

	//Create necessary structure for entity data shunting.
	struct sEntity_Queue* shunt_yard = CreateEntityQueue();
	if(shunt_yard == NULL)
	{
		fclose(file_ptr);
		remove(file_path);
		return;
	}

	//Create world header.
	fwrite(&wrld_data->LevelCount, 1, 1, file_ptr);

	unsigned char count = 0;

	//Loop through levels and write the levels.
	while(count < wrld_data->LevelCount)
	{
		//Get level size and do a null and sanity check.
		int lvl_size = iToSize(wrld_data->LevelData[count].Size);
		if(wrld_data->LevelData[count].TileData == NULL || lvl_size <= 0)
		{
			fclose(file_ptr);
			remove(file_path);
			DestroyEntityQueue(&shunt_yard);
			return;
		}	

		//Write level header.
		fwrite(&wrld_data->LevelData[count].Size, sizeof(struct Int2D), 1, file_ptr);
		fwrite(&wrld_data->LevelData[count].EntityQueue->Count, sizeof(int), 1, file_ptr);
		fwrite(wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), lvl_size, file_ptr);

		//Write the entities one by one and then shunt each off to a temporary queue that lasts the duration of the save operation.
		struct sEntity_Data* current = DequeueEntity(wrld_data->LevelData[count].EntityQueue);
		do
		{
			fwrite(current, sizeof(struct sEntity_Data), 1, file_ptr);
			EnqueueEntity(shunt_yard, current);
			current = DequeueEntity(wrld_data->LevelData[count].EntityQueue);
		} while(current != NULL);

		//Enqueue the entities back onto the main queue to prevent them from being lost.
		current = DequeueEntity(shunt_yard);
		do
		{
			EnqueueEntity(wrld_data->LevelData[count].EntityQueue, current);
			current = DequeueEntity(shunt_yard);
		} while(current != NULL);

		//Next level.
		++count;
	}

	free(shunt_yard);
	fclose(file_ptr);
}

//Destroy world data on teardown.
void DestroyWorld(struct sWorld_Data** wrld_ptr)
{
	if(wrld_ptr != NULL && *wrld_ptr != NULL)
	{
		if((*wrld_ptr)->LevelData != NULL)
		{
			for(int i = 0; i < (*wrld_ptr)->LevelCount; ++i)
			{
				if((*wrld_ptr)->LevelData[i].TileData != NULL)
				{
					free((*wrld_ptr)->LevelData[i].TileData);
					(*wrld_ptr)->LevelData[i].TileData = NULL;
					
				}
				if((*wrld_ptr)->LevelData[i].EntityQueue != NULL)
				{
					DestroyEntityQueue(&(*wrld_ptr)->LevelData[i].EntityQueue);
					(*wrld_ptr)->LevelData[i].EntityQueue = NULL;
				}
			}
			free((*wrld_ptr)->LevelData);
			(*wrld_ptr)->LevelData = NULL;
		}
		free(*wrld_ptr);
	}
}

//Set the player entity associated with the world.
void SetPlayerEntity(struct sWorld_Data* wrld, struct sEntity_Data* plyr_ent)
{
	if(wrld != NULL && plyr_ent != NULL)
		wrld->Player = plyr_ent;
}

//Get a specified tile by vertex - integer or float (decimal ignored)
struct sTile_Data* GetTileByInt(struct sLevel_Data* lvl, struct Int2D coords)
{
	return &lvl->TileData[coords.Y * lvl->Size.X + coords.X];
}
struct sTile_Data* GetTileByFloat(struct sLevel_Data* lvl, struct Float2D coords)
{
	return &lvl->TileData[(int)coords.Y * lvl->Size.X + (int)ceil(coords.X)];
}