// Purpose: UI handing code.
// Created on: 8/9/2022 @ 4:07 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include "SharedEngineCode/Storage/DynamicArray.h"

#include "BaconEngine/Math/Vector.h"
#include "UIElements.h"
#include "BaconEngine/SDLSupport.h"

CPP_GUARD_START()
    typedef enum {
        BE_UI_WINDOW_FLAG_NULL,
        BE_UI_WINDOW_FLAG_NOT_MOVABLE,
        BE_UI_WINDOW_FLAG_NOT_RESIZABLE = (1 << 2),
        BE_UI_WINDOW_FLAG_MINIMIZED = (1 << 3),
        BE_UI_WINDOW_FLAG_MAXIMIZED = (1 << 4),
        BE_UI_WINDOW_FLAG_CLOSED = (1 << 5),
        BE_UI_WINDOW_FLAG_NO_TITLE_BAR = (1 << 6),
        BE_UI_WINDOW_FLAG_NO_CLOSE = (1 << 7),
        BE_UI_WINDOW_FLAG_NO_MINIMIZE = (1 << 8),
        BE_UI_WINDOW_FLAG_NO_MAXIMIZE = (1 << 9),
        BE_UI_WINDOW_FLAG_NO_BORDER = (1 << 10),
        BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW = (1 << 11) | BE_UI_WINDOW_FLAG_NO_TITLE_BAR,
        BE_UI_WINDOW_FLAG_NO_TITLE_BAR_BUTTONS = BE_UI_WINDOW_FLAG_NO_CLOSE | BE_UI_WINDOW_FLAG_NO_MINIMIZE | BE_UI_WINDOW_FLAG_NO_MAXIMIZE
    } BE_UIWindowFlags;

    typedef struct BE_UIWindow {
        const char* name;
        const char* originalName;
        BE_Vector2I position;
        BE_Vector2U size;
        BE_UIWindowFlags flags;
        SEC_DynamicArray elements;
    } BE_UIWindow;

    void BE_InitializeUISystem(void);
    void BE_RegisterUIWindow(const char* name, BE_Vector2I position, BE_Vector2U size, BE_UIWindowFlags flags);
    void BE_RegisterUIElement(BE_UIElement* element);
    void BE_FinishRegisteringUIWindow(void);
    int BE_CloseUIWindow(const char* name);
    int BE_CloseUIWindowAt(unsigned int index);
    int BE_ToggleUIRendering(int enable);
    void UIWindowRendererDrawRectangle(BE_UIWindow* window, BE_Vector2I position, BE_Vector2U size, BE_Color4U borderColor, BE_Color4U fillColor, int borderPadding);
    BE_UIWindow** BE_GetUIWindows(void);
    int BE_GetUIWindowAmount(void);
    int BE_GetAllocatedUIWindowAmount(void);
    int BE_GetUIWindowReallocationAmount(void);
    void BE_SetCurrentUIWindow(BE_UIWindow* window);
    BE_UIWindow* BE_GetCurrentUIWindow(void);
    TTF_Font* BE_GetUIWindowFont(void);
    void BE_DestroyUIWindows(void);
CPP_GUARD_END()

#define BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, position, size, color, borderSize) UIWindowRendererDrawRectangle((uiWindow), (position), (size), (color), (color), (borderSize))
