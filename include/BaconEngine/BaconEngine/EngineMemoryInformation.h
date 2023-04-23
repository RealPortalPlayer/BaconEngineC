// Purpose: Get allocated engine memory information.
// Created on: 9/9/22 @ 10:23 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <stddef.h>

#include "DllExport.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    size_t allocatedBytes;
    size_t allocatedAmount;
} BE_EngineMemory_MemoryTypeInformation;

typedef struct {
    BE_EngineMemory_MemoryTypeInformation command;
    BE_EngineMemory_MemoryTypeInformation ui;
    BE_EngineMemory_MemoryTypeInformation dynamicArray;
    BE_EngineMemory_MemoryTypeInformation layer;
} BE_EngineMemory_MemoryInformation;

BE_DLLEXPORT BE_EngineMemory_MemoryInformation BE_EngineMemory_GetMemoryInformation(void);
BE_DLLEXPORT size_t BE_EngineMemory_GetAllocatedBytes(void);
SEC_CPP_SUPPORT_GUARD_END()
