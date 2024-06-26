// Purpose: Manages the arguments a user sends.
// Created on: 8/7/22 @ 12:02 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stddef.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/String.h>
#include <BaconAPI/Storage/DynamicDictionary.h>

#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    char* name;
    BA_Boolean required;
} BE_ArgumentManager_Argument;

BE_BINARYEXPORT int BE_ArgumentManager_GetInteger(BA_DynamicDictionary arguments, const char* name, int defaultValue);
BE_BINARYEXPORT BA_Boolean BE_ArgumentManager_GetBoolean(BA_DynamicDictionary arguments, const char* name, BA_Boolean defaultValue);
BE_BINARYEXPORT float BE_ArgumentManager_GetFloat(BA_DynamicDictionary arguments, const char* name, float defaultValue);
BE_BINARYEXPORT const char* BE_ArgumentManager_GetString(BA_DynamicDictionary arguments, const char* name, const char* defaultValue);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
