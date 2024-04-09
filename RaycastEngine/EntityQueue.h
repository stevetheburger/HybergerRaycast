#pragma once

//A wrapper for the entity objects inside of the entity queue.
struct sEntity_Wrapper
{
	struct sEntity_Wrapper* Next; //Pointer to next wrapper object.
	struct sEntity_Data* Value; //The entity object contained therein.
};

//The struct for the queue.
struct sEntity_Queue
{
	struct sEntity_Wrapper* Front,* Back; //Pointers for the front and back of the queue.
	unsigned int Count; //Number of items in the queue.
};

//Creates a queue for entities present in a level.
struct sEntity_Queue* CreateEntityQueue();
//Destroys a queue for entities present in the level at teardown.
void DestroyEntityQueue(struct sEntity_Queue**);

//Pushes new entity onto queue at the back.
void EnqueueEntity(struct sEntity_Queue*, struct sEntity_Data*);
//Removes entity from the queue at the front.
struct sEntity_Data* DequeueEntity(struct sEntity_Queue*);
