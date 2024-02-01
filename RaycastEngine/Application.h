#pragma once

//Structure that contains global application info.
struct sApplication
{
	char run_state;
	struct SDL_mutex *run_mtx;
	struct SDL_Renderer* render;
	struct SDL_Window* window;
};

//Create and cleanup methods to handle the initialization and memory of the application structure.
struct sApplication* CreateApplication(const char *, int, int, int, int);
void DestroyApplication(struct sApplication*);

//Getters for the subcomponents of the application structure. These are not assumed to be thread safe but should only be accessed by the main thread used for rendering.
struct SDL_Renderer* GetApplicationRenderer(struct sApplication*);
struct SDL_Window* GetApplicationWindow(struct sApplication*);

//No setters for the SDL objects because the renderer and window are set internally.

//Thread safe getters and setters for the application status.
char GetRunState(struct sApplication* app);
void SetRunState(struct sApplication* app, char);