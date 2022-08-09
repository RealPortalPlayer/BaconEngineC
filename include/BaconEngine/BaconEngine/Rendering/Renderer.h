// Purpose: Helps with rendering.
// Created on: 4/3/22 @ 7:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Color.h"

CPP_GUARD_START()
    typedef enum {
        RENDERER_TYPE_AUTO,
        RENDERER_TYPE_OPENGL,
        RENDERER_TYPE_VULKAN,
        RENDERER_TYPE_TEXT
    } RendererTypes;

    void SetClearColor(Color3U color);
    void InitializeRenderer(void);
    void ClearScreen(void);
    RendererTypes GetCurrentRenderer(void);
    int IsSoftwareRendering(void);
    Color3U GetClearColor(void);
CPP_GUARD_END()
