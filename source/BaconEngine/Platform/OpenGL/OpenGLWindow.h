// Purpose: OpenGL window functions.
// Created on: 9/14/22 @ 12:02 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "../../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void* BE_OpenGLWindow_GetWindow(void);
const char* BE_OpenGLWindow_GetTitle(void);
BE_Vector2_Unsigned BE_OpenGLWindow_GetSize(void);
BE_Vector2_Integer BE_OpenGLWindow_GetPosition(void);
BA_Boolean BE_OpenGLWindow_IsVisible(void);
BA_Boolean BE_OpenGLWindow_IsStillOpened(void);

void BE_OpenGLWindow_Create(const char* title, BE_Vector2_Unsigned size, int monitor);
void BE_OpenGLWindow_SetTitle(const char* title);
void BE_OpenGLWindow_SetSize(BE_Vector2_Unsigned newSize);
void BE_OpenGLWindow_SetPosition(BE_Vector2_Integer newPosition);
void BE_OpenGLWindow_SetVisibility(BA_Boolean visible);
void BE_OpenGLWindow_Close(void);
void BE_OpenGLWindow_UpdateEvents(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
