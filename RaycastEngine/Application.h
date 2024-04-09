#pragma once

#include "Int2D.h"
#include "Float2D.h"

//Structure that contains viewport dimensions.
struct sView 
{
	struct Int2D ViewPos;
	struct Int2D ViewSize;
	struct Float2D CameraPositionInWrld;
};

//Structure that contains global application info.
struct sApplication
{
	char run_state;
	char ovrd_hd_active;
	struct SDL_mutex *run_mtx;
	struct SDL_Renderer* render;
	struct SDL_Window* window;
	struct Int2D window_size;
	struct sView ovrhd; //Top down view for debug.
	struct sView frstprsn; //First person view.
};

//Create and cleanup methods to handle the initialization and memory of the application structure.
struct sApplication* CreateApplication(const char *, int, int, int, int);
void DestroyApplication(struct sApplication**);

//Getters for the subcomponents of the application structure. These are not assumed to be thread safe but should only be accessed by the main thread used for rendering.
struct SDL_Renderer* GetApplicationRenderer(struct sApplication*);
struct SDL_Window* GetApplicationWindow(struct sApplication*);

//No setters for the SDL objects because the renderer and window are set internally.
//Thread safe getters and setters for the application status.
char GetRunState(struct sApplication* app);
void SetRunState(struct sApplication* app, char);

//Getter and setter for whether or not the overhead view is displayed.
char GetOverheadState(struct sApplication* app);
void SetOverheadState(struct sApplication* app, char);

//Setters for the dimensions of the overhead display for the application.
void SetOverheadSize(struct sApplication* app, struct Int2D);
void SetOverheadPos(struct sApplication* app, struct Int2D);