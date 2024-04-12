#pragma once

#include "Float2D.h"

//Constants for default view raycast settings.
#define VIEW_MAX 64
#define NUM_RAYS 257
#define FIELD_OF_VIEW 1.57079633

//Structure to hold all of the raw view data and settings.
struct sCamera
{
	struct Float2D Location; //Location of camera.
	double Look; //Angle of camera.
	double Fov; //Field of view (cone of the camera)
	unsigned int NumRays; //Number of rays to cast.
	double* Angles; //The pre-calculated angles at which to cast the rays. 
	double* Distances; //The perpindicular distances from the environment ahead.
	struct Float2D* Hits; //The wall hits of the rays.
};

//Creates camera object with specified field of view, specified number of rays at specified position, angle,
struct sCamera* CreateCamera(double, unsigned int, struct Float2D, double);
//Destroy camera at teardown.
void DestroyCamera(struct sCamera**); 

//Casts a ray.
double CastRay(double, double, struct Float2D, struct sLevel_Data*, struct Float2D*, char*);
//Does vision calculation for the first person view.
void CalcVision(struct sCamera*, struct sLevel_Data*);