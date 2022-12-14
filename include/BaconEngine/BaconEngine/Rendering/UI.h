// Purpose: UI system.
// Created on: 9/11/22 @ 7:15 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Event.h"
#include "BaconEngine/Storage/DynamicArray.h"

SEC_CPP_SUPPORT_GUARD_START()
struct BE_UI_Element;

typedef struct {
    void (*OnRender)(struct BE_UI_Element* thisElement);
    void (*OnEvent)(struct BE_UI_Element* thisElement, BE_Event event);
} BE_UI_ElementFunctions;

typedef struct BE_UI_Element {
    BE_UI_ElementFunctions functions;
    BE_Vector_2U position;
    BE_Vector_2U size;
    SEC_Boolean enabled;
    BE_DynamicArray elements;
} BE_UI_Element;

typedef enum {
    BE_UI_WINDOW_FLAG_NULL,
    BE_UI_WINDOW_FLAG_NO_TITLE_BAR,
    BE_UI_WINDOW_FLAG_NO_CLOSE = (1 << 2),
    BE_UI_WINDOW_FLAG_NO_MINIMIZE = (1 << 3),
    BE_UI_WINDOW_FLAG_NO_MAXIMIZE = (1 << 4),
    BE_UI_WINDOW_FLAG_CLOSED = (1 << 5),
    BE_UI_WINDOW_FLAG_NOT_RESIZABLE = (1 << 5),
    BE_UI_WINDOW_FLAG_NOT_MOVABLE = (1 << 6),
    BE_UI_WINDOW_FLAG_MINIMIZED = (1 << 7),
    BE_UI_WINDOW_FLAG_MAXIMIZED = (1 << 8)
} BE_UI_WindowFlags;

const char* BE_UI_GetWindowName(int windowId);
SEC_Boolean BE_UI_IsWindowStillOpen(int windowId);
BE_DynamicArray* BE_UI_GetWindowElements(int windowId);
int BE_UI_GetWindowAmount(void);
int BE_UI_GetAllocatedWindowsAmount(void);
int BE_UI_GetWindowReallocationAmount(void);

int BE_UI_RegisterWindow(const char* name, BE_UI_WindowFlags flags, BE_Vector_2I position, BE_Vector_2U size);
SEC_Boolean BE_UI_RegisterElement(int windowId, BE_UI_Element* element);
SEC_Boolean BE_UI_ToggleWindowFlag(int windowId, BE_UI_WindowFlags flag, SEC_Boolean toggle);
SEC_Boolean BE_UI_SetActiveWindow(int windowId);
SEC_CPP_SUPPORT_GUARD_END()
