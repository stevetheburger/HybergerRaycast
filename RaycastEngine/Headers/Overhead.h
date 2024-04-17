 #pragma once

#include "Int2D.h"
#include "Float2D.h"

//Dimensions for the grid as it should be displayed on the overhead view.
#define TILE_WIDTH_OVERHEAD 32
#define TILE_HEIGHT_OVERHEAD 32

//Draw functions for the overhead view.
void DrawOverheadView(struct SDL_Renderer*, struct sEntity_Data*, struct sCamera*, struct sLevel_Data*, struct sView*);
void DrawEntityOverhead(struct SDL_Renderer*, struct sEntity_Data*, struct sView*);
void DrawPlayerViewFan(struct SDL_Renderer*, struct sCamera*, struct sView*);
void DrawWorldOverhead(struct SDL_Renderer*, struct sLevel_Data*, struct sView*);

//Conversion methods for the overhead view.
struct Float2D WorldToViewCoord(struct Float2D, struct sView*);
struct Float2D ViewToWorldCoord(struct Float2D, struct sView*);