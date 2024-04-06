#include "Application.h"

#include <SDL.h>
#include <stdio.h>

#include "Overhead.h"


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
	app->window_size.X = window_w;
	app->window_size.Y = window_h;

	app->ovrhd.ViewSize.X = window_w / 2;
	app->ovrhd.ViewSize.Y = app->frstprsn.ViewSize.Y = window_h;

	app->ovrhd.ViewPos.X = app->ovrhd.ViewPos.Y = app->frstprsn.ViewPos.X = app->frstprsn.ViewPos.Y = 0;
	app->ovrhd.CameraPositionInWrld.X = app->ovrhd.CameraPositionInWrld.Y = app->frstprsn.CameraPositionInWrld.X = app->frstprsn.CameraPositionInWrld.Y = 0.0;

	return app;
}

//Handles cleanup for the application object.
void DestroyApplication(struct sApplication** app_ptr)
{
	if(app_ptr != NULL && *app_ptr != NULL)
	{
		if((*app_ptr)->render) SDL_DestroyRenderer((*app_ptr)->render);
		if((*app_ptr)->window) SDL_DestroyWindow((*app_ptr)->window);
		if((*app_ptr)->run_mtx) SDL_DestroyMutex((*app_ptr)->run_mtx);
		free(*app_ptr);
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

char GetOverheadState(struct sApplication* app)
{
	return app->ovrd_hd_active;
}
void SetOverheadState(struct sApplication* app, char new_state)
{
	if(new_state == 1)
	{
		app->frstprsn.ViewPos.X = app->window_size.X / 2;
		app->frstprsn.ViewSize.X = app->window_size.X / 2;
	}
	else if(new_state == 0)
	{
		app->frstprsn.ViewPos.X = 0;
		app->frstprsn.ViewSize.X = app->window_size.X;
	}
}
void SetOverheadCamera(struct sApplication* app, struct Float2D new_camera)
{
	app->ovrhd.CameraPositionInWrld = (struct Float2D){new_camera.X, new_camera.Y};
}
void SetOverheadSize(struct sApplication* app, struct Int2D new_size)
{
	app->ovrhd.ViewSize = (struct Int2D){new_size.X, new_size.Y};
}
void SetOverheadPos(struct sApplication* app, struct Int2D new_pos)
{
	app->ovrhd.ViewPos = (struct Int2D){new_pos.X, new_pos.Y};
}