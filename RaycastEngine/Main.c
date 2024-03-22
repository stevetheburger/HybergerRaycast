#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include "Application.h"
#include "World.h"
#include "Entity.h"
#include "Overhead.h"
#include "Viewport.h"

//Window & hardware constants.
#define WRLD_FILE ".\\test"
#define WINDOW_TITLE "Raycast Test"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

struct sApplication* _app;
struct sWorld_Data* _wrld;
//struct sEntity_Data* _plyr_ent;
//struct sPlayer_Controller* _cntrl;

//Function to call in order to terminate program normally.
void stop()
{
	//Shut down.
	SetRunState(_app, 0);
}

void input()
{
	static SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		//Test for quit.
		if(event.type == SDL_QUIT)
			stop();
		//Otherwise, pass events through to player object.
		else
			DoInput(&_wrld->Player->Controller, &event);
	}
}

void update(double delta)
{
	//Move player.
	DoMove(_wrld->Player, delta);
}

void draw()
{
	struct SDL_Renderer* renderer = GetApplicationRenderer(_app);
	if(renderer != NULL)
	{

		if(_app->ovrd_hd_active) 
		{
			_app->ovrhd.CameraPositionInWrld.X = _wrld->Player->Location.X;
			_app->ovrhd.CameraPositionInWrld.Y = _wrld->Player->Location.Y;
			DrawOverheadView(renderer, &_wrld, &_app->ovrhd);
		}

		_app->frstprsn.CameraPositionInWrld.X = _wrld->Player->Location.X;
		_app->frstprsn.CameraPositionInWrld.Y = _wrld->Player->Location.Y;
		DrawFirstPersonView(renderer, &_wrld, &_app->frstprsn);

		//Show.
		SDL_RenderPresent(renderer);
	}
	else
		exit(EXIT_FAILURE);
}

//Garbage collection method for all dynamically allocated memory at close.
void dispose(void)
{
	DestroyApplication(&_app);
	DestroyWorld(&_wrld);
}

void setup()
{
	//Set exit to call garbage collection before close.
	if(atexit(dispose) != 0)
		exit(EXIT_FAILURE);

	//Initialize SDL library.
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		exit(EXIT_FAILURE);
	
	_wrld = CreateWorldFromFile(WRLD_FILE);
	if(_wrld == NULL)
	{
		_wrld = CreateWorldHardcode();

		WorldToFile(_wrld, WRLD_FILE);
	}

	//_plyr_ent = _wrld->Player;
	//if(_plyr_ent == NULL)
	//	exit(EXIT_FAILURE);
	//if(_plyr_ent->Lvl >= _wrld->LevelCount)
	//	exit(EXIT_FAILURE);

	if((_app = CreateApplication(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT)) == NULL)
		exit(EXIT_FAILURE);

	SetOverheadState(_app, 0);
}

//Entry point for program.
int main(int argc, char* argv[])
{
	//Do setup.
	setup();

	clock_t last, current;
	current = 0;
	last = clock();

	//Game loop. Run as long as run_state is 1;
	while(_app->run_state)
	{
		current = clock();	

		input();
		update(((double)current - last)/CLOCKS_PER_SEC);
		draw();

		last = current;
	}

	//Terminate program normally.
	exit(EXIT_SUCCESS);
	
	//In case exit fails erroneously somehow.
	return 1;
}