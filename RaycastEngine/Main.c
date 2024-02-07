#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>
#include "Application.h"
#include "World.h"
#include "Entity.h"

//Window & hardware constants.
#define WRLD_FILE ".\\test"
#define WINDOW_TITLE "Raycast Test"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

struct sApplication* _app;
struct sWorld_Data* _wrld;
struct sLevel_Data* _current_lvl;
struct sEntity_Data* _plyr_ent;
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
			DoInput(&_plyr_ent->Controller, &event);
	}
}

void update(double delta)
{
	//Move player.
	DoMove(_plyr_ent, delta);
}

void draw()
{
	struct SDL_Renderer* renderer = GetApplicationRenderer(_app);
	if(renderer != NULL)
	{
		//Set up variables.
		SDL_Rect rect;
		rect.w = TILE_WIDTH;
		rect.h = TILE_HEIGHT;
		rect.x = rect.y = 0;
		int count_x, count_y;
		count_x = count_y = 0;

		//Draw background
		SDL_SetRenderDrawColor(renderer, 0, 0, 127, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		while(count_y < _current_lvl->Size.Y)
		{
			rect.y = TILE_HEIGHT * count_y;
			while(count_x < _current_lvl->Size.X)
			{
				rect.x = TILE_WIDTH * count_x;

				if(_current_lvl->TileData[count_y * _current_lvl->Size.X + count_x].Type == 0)
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				else if(_current_lvl->TileData[count_y * _current_lvl->Size.X + count_x].Type == 1)
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

				SDL_RenderFillRect(renderer, &rect);

				++count_x;
			}
			rect.x = 0;
			count_x = 0;
			++count_y;
		}

		struct Int2D position, endpoint;
		position.X = _plyr_ent->Location.X * TILE_WIDTH;
		position.Y = _plyr_ent->Location.Y * TILE_HEIGHT;
		endpoint.X = position.X + cos(_plyr_ent->Look) * 100;
		endpoint.Y = position.Y + sin(_plyr_ent->Look) * 100;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, position.X, position.Y, endpoint.X, endpoint.Y);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		rect.w = PLAYER_WIDTH;
		rect.h = PLAYER_HEIGHT;
		rect.x = position.X - PLAYER_WIDTH / 2;
		rect.y = position.Y - PLAYER_HEIGHT / 2;
		SDL_RenderFillRect(renderer, &rect);

		//Show.
		SDL_RenderPresent(renderer);
	}
	else
		exit(EXIT_FAILURE);
}

//Garbage collection method for all dynamically allocated memory at close.
void dispose(void)
{
	DestroyApplication(_app);
	DestroyWorld(_wrld);
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

	_plyr_ent = _wrld->Player;
	if(_plyr_ent == NULL)
		exit(EXIT_FAILURE);
	if(_plyr_ent->Lvl >= _wrld->LevelCount)
		exit(EXIT_FAILURE);

	_current_lvl = &_wrld->LevelData[_plyr_ent->Lvl];

	if((_app = CreateApplication(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT)) == NULL)
		exit(EXIT_FAILURE);
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