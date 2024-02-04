#pragma once

#include "Entity.h"

struct sEntity_Wrapper
{
	struct sEntity_Wrapper* Next;
	struct sEntity_Data* Value;
};

struct sEntity_Queue
{
	struct sEntity_Wrapper* Front,* Back;
	int Count;
};

struct sEntity_Queue* CreateEntityQueue();
void DestroyEntityQueue(struct sEntity_Queue*);

void EnqueueEntity(struct sEntity_Queue*, struct sEntity_Data*);
struct sEntity_Data* DequeueEntity(struct sEntity_Queue*);
