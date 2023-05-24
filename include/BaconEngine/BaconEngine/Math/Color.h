// Purpose: Data that represent color.
// Created on: 4/26/22 @ 11:04 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    int r;
    int g;
    int b;
} BE_Color3_Integer, BE_Colour3_Integer;

typedef struct {
    unsigned r;
    unsigned g;
    unsigned b;
} BE_Color3_Unsigned, BE_Colour3_Unsigned;

typedef struct {
    float r;
    float g;
    float b;
} BE_Color3_Float, BE_Colour3_Float;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} BE_Color4_Integer, BE_Colour4_Integer;

typedef struct {
    unsigned r;
    unsigned g;
    unsigned b;
    unsigned a;
} BE_Color4_Unsigned, BE_Colour4_Unsigned;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} BE_Color4_Float, BE_Colour4_Float;
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
