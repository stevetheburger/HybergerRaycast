#include <stdio.h>
#include <SDL.h>

//Window & hardware constants.
#define WINDOW_TITLE "Raycast Test"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//Structure that contains global application info.
struct sApplication
{
	char run_state;
	struct SDL_Renderer* render;
	struct SDL_Window* window;
};

struct sApplication _app;

//Function to call in order to terminate program normally.
void stop()
{
	_app.run_state = 0;
}

void input()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			stop();
	}
}

void update()
{
	//Do game logic here.
}

void draw(struct SDL_Renderer* renderer)
{
	//Do draw.
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Show.
	SDL_RenderPresent(renderer);
}

//Garbage collection method for all dynamically allocated memory at close.
void dispose()
{
	if(_app.window != NULL) SDL_DestroyWindow(_app.window);
	if(_app.render != NULL) SDL_DestroyRenderer(_app.render);
}

void setup(struct sApplication app)
{
	//Set exit to call garbage collection before close.
	if(atexit(dispose) != 0)
		exit(1);

	//Initialize SDL library.
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		exit(1);
	
	//Allocate SDL window structure, using macro values defined at the top of this file.
	if((_app.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0)) == NULL)
		exit(1);

	//Allocate SDL renderer structure.
	if((_app.render = SDL_CreateRenderer(_app.window, -1, 0)) == NULL)
		exit(1);

	//Initialize application state.
	_app.run_state = 1;
}

//Entry point for program.
int main(int argc, char* argv[])
{
	//Do setup.
	setup(_app);

	//Game loop. Run as long as run_state is 1;
	while(_app.run_state)
	{
		input();
		update();
		draw(_app.render);
	}

	//Terminate program normally.
	exit(0);
	
	//In case exit fails erroneously somehow.
	return 1;
}