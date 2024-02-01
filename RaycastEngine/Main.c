#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include "Application.h"
#include "World.h"

//Window & hardware constants.
#define WRLD_FILE ".\\test"
#define WINDOW_TITLE "Raycast Test"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 4
#define PLAYER_FEET_PER_SEC 3
#define PLAYER_TURN_RATE_PER_SEC 2 * M_PI
#define FULL_CIRCLE 2 * M_PI

struct sPlayer_Controller
{
	struct Float2D velocity;
	char w_down, d_down, s_down, a_down, q_down, e_down;
	float look;
};

struct sApplication* _app;
struct sWorld_Data* _wrld;
struct sLevel_Data* _current_lvl;
struct sEntity_Data* _plyr_ent;
struct sPlayer_Controller* _cntrl;

//Function to call in order to terminate program normally.
void stop()
{
	//Shut down.
	SetRunState(_app, 0);
}

void input(float delta)
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		//Get key state if down.
		if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym == SDLK_w)
			{
				_cntrl->w_down = 1;
			}
			else if(event.key.keysym.sym == SDLK_d)
			{
				_cntrl->d_down = 1;
			}
			else if(event.key.keysym.sym == SDLK_s)
			{
				_cntrl->s_down = 1;
			}
			else if(event.key.keysym.sym == SDLK_a)
			{
				_cntrl->a_down = 1;
			}
			else if(event.key.keysym.sym == SDLK_q)
			{
				_cntrl->q_down = 1;
			}
			else if(event.key.keysym.sym == SDLK_e)
			{
				_cntrl->e_down = 1;
			}
		}
		//Get key state if up.
		else if(event.type == SDL_KEYUP)
		{
			if(event.key.keysym.sym == SDLK_w)
			{
				_cntrl->w_down = 0;
			}
			else if(event.key.keysym.sym == SDLK_d)
			{
				_cntrl->d_down = 0;
			}
			else if(event.key.keysym.sym == SDLK_s)
			{
				_cntrl->s_down = 0;
			}
			else if(event.key.keysym.sym == SDLK_a)
			{
				_cntrl->a_down = 0;
			}
			else if(event.key.keysym.sym == SDLK_q)
			{
				_cntrl->q_down = 0;
			}
			else if(event.key.keysym.sym == SDLK_e)
			{
				_cntrl->e_down = 0;
			}
		}
		////Detect mouse-x movement.
		//else if(event.type == SDL_MOUSEMOTION)
		//{
		//	printf("%i\n", event.motion.xrel);
		//	_cntrl->mouse_x += event.motion.xrel * delta;
		//	if(_cntrl->mouse_x >= 360.0f) _cntrl->mouse_x -= 360.0f;
		//	else if(_cntrl->mouse_x < 0.0f) _cntrl->mouse_x += 360.0f;
		//}
		//Test for quit.
		else if(event.type == SDL_QUIT)
			stop();
	}
}

void update(float delta)
{
	if(_cntrl->q_down)
	{
		_cntrl->look -= PLAYER_TURN_RATE_PER_SEC * delta;
		if(_cntrl->look >= FULL_CIRCLE)
			_cntrl->look -= FULL_CIRCLE;
		else if(_cntrl->look < 0)
			_cntrl->look += FULL_CIRCLE;
		_plyr_ent->Direction.X = cos(_cntrl->look);
		_plyr_ent->Direction.Y = sin(_cntrl->look);
	}
	if(_cntrl->e_down)
	{
		_cntrl->look += PLAYER_TURN_RATE_PER_SEC * delta;
		if(_cntrl->look >= FULL_CIRCLE)
			_cntrl->look -= FULL_CIRCLE;
		else if(_cntrl->look < 0)
			_cntrl->look += FULL_CIRCLE;
		_plyr_ent->Direction.X = cos(_cntrl->look);
		_plyr_ent->Direction.Y = sin(_cntrl->look);
	}
	//Calculate velocity from key state.
	if(_cntrl->w_down)
	{
		_cntrl->velocity.X += _plyr_ent->Direction.X;
		_cntrl->velocity.Y += _plyr_ent->Direction.Y;
	}
	if(_cntrl->s_down)
	{
		_cntrl->velocity.X += -_plyr_ent->Direction.X;
		_cntrl->velocity.Y += -_plyr_ent->Direction.Y;
	}
	if(_cntrl->d_down)
	{
		_cntrl->velocity.X += -_plyr_ent->Direction.Y;
		_cntrl->velocity.Y += _plyr_ent->Direction.X;
	}
	if(_cntrl->a_down)
	{
		_cntrl->velocity.X += _plyr_ent->Direction.Y;
		_cntrl->velocity.Y += -_plyr_ent->Direction.X;
	}

	//Normalize and apply.
	if(_cntrl->velocity.X != 0 || _cntrl->velocity.Y != 0) 
	{	
		float magnitude = sqrtf((_cntrl->velocity.X) * (_cntrl->velocity.X) + (_cntrl->velocity.Y) * (_cntrl->velocity.Y));
		_cntrl->velocity.X /= magnitude;
		_cntrl->velocity.Y /= magnitude;
		_cntrl->velocity.X *= PLAYER_FEET_PER_SEC * delta;
		_cntrl->velocity.Y *= PLAYER_FEET_PER_SEC * delta;

		_plyr_ent->Location = fAdd(_plyr_ent->Location, _cntrl->velocity);
	}
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
		unsigned int count_x, count_y;
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
		endpoint.X = position.X + cos(_cntrl->look) * 100;
		endpoint.Y = position.Y + sin(_cntrl->look) * 100;
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
		exit(1);
}

//Garbage collection method for all dynamically allocated memory at close.
void dispose()
{
	if(_cntrl != NULL) free(_cntrl);
	DestroyApplication(_app);
	DestroyWorld(_wrld);
}

void setup()
{
	//Set exit to call garbage collection before close.
	if(atexit(dispose) != 0)
		exit(1);

	//Initialize SDL library.
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		exit(1);
	
	_wrld = CreateWorldFromFile(WRLD_FILE);
	if(_wrld == NULL)
	{
		_wrld = CreateWorldHardcode();

		WorldToFile(_wrld, WRLD_FILE);
	}

	_plyr_ent = _wrld->Player;
	_current_lvl = _plyr_ent->Lvl;
	_cntrl = malloc(sizeof(struct sPlayer_Controller));
	if(_cntrl == NULL)
		exit(1);

	_cntrl->w_down = _cntrl->d_down = _cntrl->s_down = _cntrl->a_down = _cntrl->q_down = _cntrl->e_down = _cntrl->velocity.X = _cntrl->velocity.Y = _cntrl->look = 0;

	_plyr_ent->Direction.X = 1;
	_plyr_ent->Direction.Y = 0;

	if((_app = CreateApplication(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT)) == NULL)
		exit(1);
}

//Entry point for program.
int main(int argc, char* argv[])
{
	//Do setup.
	setup();

	clock_t last, current;
	float sec_elapsed = 0;
	current = 0;
	last = clock();

	//Game loop. Run as long as run_state is 1;
	while(_app->run_state)
	{
		current = clock();
		sec_elapsed = ((float)(current - last))/CLOCKS_PER_SEC;		
		last = current;

		input(sec_elapsed);
		update(sec_elapsed);
		draw();
	}

	//Terminate program normally.
	exit(0);
	
	//In case exit fails erroneously somehow.
	return 1;
}