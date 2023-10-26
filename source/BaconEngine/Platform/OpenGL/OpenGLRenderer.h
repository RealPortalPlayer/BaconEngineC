// Purpose: OpenGL renderer functions.
// Created on: 9/15/22 @ 12:50 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_Color3_Unsigned BE_OpenGLRenderer_GetClearColor(void);

void BE_OpenGLRenderer_CompileShaders(void);
void BE_OpenGLRenderer_ClearScreen(void);
void BE_OpenGLRenderer_SetClearColor(BE_Color3_Unsigned color);
void BE_OpenGLRenderer_Render(void);
void BE_OpenGLRenderer_DrawFilledRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
