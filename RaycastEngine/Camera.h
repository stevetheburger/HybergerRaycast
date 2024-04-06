#pragma once

#include "Float2D.h"

#define VIEW_MAX 64
#define FIELD_OF_VIEW 1.57079633
#define FIELD_OF_VIEW_HALF 0.78539816
#define VIEW_CAST_INCREMENT 0.00872665

struct sCamera
{
	struct Float2D Location;
	double Look;
	double Fov;
	unsigned int NumRays;
	double* Angles;
	double* Distances;
	struct Float2D* Hits;
};

struct sCamera* CreateCamera(double, unsigned int, struct Float2D, double);
void DestroyCamera(struct sCamera**); 

double CastRay(double, struct Float2D, struct sLevel_Data*, struct Float2D*, char*);
void CalcVision(struct sCamera*, struct sLevel_Data*);