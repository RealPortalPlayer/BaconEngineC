// Purpose: OpenGL renderer functions.
// Created on: 9/15/22 @ 12:50 AM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_OpenGLRenderer_CompileShaders(void);
void BE_OpenGLRenderer_ClearScreen(void);
void BE_OpenGLRenderer_SetClearColor(BE_Color_3U color);
BE_Color_3U BE_OpenGLRenderer_GetClearColor(void);
void BE_OpenGLRenderer_Render(void);
void BE_OpenGLRenderer_DrawFilledRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
SEC_CPP_SUPPORT_GUARD_END()
