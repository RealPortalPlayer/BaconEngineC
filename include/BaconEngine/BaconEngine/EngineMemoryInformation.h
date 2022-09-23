// Purpose: Get allocated engine memory information.
// Created on: 9/9/22 @ 10:23 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <stddef.h>

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

BE_EngineMemory_MemoryInformation BE_EngineMemory_GetMemoryInformation(void);
size_t BE_EngineMemory_GetAllocatedBytes(void);
SEC_CPP_SUPPORT_GUARD_END()
