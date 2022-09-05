// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/SDLSupport.h"

SEC_CPP_GUARD_START()
    void BE_Window_Initialize(const char* windowTitle, BE_Vector_2U windowSize);
    void BE_Window_Destroy(void);
    SDL_Window* BE_Window_GetInternalSDLWindow(void);
    SDL_Renderer* BE_Window_GetInternalSDLRenderer(void);
    const char* BE_Window_GetTitle(void);
    BE_Vector_2U BE_Window_GetSize(void);
    BE_Vector_2U BE_Window_GetPosition(void);
    int BE_Window_IsVisible(void);
    int BE_Window_IsStillOpened(void);
    void BE_Window_SetTitle(const char* newTitle);
    void BE_Window_SetSize(BE_Vector_2U newSize);
    void BE_Window_SetWidth(unsigned newWidth);
    void BE_Window_SetHeight(unsigned newHeight);
    void BE_Window_SetPosition(BE_Vector_2U newPosition);
    void BE_Window_SetXPosition(unsigned int newX);
    void BE_Window_SetYPosition(unsigned int newY);
    void BE_Window_SetVisibility(int visible);
SEC_CPP_GUARD_END()
