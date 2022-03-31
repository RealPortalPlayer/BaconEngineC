// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SDL.h>

#include "BaconEngine/Math/Vector.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitializeWindow(const char* windowTitle, Vector2UI windowSize);
void DestroyWindow(void);
SDL_Window* GetSDLWindow(void);
const char* GetTitle(void);
Vector2UI GetSize(void);
Vector2UI GetPosition(void);
void SetTitle(const char* newTitle);
void SetSize(Vector2UI newSize);
void SetWidth(unsigned int newWidth);
void SetHeight(unsigned int newHeight);
void SetPosition(Vector2UI newPosition);
void SetX(unsigned int newX);
void SetY(unsigned int newY);

#ifdef __cplusplus
};
#endif