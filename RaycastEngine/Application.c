#include <stdio.h>
#include <SDL.h>
#include "Application.h"

//An atomic constructor that returns an application object with the provided title, dimensions, and position.
struct sApplication* CreateApplication(const char* window_title, int window_x, int window_y, int window_w, int window_h)
{
	//Allocate the application structure.
	struct sApplication* app = NULL;
	if((app = malloc(sizeof(struct sApplication))) == NULL)
		return NULL;
	//Allocate SDL window structure, using macro values defined at the top of this file.
	if((app->window = SDL_CreateWindow(window_title, window_x, window_y, window_w, window_h, 0)) == NULL)
	{
		free(app);
		return NULL;
	}
	//Allocate SDL renderer structure.
	if((app->render = SDL_CreateRenderer(app->window, -1, 0)) == NULL)
	{
		SDL_DestroyWindow(app->window);
		free(app);
		return NULL;
	}
	//Allocate SDL mutex that blocks for the application status.
	if((app->run_mtx = SDL_CreateMutex()) == NULL)
	{
		SDL_DestroyWindow(app->window);
		SDL_DestroyRenderer(app->render);
		free(app);

		return NULL;
	}

	//Initialize application state.
	app->run_state = 1;

	return app;
}

//Handles cleanup for the application object.
void DestroyApplication(struct sApplication* app)
{
	if(app != NULL)
	{
		if(app->render) SDL_DestroyRenderer(app->render);
		if(app->window) SDL_DestroyWindow(app->window);
		if(app->run_mtx) SDL_DestroyMutex(app->run_mtx);
		free(app);
	}
}

struct SDL_Renderer* GetApplicationRenderer(struct sApplication* app) { return app != NULL ? app->render : NULL; }
struct SDL_Window* GetApplicationWindow(struct sApplication* app) { return app != NULL ? app->window : NULL; }

char GetRunState(struct sApplication* app)
{
	char value = -1;

	if(app == NULL || app->run_mtx == NULL)
		return -1;
	
	SDL_LockMutex(app->run_mtx);
	value = app->run_state;
	SDL_UnlockMutex(app->run_mtx);

	return value;
}

void SetRunState(struct sApplication* app, char value)
{
	if(app == NULL || app->run_mtx == NULL)
		return;

	SDL_LockMutex(app->run_mtx);
	app->run_state = value;
	SDL_UnlockMutex(app->run_mtx);
}