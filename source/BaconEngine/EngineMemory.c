// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Debugging/Assert.h>

#include "EngineMemory.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_EngineMemoryInformation beEngineMemoryInformation = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}
};

BA_Boolean BE_EngineMemory_AllocateDeallocateLogsEnabled(void) {
#ifdef BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION, SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT, 0);

    return enabled;
#else
    return BA_BOOLEAN_FALSE;
#endif
}

BE_EngineMemoryInformation BE_EngineMemoryInformation_Get(void) {
    return beEngineMemoryInformation;
}

size_t BE_EngineMemoryInformation_GetAllocatedBytes(void) {
    return beEngineMemoryInformation.command.allocatedBytes + beEngineMemoryInformation.ui.allocatedBytes +
           beEngineMemoryInformation.dynamicArray.allocatedBytes + beEngineMemoryInformation.layer.allocatedBytes;
}

BE_EngineMemoryInformation_MemoryType* BE_EngineMemory_GetMemoryTypeInformation(BE_EngineMemory_MemoryType memoryType) {
    switch (memoryType) {
        case BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND:
            return &beEngineMemoryInformation.command;

        case BE_ENGINEMEMORY_MEMORY_TYPE_UI:
            return &beEngineMemoryInformation.ui;

        case BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY:
            return &beEngineMemoryInformation.dynamicArray;

        case BE_ENGINEMEMORY_MEMORY_TYPE_LAYER:
            return &beEngineMemoryInformation.layer;

        default:
            BA_ASSERT_ALWAYS("This shouldn't be reached\n");
    }
}

void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemory_MemoryType memoryType) {
    void* pointer;
    BE_EngineMemoryInformation_MemoryType* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

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

void* BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemoryInformation_MemoryType* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);
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

void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemoryInformation_MemoryType* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(memoryType);

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

void BE_EngineMemory_AddSize(size_t size, BE_EngineMemory_MemoryType memoryType) {
    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Adding to allocated bytes\n"
                         "Size: %zu\n"
                         "Type: %i\n", size, memoryType);
    
    BE_EngineMemory_GetMemoryTypeInformation(memoryType)->allocatedBytes += size;
}

void BE_EngineMemory_RemoveSize(size_t size, BE_EngineMemory_MemoryType memoryType) {
    if (BE_EngineMemory_AllocateDeallocateLogsEnabled())
        BA_LOGGER_TRACE("Removing from allocated bytes\n"
                         "Size: %zu\n"
                         "Type: %i\n", size, memoryType);
    
    BE_EngineMemory_GetMemoryTypeInformation(memoryType)->allocatedBytes -= size;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
