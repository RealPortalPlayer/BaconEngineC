// Purpose: Metal window functions.
// Created on: 1/1/23 @ 1:42 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "BaconEngine/Math/Vector.h"
#include "../../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void* BE_MetalWindow_GetWindow(void);
const char* BE_MetalWindow_GetTitle(void);
BE_Vector2_Unsigned BE_MetalWindow_GetSize(void);
BE_Vector2_Integer BE_MetalWindow_GetPosition(void);
BA_Boolean BE_MetalWindow_IsVisible(void);
BA_Boolean BE_MetalWindow_IsStillOpened(void);

void BE_MetalWindow_Create(const char* title, BE_Vector2_Unsigned size, int monitor);
void BE_MetalWindow_SetTitle(const char* title);
void BE_MetalWindow_SetSize(BE_Vector2_Unsigned newSize);
void BE_MetalWindow_SetPosition(BE_Vector2_Integer newPosition);
void BE_MetalWindow_SetVisibility(BA_Boolean visible);
void BE_MetalWindow_Close(void);
void BE_MetalWindow_UpdateEvents(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
