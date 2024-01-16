// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Debugging/StaticAssert.h>
#include <BaconAPI/String.h>

#ifndef BE_CLIENT_BINARY
#   include "EngineMemory.h"
#endif

#include "InterfaceFunctions.h"
#include "BaconEngine/EngineMemoryInformation.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_EngineMemoryInformation_MemoryTypeData beEngineMemoryInformation[] = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}
};

BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(beEngineMemoryInformation, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE);

BA_Boolean BE_EngineMemory_AllocateDeallocateLogsEnabled(void) {
#   ifdef BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION, SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT, 0);

    return enabled;
#   else
    return BA_BOOLEAN_FALSE;
#   endif
}
#endif

const BE_EngineMemoryInformation_MemoryTypeData* BE_EngineMemoryInformation_Get(void) {
#ifndef BE_CLIENT_BINARY
    return beEngineMemoryInformation;
#else
    BE_INTERFACEFUNCTION(BE_EngineMemoryInformation_MemoryTypeData*, void);
    return function();
#endif
}

size_t BE_EngineMemoryInformation_GetAllocatedBytes(void) {
#ifndef BE_CLIENT_BINARY
    size_t allocatedBytes = 0;
    
    for (int i = 0; i < BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE; i++)
        allocatedBytes += beEngineMemoryInformation[i].allocatedBytes;
    
    return allocatedBytes;
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_EngineMemoryInformation_MemoryTypeData* BE_EngineMemory_GetMemoryTypeInformation(BE_EngineMemoryInformation_MemoryType memoryType) {
    BA_ASSERT(memoryType < BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE, "This shouldn't happen\n");
    return &beEngineMemoryInformation[memoryType];
}

void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemoryInformation_MemoryType memoryType) {
    void* pointer;
    BE_EngineMemoryInformation_MemoryTypeData* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(memoryType);

    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Allocating memory\n"
                        "Size: %zu\n"
                        "Type: %i\n", size, memoryType);

    BA_ASSERT(size > 0, "Size cannot be zero or below\n");
    BA_ASSERT((pointer = malloc(size)) != NULL, "Failed to allocate %zu bytes of data\n", size);

    memoryTypeInformation->allocatedBytes += size;
    memoryTypeInformation->allocatedAmount++;

    return pointer;
}

void* BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemoryInformation_MemoryType memoryType) {
    BE_EngineMemoryInformation_MemoryTypeData* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(memoryType);
    void* newPointer;

    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Reallocating memory\n"
                        "Old Size: %zu\n"
                        "New Size: %zu\n"
                        "Type: %i\n", oldSize, newSize, memoryType);

    BA_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BA_ASSERT(oldSize > 0 && newSize > 0, "Size cannot be zero or below\n");
    BA_ASSERT((newPointer = realloc(pointer, newSize)) != NULL, "Failed to reallocate %zu bytes of data\n", newSize);

    memoryTypeInformation->allocatedBytes = memoryTypeInformation->allocatedBytes - oldSize + newSize;
    return newPointer;
}

void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemoryInformation_MemoryType memoryType) {
    BE_EngineMemoryInformation_MemoryTypeData* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(memoryType);

    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Deallocating memory\n"
                        "Size: %zu\n"
                        "Type: %i\n", oldSize, memoryType);

    BA_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BA_ASSERT(oldSize > 0, "Size cannot be zero or below\n");
    free(pointer);

    memoryTypeInformation->allocatedBytes -= oldSize;
    memoryTypeInformation->allocatedAmount--;
}

void BE_EngineMemory_AddSize(size_t size, BE_EngineMemoryInformation_MemoryType memoryType) {
    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Adding to allocated bytes\n"
                        "Size: %zu\n"
                        "Type: %i\n", size, memoryType);

    BE_EngineMemory_GetMemoryTypeInformation(memoryType)->allocatedBytes += size;
}

void BE_EngineMemory_RemoveSize(size_t size, BE_EngineMemoryInformation_MemoryType memoryType) {
    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Removing from allocated bytes\n"
                        "Size: %zu\n"
                        "Type: %i\n", size, memoryType);

    BE_EngineMemory_GetMemoryTypeInformation(memoryType)->allocatedBytes -= size;
}
#endif

char* BE_EngineMemoryInformation_GetAllocationInformation(const char* prefix) {
#ifndef BE_CLIENT_BINARY
    char* finalString = BA_String_Copy("%sCommand: %zu allocated, %zu bytes\n"
                                       "%sUI: %zu allocated, %zu bytes\n"
                                       "%sDynamicArray: %zu allocated, %zu bytes\n"
                                       "%sLayer: %zu allocated, %zu bytes\n"
                                       "%sServer:\n"
                                       "%s  Connected: %zu allocated, %zu bytes\n"
                                       "%s  Client: %zu allocated, %zu bytes\n"
                                       "%s  Client Socket: %zu allocated, %zu bytes\n"
                                       "%sArgumentManager:\n"
                                       "%s  Name: %zu allocated, %zu bytes\n"
                                       "%s  Argument: %zu allocated, %zu bytes\n"
                                       "%s  Temporary Argument: %zu allocated, %zu bytes");

    BA_ASSERT(finalString != NULL, "Failed to allocate memory for a string\n");
    
#   define BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(memoryType) prefix, beEngineMemoryInformation[memoryType].allocatedAmount, beEngineMemoryInformation[memoryType].allocatedBytes
    
    BA_ASSERT(BA_String_Format(&finalString,
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_COMMAND),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_LAYER),
                               prefix,
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CONNECTED),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT_SOCKET),
                               prefix,
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_NAME),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_ARGUMENT),
                               BE_ENGINEMEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_TEMPORARY_ARGUMENT)) != NULL, "Failed to format string\n");

#   undef BE_ENGINEMEMORY_GET_MEMORY_INFORMATION
    
    return finalString;
#else
    BE_INTERFACEFUNCTION(char*, const char*);
    return function(prefix);
#endif
}

size_t BE_EngineMemoryInformation_GetAllocatedAmount(void) {
#ifndef BE_CLIENT_BINARY
    size_t finalAmount = 0;

    for (int i = 0; i < BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE; i++)
        finalAmount += beEngineMemoryInformation[i].allocatedAmount;

    return finalAmount;
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
