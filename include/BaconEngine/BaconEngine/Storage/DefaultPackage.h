// Purpose: Manage the default engine package
// Created on: 8/8/2023 @ 2:29 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <stdlib.h>

#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BA_Boolean BE_DefaultPackage_GetFile(const char* filePath, void** buffer, size_t* bufferSize);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
