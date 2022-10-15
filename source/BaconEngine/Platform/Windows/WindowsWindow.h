// Purpose: Windows software window functions.
// Created on: 10/9/22 @ 3:17 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"

SEC_CPP_SUPPORT_GUARD_START()
void* BE_WindowsWindow_GetInternal(void);
void BE_WindowsWindow_Create(const char* title, BE_Vector_2U size, int monitor);
const char* BE_WindowsWindow_GetTitle(void);
BE_Vector_2U BE_WindowsWindow_GetSize(void);
BE_Vector_2I BE_WindowsWindow_GetPosition(void);
int BE_WindowsWindow_IsVisible(void);
int BE_WindowsWindow_IsStillOpened(void);
void BE_WindowsWindow_SetTitle(const char* title);
void BE_WindowsWindow_SetSize(BE_Vector_2U newSize);
void BE_WindowsWindow_SetPosition(BE_Vector_2I newPosition);
void BE_WindowsWindow_SetVisibility(int visible);
void BE_WindowsWindow_Close(void);
void BE_WindowsWindow_UpdateEvents(void);
SEC_CPP_SUPPORT_GUARD_END()
