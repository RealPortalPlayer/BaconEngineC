// Purpose: Manages the arguments a user sends.
// Created on: 8/7/22 @ 12:02 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <stddef.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <SharedEngineCode/String.h>

#include "BaconEngine/BinaryExport.h"
#include "BaconEngine/Storage/DynamicDictionary.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT int BE_ArgumentManager_GetInteger(BE_DynamicDictionary arguments, const char* name, int defaultValue);
BE_BINARYEXPORT SEC_Boolean BE_ArgumentManager_GetBoolean(BE_DynamicDictionary arguments, const char* name, SEC_Boolean defaultValue);
BE_BINARYEXPORT float BE_ArgumentManager_GetFloat(BE_DynamicDictionary arguments, const char* name, float defaultValue);
BE_BINARYEXPORT const char* BE_ArgumentManager_GetString(BE_DynamicDictionary arguments, const char* name, const char* defaultValue);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
