// Purpose: Manage configuration file/strings
// Created on: 12/22/23 @ 3:21 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Storage/DynamicDictionary.h>
#include <stdio.h>

#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BA_DynamicDictionary* BE_Configuration_ParseFromFile(FILE* configurationFile);
BE_BINARYEXPORT BA_DynamicDictionary* BE_Configuration_Parse(const char* configurationData);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
