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
	double draw_height = 0.0, x_draw_size = 0.0, remainder_left = 0.0, remainder_right = 0.0, correction_angle = 0.0;
	int columns_per_cast = view->ViewSize.X / cam->NumRays, x_draw_position_left = view->ViewPos.X + view->ViewSize.X / 2 - 1, x_draw_position_right = x_draw_position_left + 1, num_pixels = 0;

	//Loop through the rays.
	unsigned int count = 0;
	while(count < cam->NumRays)
	{
		//Left.
		//Calculate the line height for the current ray.
		draw_height = view->ViewSize.Y/cam->Distances[count++]; if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;

		//Calculate the number of pixel columns to draw in the current ray.
		x_draw_size = columns_per_cast + remainder_left;
		num_pixels = (int)x_draw_size; //Get number of complete pixels in order to draw.
		remainder_left = x_draw_size - num_pixels; //Get the decimal portion remaining to apply to the next increment.
		//Draw left.
		while(num_pixels>0)
		{
			SDL_RenderDrawLine(renderer, x_draw_position_left, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position_left, (int)(view->ViewSize.Y + draw_height) / 2);
 			--x_draw_position_left;
			--num_pixels;
		}

		//Right.
		//Calculate the line height for the current ray.
		draw_height = view->ViewSize.Y/(cam->Distances[count++]); if(draw_height>view->ViewSize.Y) draw_height=view->ViewSize.Y;

		//Calculate the number of pixel columns to draw in the current ray.
		x_draw_size = columns_per_cast + remainder_right;
		num_pixels = (int)x_draw_size; //Get number of complete pixels in order to draw.
		remainder_right = x_draw_size - num_pixels; //Get the decimal portion remaining to apply to the next increment.
		//Draw right.
		while(num_pixels>0)
		{
			SDL_RenderDrawLine(renderer, x_draw_position_right, (int)(view->ViewSize.Y - draw_height) / 2, x_draw_position_right, (int)(view->ViewSize.Y + draw_height) / 2);
			++x_draw_position_right;
			--num_pixels;
		}
	}
}