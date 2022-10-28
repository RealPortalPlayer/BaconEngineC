// Purpose: Renderer specific functions.
// Created on: 9/14/22 @ 11:54 AM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    struct {
        void (*CreateWindow)(const char* title, BE_Vector_2U size, int monitor);
        const char* (*GetTitle)(void);
        BE_Vector_2U (*GetSize)(void);
        BE_Vector_2I (*GetPosition)(void);
        SEC_Boolean (*IsVisible)(void);
        SEC_Boolean (*IsStillOpened)(void);
        void (*SetTitle)(const char* title);
        void (*SetSize)(BE_Vector_2U newSize);
        void (*SetPosition)(BE_Vector_2I position);
        void (*SetVisibility)(SEC_Boolean visible);
        void (*CloseWindow)(void);
        void (*UpdateEvents)(void);
    } windowFunctions;
    struct {
        void (*ClearScreen)(void);
        void (*SetClearColor)(BE_Color_3U color);
        BE_Color_3U (*GetClearColor)(void);
        void (*Render)(void);
        void (*DrawFilledRectangle)(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
    } rendererFunctions;
    struct {
        SEC_Boolean (*IsKeyDown)(BE_Keyboard_KeyCodes keyCode);
        SEC_Boolean (*IsKeyPressed)(BE_Keyboard_KeyCodes keyCode);
        SEC_Boolean (*IsMouseButtonDown)(BE_Mouse_ButtonType button);
    } inputFunctions;
    void (*Destroy)(void);
    double (*GetTimer)(void);
} BE_SpecificPlatformFunctions;

BE_SpecificPlatformFunctions BE_SpecificPlatformFunctions_Get(void);
void BE_SpecificPlatformFunctions_Initialize(BE_SpecificPlatformFunctions functions);
SEC_CPP_SUPPORT_GUARD_END()
