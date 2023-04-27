// Purpose: OpenGL window functions.
// Created on: 9/14/22 @ 12:02 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "../../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"

SEC_CPP_SUPPORT_GUARD_START()
void* BE_OpenGLWindow_GetWindow(void);
const char* BE_OpenGLWindow_GetTitle(void);
BE_Vector_2U BE_OpenGLWindow_GetSize(void);
BE_Vector_2I BE_OpenGLWindow_GetPosition(void);
SEC_Boolean BE_OpenGLWindow_IsVisible(void);
SEC_Boolean BE_OpenGLWindow_IsStillOpened(void);

void BE_OpenGLWindow_Create(const char* title, BE_Vector_2U size, int monitor);
void BE_OpenGLWindow_SetTitle(const char* title);
void BE_OpenGLWindow_SetSize(BE_Vector_2U newSize);
void BE_OpenGLWindow_SetPosition(BE_Vector_2I newPosition);
void BE_OpenGLWindow_SetVisibility(SEC_Boolean visible);
void BE_OpenGLWindow_Close(void);
void BE_OpenGLWindow_UpdateEvents(void);
SEC_CPP_SUPPORT_GUARD_END()
