// Purpose: Custom and built-in UI elements.
// Created on: 8/11/2022 @ 2:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <SDL.h>

#include "BaconEngine/Math/Vector.h"

CPP_GUARD_START()
    typedef struct {
        Vector2I position;
        Vector2U size;
    } UIElement;

    typedef struct {
        void (*UIElementOnRender)(double deltaTime);
        void (*UIElementOnEvent)(SDL_Event event);
    } UIElementFunctions;

    void CreateElement(Vector2I position, Vector2U size, UIElementFunctions functions);
CPP_GUARD_END()
