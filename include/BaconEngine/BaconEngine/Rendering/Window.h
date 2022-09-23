// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"

SEC_CPP_SUPPORT_GUARD_START()
const char* BE_Window_GetTitle(void);
BE_Vector_2U BE_Window_GetSize(void);
BE_Vector_2I BE_Window_GetPosition(void);
int BE_Window_IsVisible(void);
int BE_Window_IsStillOpened(void);
void BE_Window_SetTitle(const char* newTitle);
void BE_Window_SetSize(BE_Vector_2U newSize);
void BE_Window_SetWidth(unsigned newWidth);
void BE_Window_SetHeight(unsigned newHeight);
void BE_Window_SetPosition(BE_Vector_2I newPosition);
void BE_Window_SetXPosition(int newX);
void BE_Window_SetYPosition(int newY);
void BE_Window_SetVisibility(int visible);
SEC_CPP_SUPPORT_GUARD_END()
