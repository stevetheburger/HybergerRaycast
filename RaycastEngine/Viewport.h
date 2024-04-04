#pragma once

#include <SDL.h>
#include <stdio.h>
#include "World.h"
#include "Entity.h"

#define VIEW_MAX 64
#define FIELD_OF_VIEW 1.57079633
#define FIELD_OF_VIEW_HALF 0.78539816
#define VIEW_CAST_INCREMENT 0.00872665
#define THREE_QUARTER_CIRCLE 4.71238898
#define QUARTER_CIRCLE 1.57079623
#define COLUMNS_FACTOR 0.00555556

void DrawFirstPersonView(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);
double CastRay(double, struct sEntity_Data*, struct sLevel_Data*, struct Float2D*, char*);