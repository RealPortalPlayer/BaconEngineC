// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Debugging/Assert.h"
#include "EngineMemory.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_EngineMemory_MemoryInformation beEngineMemoryInformation = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}
};

int BE_EngineMemory_AllocDeallocLogsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC, SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC_SHORT, 0);

    return enabled;
}

BE_EngineMemory_MemoryInformation BE_EngineMemory_GetMemoryInformation(void) {
    return beEngineMemoryInformation;
}

size_t BE_EngineMemory_GetAllocatedBytes(void) {
    return beEngineMemoryInformation.command.allocatedBytes + beEngineMemoryInformation.ui.allocatedBytes +
           beEngineMemoryInformation.dynamicArray.allocatedBytes + beEngineMemoryInformation.layer.allocatedBytes +
           beEngineMemoryInformation.server.allocatedBytes;
}

BE_EngineMemory_MemoryTypeInformation* BE_EngineMemory_GetMemoryTypeInformation(BE_EngineMemory_MemoryType memoryType) {
    switch (memoryType) {
        case BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND:       return &beEngineMemoryInformation.command;
        case BE_ENGINEMEMORY_MEMORY_TYPE_UI:            return &beEngineMemoryInformation.ui;
        case BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY: return &beEngineMemoryInformation.dynamicArray;
        case BE_ENGINEMEMORY_MEMORY_TYPE_LAYER:         return &beEngineMemoryInformation.layer;
        case BE_ENGINEMEMORY_MEMORY_TYPE_SERVER:        return &beEngineMemoryInformation.server;
        default: BE_ASSERT_ALWAYS("This shouldn't be reached\n");
    }
}

void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemory_MemoryType memoryType) {
    void* pointer;
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    if (BE_EngineMemory_AllocDeallocLogsEnabled())
        SEC_LOGGER_TRACE("Allocating memory\n"
                         "Size: %zu\n"
                         "Type: %i\n", size, memoryType);

    BE_ASSERT(size > 0, "Size cannot be zero or below\n");
    BE_ASSERT((pointer = malloc(size)) != NULL, "Failed to allocate %zu bytes of data\n", size);

    memoryTypeInformation->allocatedBytes += size;
    memoryTypeInformation->allocatedAmount++;

    return pointer;
}

void BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    if (BE_EngineMemory_AllocDeallocLogsEnabled())
        SEC_LOGGER_TRACE("Reallocating memory\n"
                         "Old Size: %zu\n"
                         "New Size: %zu\n"
                         "Type: %i\n", oldSize, newSize, memoryType);

    BE_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BE_ASSERT(oldSize > 0 && newSize > 0, "Size cannot be zero or below\n");
    BE_ASSERT((pointer = realloc(pointer, newSize)) != NULL, "Failed to reallocate %zu bytes of data\n", newSize);

    memoryTypeInformation->allocatedBytes = memoryTypeInformation->allocatedBytes - oldSize + newSize;
}

void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    if (BE_EngineMemory_AllocDeallocLogsEnabled())
        SEC_LOGGER_TRACE("Deallocating memory\n"
                         "Size: %zu\n"
                         "Type: %i\n", oldSize, memoryType);

    BE_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BE_ASSERT(oldSize > 0, "Size cannot be zero or below\n");
    free(pointer);

    memoryTypeInformation->allocatedBytes -= oldSize;
    memoryTypeInformation->allocatedAmount--;
}
SEC_CPP_SUPPORT_GUARD_END()
