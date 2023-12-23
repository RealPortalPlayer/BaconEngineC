// Purpose: Get allocated engine memory information.
// Created on: 9/9/22 @ 10:23 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stddef.h>

#include "BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    size_t allocatedBytes;
    size_t allocatedAmount;
} BE_EngineMemoryInformation_MemoryType;

typedef struct {
    BE_EngineMemoryInformation_MemoryType command;
    BE_EngineMemoryInformation_MemoryType ui;
    BE_EngineMemoryInformation_MemoryType dynamicArray;
    BE_EngineMemoryInformation_MemoryType layer;
    BE_EngineMemoryInformation_MemoryType server;
} BE_EngineMemoryInformation;

BE_BINARYEXPORT BE_EngineMemoryInformation BE_EngineMemoryInformation_Get(void);
BE_BINARYEXPORT size_t BE_EngineMemoryInformation_GetAllocatedBytes(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
