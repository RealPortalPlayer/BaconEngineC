// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#define BE_VECTOR_CREATE2(type, name) \
typedef struct {                      \
    type x;                           \
    type y;                           \
} BE_Vector2_ ## name

#define BE_VECTOR_CREATE3(type, name) \
typedef struct {                      \
    type x;                           \
    type y;                           \
    type z;                           \
} BE_Vector3_ ## name

#define BE_VECTOR_CREATE4(type, name) \
typedef struct {                      \
    type x;                           \
    type y;                           \
    type z;                           \
    type w;                           \
} BE_Vector4_ ## name

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_VECTOR_CREATE2(int, Integer);
BE_VECTOR_CREATE2(unsigned, Unsigned);
BE_VECTOR_CREATE2(float, Float);
BE_VECTOR_CREATE3(int, Integer);
BE_VECTOR_CREATE3(unsigned, Unsigned);
BE_VECTOR_CREATE3(float, Float);
BE_VECTOR_CREATE4(int, Integer);
BE_VECTOR_CREATE4(unsigned, Unsigned);
BE_VECTOR_CREATE4(float, Float);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
