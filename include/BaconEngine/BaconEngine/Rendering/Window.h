// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/DllExport.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DLLEXPORT const char* BE_Window_GetTitle(void);
BE_DLLEXPORT BE_Vector_2U BE_Window_GetSize(void);
BE_DLLEXPORT BE_Vector_2I BE_Window_GetPosition(void);
BE_DLLEXPORT SEC_Boolean BE_Window_IsVisible(void);
BE_DLLEXPORT SEC_Boolean BE_Window_IsStillOpened(void);

BE_DLLEXPORT void BE_Window_SetTitle(const char* newTitle);
BE_DLLEXPORT void BE_Window_SetSize(BE_Vector_2U newSize);
BE_DLLEXPORT void BE_Window_SetWidth(unsigned newWidth);
BE_DLLEXPORT void BE_Window_SetHeight(unsigned newHeight);
BE_DLLEXPORT void BE_Window_SetPosition(BE_Vector_2I newPosition);
BE_DLLEXPORT void BE_Window_SetXPosition(int newX);
BE_DLLEXPORT void BE_Window_SetYPosition(int newY);
BE_DLLEXPORT void BE_Window_SetVisibility(SEC_Boolean visible);
SEC_CPP_SUPPORT_GUARD_END()
