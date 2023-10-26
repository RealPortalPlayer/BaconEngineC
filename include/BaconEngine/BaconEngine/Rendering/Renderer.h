// Purpose: Helps with rendering.
// Created on: 4/3/22 @ 7:48 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_RENDERER_TYPE_OPENGL,
    BE_RENDERER_TYPE_VULKAN,
    BE_RENDERER_TYPE_METAL,
    BE_RENDERER_TYPE_DIRECTX,
    BE_RENDERER_TYPE_SOFTWARE,
    BE_RENDERER_TYPE_TEXT
} BE_Renderer_Types;

BE_BINARYEXPORT int BE_Renderer_GetCalls(void);
BE_BINARYEXPORT BE_Renderer_Types BE_Renderer_GetCurrentType(void);
BE_BINARYEXPORT BE_Color3_Unsigned BE_Renderer_GetClearColor(void);

BE_BINARYEXPORT void BE_Renderer_ClearScreen(void);
BE_BINARYEXPORT BE_Vector2_Integer BE_Renderer_GetCenterPosition(BE_Vector2_Integer rectanglePosition, BE_Vector2_Unsigned rectangleSize, BE_Vector2_Unsigned objectSize);
BE_BINARYEXPORT void BE_Renderer_DrawLine(BE_Vector2_Integer firstPoint, BE_Vector2_Integer secondPoint, BE_Color4_Unsigned color);
BE_BINARYEXPORT void BE_Renderer_DrawPoint(BE_Vector2_Integer position, BE_Color4_Unsigned color);
BE_BINARYEXPORT void BE_Renderer_DrawRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color);
BE_BINARYEXPORT void BE_Renderer_FillRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color);
BE_BINARYEXPORT void BE_Renderer_DrawBorderedRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned borderColor, BE_Color4_Unsigned fillColor, int borderPadding);
// BE_Vector2_Unsigned BE_Renderer_GetFontSize(TTF_Font* font, const char* text);
BE_BINARYEXPORT void BE_Renderer_SetClearColor(BE_Color3_Unsigned color);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, size, color, borderPadding) BE_Renderer_DrawBorderedRectangle((position), (size), (color), (color), (borderPadding))
