// Purpose: UI handing code.
// Created on: 8/9/2022 @ 4:07 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"

CPP_GUARD_START()
    typedef enum {
        UI_WINDOW_FLAG_NULL,
        UI_WINDOW_FLAG_NOT_MOVABLE,
        UI_WINDOW_FLAG_NOT_RESIZABLE = (1 << 2),
        UI_WINDOW_FLAG_NO_TITLE_BAR = (1 << 3),
        UI_WINDOW_FLAG_NO_CLOSE = (1 << 4),
        UI_WINDOW_FLAG_NO_MINIMIZE = (1 << 5),
        UI_WINDOW_FLAG_NO_MAXIMIZE = (1 << 6),
        UI_WINDOW_FLAG_NO_TITLE_BAR_BUTTONS = UI_WINDOW_FLAG_NO_CLOSE | UI_WINDOW_FLAG_NO_MINIMIZE | UI_WINDOW_FLAG_NO_MAXIMIZE
    } UIWindowFlags;

    typedef struct {
        const char* name;
        Vector2I position;
        Vector2I size;
        UIWindowFlags flags;
    } UIWindow;

    void InitializeUISystem(void);
    void RegisterUIWindow(const char* name, Vector2I position, Vector2I size, UIWindowFlags flags);
    UIWindow** GetUIWindows(void);
    int GetUIWindowAmount(void);
    int GetAllocatedUIWindowAmount(void);
    int GetUIWindowReallocationAmount(void);
CPP_GUARD_END()
