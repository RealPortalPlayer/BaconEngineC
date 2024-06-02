// Purpose: Helper functions to help with arguments
// Created on: 1/8/24 @ 7:09 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Storage/DynamicArray.h>

#include "AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_PrivateArgumentManager_ParseName(const char* input, char** name, int* argumentStartingIndex);
BA_Boolean BE_PrivateArgumentManager_ParseArguments(const char* input, int startingIndex, BA_Boolean fancyArgumentParsing, BA_DynamicArray comparedArguments, BA_DynamicDictionary* arguments);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
