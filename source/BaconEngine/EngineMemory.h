// Purpose: Wrapper functions around memory functions to count memory usage.
// Created on: 9/9/22 @ 10:18 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stddef.h>

#include "AntiClientGuard.h"
#include "BaconEngine/EngineMemoryInformation.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemoryInformation_MemoryType memoryType);
void* BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemoryInformation_MemoryType memoryType);
void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemoryInformation_MemoryType memoryType);
void BE_EngineMemory_AddSize(size_t size, BE_EngineMemoryInformation_MemoryType memoryType);
void BE_EngineMemory_RemoveSize(size_t size, BE_EngineMemoryInformation_MemoryType memoryType);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
