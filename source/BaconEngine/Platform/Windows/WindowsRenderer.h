// Purpose: Windows software renderer functions.
// Created on: 10/9/22 @ 3:17 PM

// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_WindowsRenderer_CompileShaders(void);
void BE_WindowsRenderer_ClearScreen(void);
void BE_WindowsRenderer_SetClearColor(BE_Color_3U color);
BE_Color_3U BE_WindowsRenderer_GetClearColor(void);
void BE_WindowsRenderer_Render(void);
void BE_WindowsRenderer_DrawFilledRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
SEC_CPP_SUPPORT_GUARD_END()
