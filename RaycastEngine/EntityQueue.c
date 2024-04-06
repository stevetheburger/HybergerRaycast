#include "EntityQueue.h"

#include <stdlib.h>

#include "Entity.h"

struct sEntity_Queue* CreateEntityQueue()
{
	struct sEntity_Queue* ret_val = malloc(sizeof(struct sEntity_Queue));
	if(ret_val != NULL)
	{
		ret_val->Front = ret_val->Back = NULL;
		ret_val->Count = 0;
	}

	return ret_val;
}
void DestroyEntityQueue(struct sEntity_Queue** queue_ptr)
{
	if(queue_ptr != NULL && *queue_ptr != NULL)
	{
		struct sEntity_Data* current = NULL;
		do
		{
			current = DequeueEntity(*queue_ptr);
				DestroyEntity(&current);
		} 
		while(current != NULL);
		free(*queue_ptr);
	}
}

void EnqueueEntity(struct sEntity_Queue* queue, struct sEntity_Data* ent)
{
	if(queue != NULL && ent != NULL)
	{
		struct sEntity_Wrapper* wrapper = malloc(sizeof(struct sEntity_Wrapper));
		if(wrapper == NULL)
			return;

		wrapper->Next = NULL;
		wrapper->Value = ent;

		if(queue->Back != NULL)
		{
			queue->Back->Next = wrapper;
			queue->Back = wrapper;
		}
		else
		{
			queue->Front = queue->Back = wrapper;
		}
		++queue->Count;
	}
}
struct sEntity_Data* DequeueEntity(struct sEntity_Queue* queue)
{
	struct sEntity_Data* ret_val = NULL;
	struct sEntity_Wrapper* wrapper = NULL;

	if(queue != NULL)
	{
		if(queue->Front != NULL)
		{
			wrapper = queue->Front;
			ret_val = queue->Front->Value;
			queue->Front = queue->Front->Next;
			free(wrapper);
			if(queue->Front == NULL)
				queue->Back = NULL;
		}
		--queue->Count;
	}

	return ret_val;
}