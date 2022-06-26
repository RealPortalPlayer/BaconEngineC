// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SDL.h>

#include "BaconEngine/Math/Vector.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitializeWindow(const char* windowTitle, Vector2U windowSize);
void DestroyWindow(void);
SDL_Window* GetSDLWindow(void);
SDL_Renderer* GetSDLRenderer(void);
const char* GetTitle(void);
Vector2U GetSize(void);
Vector2U GetPosition(void);
int IsWindowVisible(void);
int IsWindowStillOpened(void);
void SetTitle(const char* newTitle);
void SetSize(Vector2U newSize);
void SetWidth(unsigned int newWidth);
void SetHeight(unsigned int newHeight);
void SetPosition(Vector2U newPosition);
void SetX(unsigned int newX);
void SetY(unsigned int newY);
void SetWindowVisibility(int visible);

#ifdef __cplusplus
};
#endif