// Purpose: Windows software window functions.
// Created on: 10/9/22 @ 3:17 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "../../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void* BE_WindowsWindow_GetInternal(void);
const char* BE_WindowsWindow_GetTitle(void);
BE_Vector2_Unsigned BE_WindowsWindow_GetSize(void);
BE_Vector2_Integer BE_WindowsWindow_GetPosition(void);
SEC_Boolean BE_WindowsWindow_IsVisible(void);
SEC_Boolean BE_WindowsWindow_IsStillOpened(void);

void BE_WindowsWindow_Create(const char* title, BE_Vector2_Unsigned size, int monitor);
void BE_WindowsWindow_SetTitle(const char* title);
void BE_WindowsWindow_SetSize(BE_Vector2_Unsigned newSize);
void BE_WindowsWindow_SetPosition(BE_Vector2_Integer newPosition);
void BE_WindowsWindow_SetVisibility(SEC_Boolean visible);
void BE_WindowsWindow_Close(void);
void BE_WindowsWindow_UpdateEvents(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
