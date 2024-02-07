#include <stdlib.h>
#include <stdio.h>
#include "Int2D.h"
#include "World.h"
#include "Entity.h"

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

struct sWorld_Data* CreateWorldHardcode()
{
	struct sEntity_Data* plyr_data = NULL;

	struct sWorld_Data* wrld_data = malloc(sizeof(struct sWorld_Data));
	if(wrld_data == NULL)
		return NULL;

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
	while(count < LEVEL_DIM[0] * LEVEL_DIM[1])
	{
		wrld_data->LevelData[0].TileData[count].Type = LVL_1_TILE[count];
		if(LVL_1_ENT[count] == 1)
		{
			struct Float2D loc;
			loc.X = count % LEVEL_DIM[0] + 0.5;
			loc.Y = count / LEVEL_DIM[0] + 0.5;
			struct sEntity_Data* plyr_data = CreatePlayerEntity(loc, 0, 0, 1);
			if(plyr_data == NULL)
				goto BAD_CREATE;

			EnqueueEntity(wrld_data->LevelData[0].EntityQueue, plyr_data);
			wrld_data->Player = plyr_data;
		}
		++count;
	}

	return wrld_data;

	BAD_CREATE:
	if(plyr_data != NULL) free(plyr_data);
	DestroyWorld(wrld_data);
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
	
	entity_buffer = CreateDefaultEntity();
	if(entity_buffer == NULL)
		goto BAD_CREATE;

	unsigned char count = 0;
	while(count < wrld_data->LevelCount)
	{
		if(fread(cpy_buffer, sizeof(struct Int2D), 1, file_ptr) != 1)
			goto BAD_CREATE;

		wrld_data->LevelData[count].Size = *(struct Int2D *)cpy_buffer;
		wrld_data->LevelData[count].TileData = malloc(sizeof(struct sTile_Data) * iToSize(wrld_data->LevelData[count].Size));
		if(wrld_data->LevelData[count].TileData == NULL)
			goto BAD_CREATE;

		wrld_data->LevelData[count].EntityQueue = CreateEntityQueue();
		if(wrld_data->LevelData[count].EntityQueue == NULL)
			goto BAD_CREATE;

		if(fread(cpy_buffer, sizeof(int), 1, file_ptr) != 1)
			goto BAD_CREATE;

		if(fread(wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), iToSize(wrld_data->LevelData[count].Size), file_ptr) != iToSize(wrld_data->LevelData[count].Size))
			goto BAD_CREATE;

		for(int i = 0; i < *(int *)cpy_buffer; ++i)
		{
			if(entity_buffer == NULL)
			{
				free(entity_buffer);
				goto BAD_CREATE;
			}

			if(fread(entity_buffer, sizeof(struct sEntity_Data), 1, file_ptr) != 1)
				goto BAD_CREATE;
			


			if(entity_buffer->Controller.IsPlayer) 
			{	
				wrld_data->Player = EntityDeepCopy(entity_buffer);
				EnqueueEntity(wrld_data->LevelData[count].EntityQueue, wrld_data->Player);
			}
			else
			{
				EnqueueEntity(wrld_data->LevelData[count].EntityQueue, EntityDeepCopy(entity_buffer));
			}
		}

		++count;
	}

	free(entity_buffer);
	free(cpy_buffer);
	fclose(file_ptr);

	return wrld_data;

	//Goto in case of error on creation.
	BAD_CREATE:
	if(cpy_buffer != NULL) free(cpy_buffer);
	if(file_ptr != NULL) fclose(file_ptr);
	if(wrld_data != NULL) DestroyWorld(wrld_data);
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
	//printf("Num lvls: %i\n", wrld_data->LevelCount);
	unsigned char count = 0;

	//Loop through levels and write the levels.
	while(count < wrld_data->LevelCount)
	{
		//printf("Lvl %i\n", count);

		//Get level size and do a null and sanity check.
		int lvl_size = iToSize(wrld_data->LevelData[count].Size);
		if(wrld_data->LevelData[count].TileData == NULL || lvl_size <= 0)
		{
			fclose(file_ptr);
			remove(file_path);
			DestroyEntityQueue(shunt_yard);
			return;
		}	

		//Write level header.
		fwrite(&wrld_data->LevelData[count].Size, sizeof(struct Int2D), 1, file_ptr);
		//printf("Lvl Size: %i, %i\n", wrld_data->LevelData[count].Size.X, wrld_data->LevelData[count].Size.Y);
		fwrite(&wrld_data->LevelData[count].EntityQueue->Count, sizeof(int), 1, file_ptr);
		//printf("Num Entities: %i\n", wrld_data->LevelData[count].EntityQueue->Count);
		fwrite(wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), lvl_size, file_ptr);
		//printf("Tile Data:\n");
		//for(int i = 0; i < wrld_data->LevelData[count].Size.Y; ++i)
		//{
		//	for(int e = 0; e < wrld_data->LevelData[count].Size.X; ++e)
		//	{
		//		printf(" %i ", wrld_data->LevelData[count].TileData[i * wrld_data->LevelData[count].Size.X + e].Type);
		//	}
		//	printf("\n");
		//}

		//Write the entities one by one and then shunt each off to a temporary queue that lasts the duration of the save operation.
		struct sEntity_Data* current = DequeueEntity(wrld_data->LevelData[count].EntityQueue);
		do
		{
			fwrite(current, sizeof(struct sEntity_Data), 1, file_ptr);
	
			//printf("Entity:\n");
			//printf(" type: %i\n", current->Type);
			//printf(" lvl: %i\n", current->Lvl);
			//printf(" loc: %f, %f\n", current->Location.X, current->Location.Y);
			//printf(" dir: %f\n", current->Look);
			//printf(" vel: %f, %f\n", current->Velocity.X, current->Velocity.Y);
			//printf(" player?: %i\n", current->Controller.IsPlayer);
			//printf(" Controls: \n");
			//printf(" w: %i, a: %i, s: %i, d: %i, q: %i, e: %i\n", current->Controller.w_down, current->Controller.a_down, current->Controller.s_down, current->Controller.d_down, current->Controller.q_down, current->Controller.e_down);	

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

void DestroyWorld(struct sWorld_Data* wrld)
{
	if(wrld != NULL)
	{
		if(wrld->LevelData != NULL)
		{
			for(int i = 0; i < wrld->LevelCount; ++i)
			{
				if(wrld->LevelData[i].TileData != NULL)
				{
					free(wrld->LevelData[i].TileData);
					wrld->LevelData[i].TileData = NULL;
					
				}
				if(wrld->LevelData[i].EntityQueue != NULL)
				{
					DestroyEntityQueue(wrld->LevelData[i].EntityQueue);
					wrld->LevelData[i].EntityQueue = NULL;
				}
			}
			free(wrld->LevelData);
			wrld->LevelData = NULL;
		}
		free(wrld);
	}
}

void SetPlayerEntity(struct sWorld_Data* wrld, struct sEntity_Data* plyr_ent)
{
	if(wrld != NULL && plyr_ent != NULL)
		wrld->Player = plyr_ent;
}