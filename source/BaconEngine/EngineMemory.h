// Purpose: Wrapper functions around memory functions to count memory usage.
// Created on: 9/9/22 @ 10:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stddef.h>

#include "AntiClientGuard.h"
#include "BaconEngine/EngineMemoryInformation.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND,
    BE_ENGINEMEMORY_MEMORY_TYPE_UI,
    BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY,
    BE_ENGINEMEMORY_MEMORY_TYPE_LAYER,
    BE_ENGINEMEMORY_MEMORY_TYPE_SERVER
} BE_EngineMemory_MemoryType;

void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemory_MemoryType memoryType);
void* BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemory_MemoryType memoryType);
void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemory_MemoryType memoryType);
void BE_EngineMemory_AddSize(size_t size, BE_EngineMemory_MemoryType memoryType);
void BE_EngineMemory_RemoveSize(size_t size, BE_EngineMemory_MemoryType memoryType);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
