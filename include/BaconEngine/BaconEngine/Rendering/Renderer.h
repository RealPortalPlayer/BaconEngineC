// Purpose: Helps with rendering.
// Created on: 4/3/22 @ 7:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Math/Vector.h"

CPP_GUARD_START()
    typedef enum {
        RENDERER_TYPE_AUTO,
        RENDERER_TYPE_OPENGL,
        RENDERER_TYPE_VULKAN,
        RENDERER_TYPE_TEXT
    } RendererTypes;

    void SetClearColor(Color3U color);
    void InitializeRenderer(void);
    int ClearScreen(void);
    RendererTypes GetCurrentRenderer(void);
    int IsSoftwareRendering(void);
    Color3U GetClearColor(void);
    int RendererDrawLine(Vector2I firstPoint, Vector2I secondPoint, Color4U color);
    int RendererDrawPoint(Vector2I position, Color4U color);
    int RendererDrawRectangle(Vector2I position, Vector2U size, Color4U color);
    int RendererFillRectangle(Vector2I position, Vector2U size, Color4U color);
    int RendererDrawFilledRectangle(Vector2I position, Vector2U size, Color4U borderColor, Color4U fillColor, int borderSize);

#   define RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(position, size, color, borderSize) RendererDrawFilledRectangle((position), (size), (color), (color), (borderSize))
CPP_GUARD_END()
