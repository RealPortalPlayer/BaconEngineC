// Purpose: SDL window wrapper.
// Created on: 3/31/22 @ 1:52 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/String.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/BinaryExport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT const char* BE_Window_GetTitle(void);
BE_BINARYEXPORT BE_Vector2_Unsigned BE_Window_GetSize(void);
BE_BINARYEXPORT BE_Vector2_Integer BE_Window_GetPosition(void);
BE_BINARYEXPORT SEC_Boolean BE_Window_IsVisible(void);
BE_BINARYEXPORT SEC_Boolean BE_Window_IsStillOpened(void);

BE_BINARYEXPORT void BE_Window_SetTitle(const char* newTitle);
BE_BINARYEXPORT void BE_Window_SetSize(BE_Vector2_Unsigned newSize);
BE_BINARYEXPORT void BE_Window_SetWidth(unsigned newWidth);
BE_BINARYEXPORT void BE_Window_SetHeight(unsigned newHeight);
BE_BINARYEXPORT void BE_Window_SetPosition(BE_Vector2_Integer newPosition);
BE_BINARYEXPORT void BE_Window_SetXPosition(int newX);
BE_BINARYEXPORT void BE_Window_SetYPosition(int newY);
BE_BINARYEXPORT void BE_Window_SetVisibility(SEC_Boolean visible);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
