// Purpose: Helps with rendering.
// Created on: 4/3/22 @ 7:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/SDLSupport.h"

CPP_GUARD_START()
    typedef enum {
        BE_RENDERER_TYPE_AUTO,
        BE_RENDERER_TYPE_OPENGL,
        BE_RENDERER_TYPE_VULKAN,
        BE_RENDERER_TYPE_TEXT
    } BE_RendererTypes;

    void BE_SetClearColor(BE_Color3U color);
    void BE_InitializeRenderer(void);
    int BE_ClearScreen(void);
    int BE_GetRendererCalls(void);
    BE_RendererTypes BE_GetCurrentRenderer(void);
    int BE_IsSoftwareRendering(void);
    BE_Color3U BE_GetClearColor(void);
    BE_Vector2U BE_GetRendererFontSize(TTF_Font* font, const char* text);
    BE_Vector2I BE_GetCenterPosition(BE_Vector2I rectanglePosition, BE_Vector2U rectangleSize, BE_Vector2U objectSize);
    int BE_RendererDrawLine(BE_Vector2I firstPoint, BE_Vector2I secondPoint, BE_Color4U color);
    int BE_RendererDrawPoint(BE_Vector2I position, BE_Color4U color);
    int BE_RendererDrawRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U color);
    int BE_RendererFillRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U color);
    int BE_RendererDrawBorderedRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U borderColor, BE_Color4U fillColor, int borderPadding);
CPP_GUARD_END()

#define BE_RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(position, size, color, borderSize) RendererDrawFilledRectangle((position), (size), (color), (color), (borderSize))
