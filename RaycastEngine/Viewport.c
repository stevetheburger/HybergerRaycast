#include "Viewport.h"

#include <SDL.h>
#include <stdio.h>

#include "TrigConstants.h"
#include "Application.h"
#include "Camera.h"
#include "World.h"
#include "Entity.h"

//Give the player vision.
void DrawFirstPersonView(struct SDL_Renderer* renderer, struct sCamera* cam, struct sView* view)
{
	//Draw sky.
	SDL_Rect rect;
	rect.x = view->ViewPos.X;
	rect.y = view->ViewPos.Y;
	rect.w = view->ViewSize.X;
	rect.h = view->ViewSize.Y;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);

	//Create variables.
	double draw_height = 0.0, x_draw_size = 0.0, remainder = 0.0;
	double columns_per_cast = (double)view->ViewSize.X / cam->NumRays, x_draw_position = view->ViewPos.X, num_pixels = 0;

	//Loop through the rays.
	for(unsigned int count = 0; count < cam->NumRays; ++count)
	{
		//Left.
		//Calculate the line height for the current ray.
		draw_height = view->ViewSize.Y/cam->Distances[count]; if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;

		//Calculate the number of pixel columns to draw in the current ray.
		x_draw_size = columns_per_cast + remainder;
		num_pixels = (int)x_draw_size; //Get number of complete pixels in order to draw.
		remainder = x_draw_size - num_pixels; //Get the decimal portion remaining to apply to the next increment.
		//Draw left.
		while(num_pixels>0)
		{
			SDL_RenderDrawLine(renderer, x_draw_position, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position, (int)(view->ViewSize.Y + draw_height) / 2);
 			++x_draw_position;
			--num_pixels;
		}
	}
}