// Purpose: Helps with rendering.
// Created on: 4/3/22 @ 7:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/SDLSupport.h"

SEC_CPP_GUARD_START()
    typedef enum {
        BE_RENDERER_TYPE_AUTO,
        BE_RENDERER_TYPE_OPENGL,
        BE_RENDERER_TYPE_VULKAN,
        BE_RENDERER_TYPE_TEXT
    } BE_Renderer_Types;

    void BE_Renderer_SetClearColor(BE_Color_3U color);
    void BE_Renderer_Initialize(void);
    int BE_Renderer_ClearScreen(void);
    int BE_Renderer_GetCalls(void);
    BE_Renderer_Types BE_Renderer_GetCurrentType(void);
    int BE_Renderer_IsSoftwareMode(void);
    BE_Color_3U BE_Renderer_GetClearColor(void);
    BE_Vector_2U BE_Renderer_GetFontSize(TTF_Font* font, const char* text);
    BE_Vector_2I BE_Renderer_GetCenterPosition(BE_Vector_2I rectanglePosition, BE_Vector_2U rectangleSize, BE_Vector_2U objectSize);
    int BE_Renderer_DrawLine(BE_Vector_2I firstPoint, BE_Vector_2I secondPoint, BE_Color_4U color);
    int BE_Renderer_DrawPoint(BE_Vector_2I position, BE_Color_4U color);
    int BE_Renderer_DrawRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
    int BE_Renderer_FillRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
    int BE_Renderer_DrawBorderedRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U borderColor, BE_Color_4U fillColor, int borderPadding);
SEC_CPP_GUARD_END()

#define BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, size, color, borderSize) BE_Renderer_DrawBorderedRectangle((position), (size), (color), (color), (borderSize))
