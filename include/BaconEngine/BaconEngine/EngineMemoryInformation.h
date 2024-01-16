// Purpose: Get allocated engine memory information.
// Created on: 9/9/22 @ 10:23 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stddef.h>

#include "BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_COMMAND,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_LAYER,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CONNECTED,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT_SOCKET,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_NAME,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_ARGUMENT,
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_TEMPORARY_ARGUMENT,
    
    /**
     * Not a real type
     */
    BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE
} BE_EngineMemoryInformation_MemoryType;

typedef struct {
    size_t allocatedBytes;
    size_t allocatedAmount;
} BE_EngineMemoryInformation_MemoryTypeData;

BE_BINARYEXPORT const BE_EngineMemoryInformation_MemoryTypeData* BE_EngineMemoryInformation_Get(void);
BE_BINARYEXPORT size_t BE_EngineMemoryInformation_GetAllocatedBytes(void);
BE_BINARYEXPORT size_t BE_EngineMemoryInformation_GetAllocatedAmount(void);

/**
 * @note Free after usage
 */
BE_BINARYEXPORT char* BE_EngineMemoryInformation_GetAllocationInformation(const char* prefix);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
