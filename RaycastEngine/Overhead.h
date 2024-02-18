#pragma once
#include <SDL.h>
#include "Application.h"
#include "Int2D.h"
#include "Float2D.h"

void DrawOverheadView(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);
void DrawEntityOverhead(struct SDL_Renderer*, struct sEntity_Data*, struct sView*);
void DrawPlayerViewLine(struct SDL_Renderer*, struct sEntity_Data*, struct sWorld_Data**, struct sView*);

void DrawWorldOverhead(struct SDL_Renderer*, struct sWorld_Data**, struct sView*);
struct Float2D WorldToViewCoord(struct Float2D, struct sView*);
struct Float2D ViewToWorldCoord(struct Float2D, struct sView*);