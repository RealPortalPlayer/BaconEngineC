// Purpose: Renderer specific functions.
// Created on: 9/14/22 @ 11:54 AM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "SharedEngineCode/Internal/CppSupport.h"

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    void* (*GetInternalWindow)(void);
    void (*CreateWindow)(const char* title, BE_Vector_2U size, int monitor);
    const char* (*GetTitle)(void);
    BE_Vector_2U (*GetSize)(void);
    BE_Vector_2I (*GetPosition)(void);
    int (*IsVisible)(void);
    int (*IsStillOpened)(void);
    void (*SetTitle)(const char* title);
    void (*SetSize)(BE_Vector_2U newSize);
    void (*SetPosition)(BE_Vector_2I position);
    void (*SetVisibility)(int visible);
    void (*CloseWindow)(void);
    void (*UpdateEvents)(void);
} BE_SpecificPlatformFunctions_Window;

typedef struct {
    void (*ClearScreen)(void);
    void (*SetClearColor)(BE_Color_3U color);
    BE_Color_3U (*GetClearColor)(void);
    void (*Render)(void);
    void (*DrawFilledRectangle)(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
} BE_SpecificPlatformFunctions_Renderer;

typedef struct {
    BE_SpecificPlatformFunctions_Window windowFunctions;
    BE_SpecificPlatformFunctions_Renderer rendererFunctions;
    void (*Destroy)(void);
    double (*GetTimer)(void);
} BE_SpecificPlatformFunctions;

BE_SpecificPlatformFunctions BE_SpecificPlatformFunctions_Get(void);
void BE_SpecificPlatformFunctions_InitializeWindow(BE_SpecificPlatformFunctions_Window functions);
void BE_SpecificPlatformFunctions_InitializeRenderer(BE_SpecificPlatformFunctions_Renderer renderer);
void BE_SpecificPlatformFunctions_SetDestroy(void (*Destroy)(void));
void BE_SpecificPlatformFunctions_SetGetTimer(double (*GetTimer)(void));
SEC_CPP_SUPPORT_GUARD_END()
