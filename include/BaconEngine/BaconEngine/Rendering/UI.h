// Purpose: UI handing code.
// Created on: 8/9/2022 @ 4:07 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include "SharedEngineCode/Storage/DynamicArray.h"

#include "BaconEngine/Math/Vector.h"
#include "UIElement.h"
#include "BaconEngine/SDLSupport.h"

SEC_CPP_GUARD_START()
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
    } BE_UI_WindowFlags;

    typedef struct BE_UI_Window {
        const char* name;
        const char* originalName;
        BE_Vector_2I position;
        BE_Vector_2U size;
        BE_UI_WindowFlags flags;
        SEC_DynamicArray elements;
    } BE_UI_Window;

    void BE_UI_InitializeUIs(void);
    void BE_UI_RegisterWindow(const char* name, BE_Vector_2I position, BE_Vector_2U size, BE_UI_WindowFlags flags);
    void BE_UI_RegisterElement(BE_UIElement* element);
    void BE_UI_FinishRegisteringWindow(void);
    int BE_UI_CloseWindow(const char* name);
    int BE_UI_CloseWindowAt(unsigned int index);
    int BE_UI_ToggleWindowRendering(int enable);
    void BE_UI_WindowRendererDrawRectangle(BE_UI_Window* window, BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U borderColor, BE_Color_4U fillColor, int borderPadding);
    BE_UI_Window** BE_UI_GetWindows(void);
    int BE_UI_GetWindowAmount(void);
    int BE_UI_GetAllocatedWindowAmount(void);
    int BE_UI_GetWindowReallocationAmount(void);
    void BE_UI_SetCurrentWindow(BE_UI_Window* window);
    BE_UI_Window* BE_UI_GetCurrentWindow(void);
    TTF_Font* BE_UI_GetWindowFont(void);
    void BE_UI_DestroyWindows(void);
SEC_CPP_GUARD_END()

#define BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, position, size, color, borderSize) BE_UI_WindowRendererDrawRectangle((uiWindow), (position), (size), (color), (color), (borderSize))
