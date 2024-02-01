#include <stdlib.h>
#include <stdio.h>
#include "Int2D.h"
#include "World.h"

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
	struct sWorld_Data* wrld_data = malloc(sizeof(struct sWorld_Data));
	if(wrld_data == NULL)
		return NULL;

	struct sEntity_Data* plyr_data = malloc(sizeof(struct sEntity_Data));
	if(plyr_data == NULL)
		goto BAD_CREATE;	

	wrld_data->LevelData = malloc(sizeof(struct sLevel_Data) * NUM_LVL);
	if(wrld_data->LevelData == NULL)
		goto BAD_CREATE;
	
	wrld_data->LevelCount = NUM_LVL;
	wrld_data->LevelData[0].TileData = malloc(sizeof(struct sTile_Data) * LEVEL_DIM[0] * LEVEL_DIM[1]);
	if(wrld_data->LevelData[0].TileData == NULL)
		goto BAD_CREATE;
	wrld_data->LevelData[0].EntityCount = 0;
	wrld_data->LevelData[0].Size.X = LEVEL_DIM[0];
	wrld_data->LevelData[0].Size.Y = LEVEL_DIM[1];
	unsigned int count = 0;
	char player_set = 0;
	while(count < LEVEL_DIM[0] * LEVEL_DIM[1])
	{
		wrld_data->LevelData[0].TileData[count].Type = LVL_1_TILE[count];
		if(LVL_1_ENT[count] == 1)
		{
			wrld_data->LevelData[0].EntityData = plyr_data;
			plyr_data->Lvl = &wrld_data->LevelData[0];
			wrld_data->Player = plyr_data;
			plyr_data->Location.X = count % LEVEL_DIM[0] + 0.5;
			plyr_data->Location.Y = count / LEVEL_DIM[0] + 0.5;
			plyr_data->Direction.X = 0;
			plyr_data->Direction.Y = -1;
			plyr_data->Type = 1;
			++wrld_data->LevelData[0].EntityCount;
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
	FILE* file_ptr;

	//Open file and make sure it exists.
	fopen_s(&file_ptr, file_path, "r");
	if(file_ptr == NULL)
		return NULL;

	//Create buffer for the dimensions of the world.
	void *cpy_buffer = malloc(sizeof(struct Int2D));
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
	
	unsigned char count = 0;
	while(count < wrld_data->LevelCount)
	{
		if(fread(cpy_buffer, sizeof(struct Int2D), 1, file_ptr) != sizeof(struct Int2D))
			goto BAD_CREATE;

		wrld_data->LevelData[count].Size = *(struct Int2D *)cpy_buffer;
		wrld_data->LevelData[count].TileData = malloc(sizeof(struct sTile_Data) * iToSize(wrld_data->LevelData[count].Size));
		if(wrld_data->LevelData[count].TileData == NULL)
			goto BAD_CREATE;

		if(fread(cpy_buffer, sizeof(int), 1, file_ptr) != sizeof(int))
			goto BAD_CREATE;

		wrld_data->LevelData[count].EntityCount = *(int *)cpy_buffer;
		wrld_data->LevelData[count].EntityData = malloc(sizeof(struct sEntity_Data) * wrld_data->LevelData[count].EntityCount);
		if(wrld_data->LevelData[count].EntityData == NULL)
			goto BAD_CREATE;

		fread(wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), iToSize(wrld_data->LevelData[count].Size), file_ptr);
		fread(wrld_data->LevelData[count].EntityData, sizeof(struct sEntity_Data), wrld_data->LevelData[count].EntityCount, file_ptr);

		++count;
	}

	free(cpy_buffer);
	fclose(file_ptr);

	return wrld_data;

	//Goto in case of error on creation.
	BAD_CREATE:
	if(cpy_buffer != NULL) free(cpy_buffer);
	if(file_ptr != NULL) fclose(file_ptr);
	if(wrld_data != NULL) DestroyWorld(wrld_data);
	return NULL;
}

void WorldToFile(struct sWorld_Data* wrld_data, const char* file_path)
{
	FILE* file_ptr = NULL;

	if(wrld_data == NULL)
		return;

	if(wrld_data->LevelData == NULL || wrld_data->LevelCount <= 0)
		return;

	fopen_s(&file_ptr, file_path, "w");
	if(file_ptr == NULL)
		return;


	fwrite(&wrld_data->LevelCount, 1, 1, file_ptr);
	unsigned char count = 0;
	while(count < wrld_data->LevelCount)
	{
		int lvl_size = iToSize(wrld_data->LevelData[count].Size);
		if(wrld_data->LevelData[count].TileData == NULL || lvl_size <= 0);
		{
			fclose(file_ptr);
			remove(file_path);
		}	

		fwrite(&wrld_data->LevelData[count].Size, sizeof(struct Int2D), 1, file_ptr);
		fwrite(&wrld_data->LevelData[count].EntityCount, sizeof(int), 1, file_ptr);
		fwrite(&wrld_data->LevelData[count].TileData, sizeof(struct sTile_Data), lvl_size, file_ptr);
		fwrite(&wrld_data->LevelData[count].EntityData, sizeof(struct sEntity_Data), wrld_data->LevelData[count].EntityCount, file_ptr);
		++count;
	}

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
				if(wrld->LevelData[i].EntityData != NULL)
				{
					free(wrld->LevelData[i].EntityData);
					wrld->LevelData[i].EntityData = NULL;
				}
			}
			free(wrld->LevelData);
			wrld->LevelData = NULL;
		}
		free(wrld);
	}
}

//void Init()
//{
//
//}
//void Enter()
//{
//	foreach(KeyValuePair<int, ChunkDataEntry> entry in CData)
//		entry.Value.Enter();
//}
//
//void Reset()
//{
//	foreach(KeyValuePair<int, ChunkDataEntry> entry in CData)
//		entry.Value.Reset();
//}
//
//void Update()
//{
//	foreach(KeyValuePair<int, ChunkDataEntry> entry in CData)
//		entry.Value.Update();
//}
//
//void Exit()
//{
//	foreach(KeyValuePair<int, ChunkDataEntry> entry in CData)
//		entry.Value.Exit();
//
//	CData.Clear();
//}