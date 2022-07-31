// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"

CPP_GUARD_START()
    void InitializeWindow(const char* windowTitle, Vector2U windowSize);
    void DestroyWindow(void);
    SDL_Window* GetInternalSDLWindow(void);
    SDL_Renderer* GetInternalSDLRenderer(void);
    const char* GetWindowTitle(void);
    Vector2U GetWindowSize(void);
    Vector2U GetWindowPosition(void);
    int IsWindowVisible(void);
    int IsWindowStillOpened(void);
    void SetWindowTitle(const char* newTitle);
    void SetWindowSize(Vector2U newSize);
    void SetWidth(unsigned int newWidth);
    void SetWindowHeight(unsigned int newHeight);
    void SetWindowPosition(Vector2U newPosition);
    void SetWindowXPosition(unsigned int newX);
    void SetWindowYPosition(unsigned int newY);
    void SetWindowVisibility(int visible);
CPP_GUARD_END()