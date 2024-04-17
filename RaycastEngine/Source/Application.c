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
		goto BAD_ALLOC;
	//Allocate SDL window structure, using macro values defined at the top of this file.
	if((app->window = SDL_CreateWindow(window_title, window_x, window_y, window_w, window_h, 0)) == NULL)
		goto BAD_ALLOC;
	//Allocate SDL renderer structure.
	if((app->render = SDL_CreateRenderer(app->window, -1, 0)) == NULL)
		goto BAD_ALLOC;
	//Allocate SDL mutex that blocks for the application status.
	if((app->run_mtx = SDL_CreateMutex()) == NULL)
		goto BAD_ALLOC;

	//Initialize application state.
	app->run_state = 1;
	app->window_size.X = window_w;
	app->window_size.Y = window_h;

	app->ovrhd.ViewSize.X = window_w / 2;
	app->ovrhd.ViewSize.Y = app->frstprsn.ViewSize.Y = window_h;

	app->ovrhd.ViewPos.X = app->ovrhd.ViewPos.Y = app->frstprsn.ViewPos.X = app->frstprsn.ViewPos.Y = 0;
	app->ovrhd.CameraPositionInWrld.X = app->ovrhd.CameraPositionInWrld.Y = app->frstprsn.CameraPositionInWrld.X = app->frstprsn.CameraPositionInWrld.Y = 0.0;

	return app;

	BAD_ALLOC:
	DestroyApplication(&app);	
	return NULL;
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

//Getters for SDL objects.
struct SDL_Renderer* GetApplicationRenderer(struct sApplication* app) { return app != NULL ? app->render : NULL; }
struct SDL_Window* GetApplicationWindow(struct sApplication* app) { return app != NULL ? app->window : NULL; }

//Thread safe getter for the application run state.
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
//Thread safe setter for the application run state.
void SetRunState(struct sApplication* app, char value)
{
	if(app == NULL || app->run_mtx == NULL)
		return;

	SDL_LockMutex(app->run_mtx);
	app->run_state = value;
	SDL_UnlockMutex(app->run_mtx);
}

//Getter for the overhead display on/off state.
char GetOverheadState(struct sApplication* app)
{
	return app->ovrd_hd_active;
}
//Setter for the overhead display on/off state.
void SetOverheadState(struct sApplication* app, char new_state)
{
	if(new_state == 1)
	{
		//If overhead on, move and scale first person view over to display debug overhead on the left.
		app->frstprsn.ViewPos.X = app->window_size.X / 2;
		app->frstprsn.ViewSize.X = app->window_size.X / 2;
	}
	else if(new_state == 0)
	{
		//If overhead off, have first person view take up the entire view.
		app->frstprsn.ViewPos.X = 0;
		app->frstprsn.ViewSize.X = app->window_size.X;
	}
}

//Setters for the dimensions of the overhead view for the application.
void SetOverheadSize(struct sApplication* app, struct Int2D new_size)
{
	app->ovrhd.ViewSize = (struct Int2D){new_size.X, new_size.Y};
}
void SetOverheadPos(struct sApplication* app, struct Int2D new_pos)
{
	app->ovrhd.ViewPos = (struct Int2D){new_pos.X, new_pos.Y};
}