// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/SDLSupport.h"

CPP_GUARD_START()
    void BE_InitializeWindow(const char* windowTitle, BE_Vector2U windowSize);
    void BE_DestroyWindow(void);
    SDL_Window* BE_GetInternalSDLWindow(void);
    SDL_Renderer* BE_GetInternalSDLRenderer(void);
    const char* BE_GetWindowTitle(void);
    BE_Vector2U BE_GetWindowSize(void);
    BE_Vector2U BE_GetWindowPosition(void);
    int BE_IsWindowVisible(void);
    int BE_IsWindowStillOpened(void);
    void BE_SetWindowTitle(const char* newTitle);
    void BE_SetWindowSize(BE_Vector2U newSize);
    void BE_SetWindowWidth(unsigned newWidth);
    void BE_SetWindowHeight(unsigned newHeight);
    void BE_SetWindowPosition(BE_Vector2U newPosition);
    void BE_SetWindowXPosition(unsigned int newX);
    void BE_SetWindowYPosition(unsigned int newY);
    void BE_SetWindowVisibility(int visible);
CPP_GUARD_END()
