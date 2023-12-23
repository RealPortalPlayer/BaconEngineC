// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    int x;
    int y;
} BE_Vector2_Integer;

typedef struct {
    unsigned x;
    unsigned y;
} BE_Vector2_Unsigned;

typedef struct {
    float x;
    float y;
} BE_Vector2_Float;

typedef struct {
    int x;
    int y;
    int z;
} BE_Vector3_Integer;

typedef struct {
    unsigned x;
    unsigned y;
    unsigned z;
} BE_Vector3_Unsigned;

typedef struct {
    float x;
    float y;
    float z;
} BE_Vector3_Float;

typedef struct {
    int x;
    int y;
    int z;
    int w;
} BE_Vector4_Integer;

typedef struct {
    unsigned x;
    unsigned y;
    unsigned z;
    unsigned w;
} BE_Vector4_Unsigned;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} BE_Vector4_Float;
BA_CPLUSPLUS_SUPPORT_GUARD_END()
