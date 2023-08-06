// Purpose: Renderer specific functions.
// Created on: 9/14/22 @ 11:54 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <SharedEngineCode/String.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    struct {
        void (*CreateWindow)(const char* title, BE_Vector2_Unsigned size, int monitor);
        const char* (*GetTitle)(void);
        BE_Vector2_Unsigned (*GetSize)(void);
        BE_Vector2_Integer (*GetPosition)(void);
        SEC_Boolean (*IsVisible)(void);
        SEC_Boolean (*IsStillOpened)(void);
        void (*SetTitle)(const char* title);
        void (*SetSize)(BE_Vector2_Unsigned newSize);
        void (*SetPosition)(BE_Vector2_Integer position);
        void (*SetVisibility)(SEC_Boolean visible);
        void (*CloseWindow)(void);
        void (*UpdateEvents)(void);
    } windowFunctions;
    struct {
        void (*ClearScreen)(void);
        void (*SetClearColor)(BE_Color3_Unsigned color);
        BE_Color3_Unsigned (*GetClearColor)(void);
        void (*Render)(void);
        void (*DrawFilledRectangle)(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color);
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
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
