// Purpose: Data that represent color.
// Created on: 4/26/22 @ 11:04 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#define BE_COLOR_CREATE3(type, name) \
typedef struct {                     \
    type r;                          \
    type g;                          \
    type b;                          \
} BE_Color3_ ## name, BE_Colour3_ ## name

#define BE_COLOR_CREATE4(type, name) \
typedef struct {                     \
    type r;                          \
    type g;                          \
    type b;                          \
    type a;                          \
} BE_Color4_ ## name, BE_Colour4_ ## name

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_COLOR_CREATE3(int, Integer);
BE_COLOR_CREATE3(unsigned, Unsigned);
BE_COLOR_CREATE3(float, Float);
BE_COLOR_CREATE4(int, Integer);
BE_COLOR_CREATE4(unsigned, Unsigned);
BE_COLOR_CREATE4(float, Float);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
