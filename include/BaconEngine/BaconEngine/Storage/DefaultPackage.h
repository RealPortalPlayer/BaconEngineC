// Purpose: Manage the default engine package
// Created on: 8/8/23 @ 2:29 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <stdlib.h>

#include "BaconEngine/BinaryExport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT SEC_Boolean BE_DefaultPackage_GetFile(const char* filePath, void** buffer, size_t* bufferSize);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
