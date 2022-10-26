// Purpose: Data that represent color.
// Created on: 4/26/22 @ 11:04 PM

// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    int r;
    int g;
    int b;
} BE_Color_3I, BE_Colour_3I; // Int

typedef struct {
    unsigned r;
    unsigned g;
    unsigned b;
} BE_Color_3U, BE_Colour_3U; // Unsigned Int

typedef struct {
    float r;
    float g;
    float b;
} BE_Color_3F, BE_Colour_3F; // Float

typedef struct {
    int r;
    int g;
    int b;
    int a;
} BE_Color_4I, BE_Colour_4I; // Int

typedef struct {
    unsigned r;
    unsigned g;
    unsigned b;
    unsigned a;
} BE_Color_4U, BE_Colour_4U; // Unsigned Int

typedef struct {
    float r;
    float g;
    float b;
    float a;
} BE_Color_4F, BE_Colour_4F; // Float
SEC_CPP_SUPPORT_GUARD_END()
